//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/FragmentGenerator.h>
#include <MaterialXCodegen/Fragment.h>
#include <MaterialXCodegen/Context.h>

#include <MaterialXRuntime/RtApi.h>
#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtNodeGraph.h>
#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtNodeImpl.h>
#include <MaterialXRuntime/Tokens.h>

#include <MaterialXFormat/File.h>
#include <MaterialXFormat/Util.h>

#include <MaterialXGenShader/Util.h>

#include <sstream>

namespace MaterialX
{
namespace Codegen
{

FragmentGenerator::FragmentGenerator(const Context& context) :
    _context(context)
{
}

FragmentPtr FragmentGenerator::createFragment(const RtToken& name) const
{
    return FragmentPtr(new Fragment(name));
}

FragmentPtr FragmentGenerator::createFragment(const RtNode& node) const
{
    const RtToken& target = _context.getTarget();

    if (node.getPrim().hasApi<RtNodeGraph>())
    {
        return createFragmentGraph(node);
    }

    RtPrim nodedefPrim = node.getNodeDef();
    if (!(nodedefPrim && nodedefPrim.hasApi<RtNodeDef>()))
    {
        throw ExceptionRuntimeError("No valid nodedef found for node '" + node.getName().str() + "'");
    }

    RtNodeDef nodedef(nodedefPrim);
    RtPrim nodeImplPrim = nodedef.getNodeImpl(target);
    if (!(nodeImplPrim && nodeImplPrim.hasApi<RtNodeImpl>()))
    {
        throw ExceptionRuntimeError("No valid implementation found for nodedef '" +
            nodedef.getName().str() + "' and target '" + target.str() + "'");
    }

    RtNodeImpl nodeimpl(nodeImplPrim);

    FragmentPtr frag = createFragment(node.getName());
    for (RtAttribute attr : nodedef.getInputs())
    {
        Fragment::Input* input = frag->createInput(attr.getType(), attr.getName());
        RtValue::copy(input->type, attr.getValue(), input->value);
    }
    for (RtAttribute attr : nodedef.getOutputs())
    {
        frag->createOutput(attr.getType(), attr.getName());
    }

    string source = nodeimpl.getSourceCode();
    if (source.empty())
    {
        // TODO: Don't read the file for every new fragment. Store source string in the nodeimpl instead?
        const FilePath path = RtApi::get().getSearchPath().find(nodeimpl.getFile());
        source = readFile(path);
        if (source.empty())
        {
            throw ExceptionRuntimeError("Failed to get source code from file '" + path.asString() +
                "' used by implementation '" + nodeimpl.getName().str() + "'");
        }
    }

    const RtToken& function = nodeimpl.getFunction();
    if (function == EMPTY_TOKEN)
    {
        // Remove any newlines from inline expression.
        source.erase(std::remove(source.begin(), source.end(), '\n'), source.end());
    }

    frag->setFunctionName(function);
    frag->setSourceCode(source);
    frag->setClass(getClassMask(node));

    return frag;
}

FragmentGraphPtr FragmentGenerator::createFragmentGraph(const RtToken& name) const
{
    return FragmentGraphPtr(new FragmentGraph(name));
}

FragmentGraphPtr FragmentGenerator::createFragmentGraph(const RtNode& node, bool publishAllInputs) const
{
    if (node.getPrim().hasApi<RtNodeGraph>())
    {
        RtNodeGraph nodegraph(node.getPrim());

        FragmentGraphPtr graph = createFragmentGraph(nodegraph.getName());
        for (RtAttribute attr : nodegraph.getInputs())
        {
            Fragment::Input* input = graph->createInput(attr.getType(), attr.getName());
            RtValue::copy(input->type, attr.getValue(), input->value);
        }
        for (RtAttribute attr : nodegraph.getOutputs())
        {
            graph->createOutput(attr.getType(), attr.getName());
        }

        for (RtPrim child : nodegraph.getNodes())
        {
            FragmentPtr childFrag = createFragment(child);
            graph->addFragment(childFrag);
        }

        for (RtPrim child : nodegraph.getNodes())
        {
            RtNode childNode(child);
            Fragment* childFragment = graph->getFragment(childNode.getName());

            for (RtAttribute attr : childNode.getInputs())
            {
                RtInput input = attr.asA<RtInput>();
                RtOutput upstream = input.getConnection();
                if (upstream)
                {
                    Fragment::Input* fragmentInput = childFragment->getInput(input.getName());
                    if (upstream.isSocket())
                    {
                        Fragment::Output* graphSocket = graph->getInputSocket(upstream.getName());
                        graph->connect(graphSocket, fragmentInput);
                    }
                    else
                    {
                        Fragment* upstreamChildFragment = graph->getFragment(upstream.getParent().getName());
                        Fragment::Output* upstreamChildOutput = upstreamChildFragment->getOutput(upstream.getName());
                        graph->connect(upstreamChildOutput, fragmentInput);
                    }
                }
            }
        }

        for (RtAttribute attr : nodegraph.getOutputs())
        {
            RtInput socket = nodegraph.getOutputSocket(attr.getName());
            RtOutput upstream = socket.getConnection();
            if (upstream)
            {
                Fragment* upstreamChildFragment = graph->getFragment(upstream.getParent().getName());
                Fragment::Output* upstreamChildOutput = upstreamChildFragment->getOutput(upstream.getName());
                Fragment::Input* fragmentSocket = graph->getOutputSocket(attr.getName());
                graph->connect(upstreamChildOutput, fragmentSocket);
            }
        }

        graph->finalize(_context, publishAllInputs);

        return graph;
    }

    return nullptr;
}

uint32_t FragmentGenerator::getClassMask(const RtNode& node) const
{
    RtNodeDef nodedef = node.getNodeDef();
    if (!nodedef)
    {
        throw ExceptionRuntimeError("FragmentGenerator::getClassMask: No nodedef found for node '" + node.getName().str() + "'");
    }

    const RtToken nodeType = nodedef.getNode();
    const RtToken nodeGroup = nodedef.getNodeGroup();

    // Defaulting to texture node
    uint32_t mask = FragmentClass::TEXTURE;

    // First, check for specific output types
    const RtToken primaryOutputType = node.getOutput().getType();
    if (primaryOutputType == RtType::SURFACESHADER)
    {
        mask = FragmentClass::SURFACE | FragmentClass::SHADER;
    }
    else if (primaryOutputType == RtType::LIGHTSHADER)
    {
        mask = FragmentClass::LIGHT | FragmentClass::SHADER;
    }
    else if (primaryOutputType == RtType::BSDF)
    {
        mask = FragmentClass::BSDF | FragmentClass::CLOSURE;

        // Add additional classifications if the BSDF is restricted to
        // only reflection or transmission

        const RtTypedValue* bsdf = nodedef.getMetadata(Tokens::BSDF, RtType::TOKEN);
        if (bsdf)
        {
            const RtToken val = bsdf->getValue().asToken();
            if (val == Tokens::BSDF_R)
            {
                mask |= FragmentClass::BSDF_R;
            }
            else if (val == Tokens::BSDF_T)
            {
                mask |= FragmentClass::BSDF_T;
            }
        }

        // Check for specific nodedefs.
        static const RtToken ND_layer_bsdf("ND_layer_bsdf");
        static const RtToken ND_thin_film_bsdf("ND_thin_film_bsdf");
        if (nodedef.getName() == ND_layer_bsdf)
        {
            mask |= FragmentClass::LAYER;
        }
        // Check specifically for the thin-film node
        else if (nodedef.getName() == ND_thin_film_bsdf)
        {
            mask |= FragmentClass::THINFILM;
        }
    }
    else if (primaryOutputType == RtType::EDF)
    {
        mask = FragmentClass::EDF | FragmentClass::CLOSURE;
    }
    else if (primaryOutputType == RtType::VDF)
    {
        mask = FragmentClass::VDF | FragmentClass::CLOSURE;
    }
    // Second, check for specific nodes types
    else if (nodeType == Tokens::CONSTANT)
    {
        mask = FragmentClass::TEXTURE | FragmentClass::CONSTANT;
    }
    else if (nodeType == Tokens::COMPARE)
    {
        mask = FragmentClass::TEXTURE | FragmentClass::CONDITIONAL | FragmentClass::IFELSE;
    }
    else if (nodeType == Tokens::SWITCH)
    {
        mask = FragmentClass::TEXTURE | FragmentClass::CONDITIONAL | FragmentClass::SWITCH;
    }
    // Third, check for file texture classification by group name
    else if (nodeGroup == Tokens::TEXTURE2D_GROUPNAME || nodeGroup == Tokens::TEXTURE3D_GROUPNAME)
    {
        mask = FragmentClass::TEXTURE | FragmentClass::FILETEXTURE;
    }

    // Add in group classification
    if (nodeGroup == Tokens::TEXTURE2D_GROUPNAME || nodeGroup == Tokens::PROCEDURAL2D_GROUPNAME)
    {
        mask |= FragmentClass::SAMPLE2D;
    }
    else if (nodeGroup == Tokens::TEXTURE3D_GROUPNAME || nodeGroup == Tokens::PROCEDURAL3D_GROUPNAME)
    {
        mask |= FragmentClass::SAMPLE3D;
    }

    return mask;
}

} // namespace Codegen
} // namespace MaterialX
