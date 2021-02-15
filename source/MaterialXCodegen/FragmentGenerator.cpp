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
    registerFragmentClass(FragmentGraph::className(), FragmentGraph::create);
    registerFragmentClass(SourceFragment::className(), SourceFragment::create);
}

FragmentPtr FragmentGenerator::createFragment(const RtToken& className, const RtToken& instanceName) const
{
    auto it = _creatorFunctions.find(className);
    return it != _creatorFunctions.end() ? it->second(instanceName) : nullptr;
}

FragmentPtr FragmentGenerator::createFragment(const RtNode& node) const
{
    if (node.getPrim().hasApi<RtNodeGraph>())
    {
        return createFragmentGraph(node);
    }

    RtPrim nodedefPrim = node.getNodeDef();
    if (!(nodedefPrim && nodedefPrim.hasApi<RtNodeDef>()))
    {
        throw ExceptionRuntimeError("No valid nodedef found for node '" + node.getName().str() + "'");
    }

    const RtToken& target = _context.getTarget();

    RtNodeDef nodedef(nodedefPrim);
    RtPrim nodeImplPrim = nodedef.getNodeImpl(target);
    if (!(nodeImplPrim && nodeImplPrim.hasApi<RtNodeImpl>()))
    {
        throw ExceptionRuntimeError("No valid implementation found for nodedef '" +
            nodedef.getName().str() + "' and target '" + target.str() + "'");
    }

    RtNodeImpl nodeimpl(nodeImplPrim);

    // Try creation from the nodeimpl name (fragment class name).
    FragmentPtr frag = createFragment(nodeimpl.getName(), node.getName());
    if (!frag)
    {
        // No fragment of this class has been registerd so
        // fallback to a source fragment as default class.
        frag = SourceFragment::create(node.getName());
    }

    // Create fragment ports according to the nodedef.
    for (RtAttribute attr : nodedef.getInputs())
    {
        Fragment::Input* input = frag->createInput(attr.getType(), attr.getName());
        RtValue::copy(input->type, attr.getValue(), input->value);
    }
    for (RtAttribute attr : nodedef.getOutputs())
    {
        frag->createOutput(attr.getType(), attr.getName());
    }

    const RtToken& function = nodeimpl.getFunction();
    frag->setFunctionName(function);
    frag->setClassification(getClassificationMask(node));

    if (frag->getType() == FRAGMENT_TYPE_SOURCE_CODE)
    {
        SourceFragment* sourceFragment = frag->asA<SourceFragment>();

        // Get the source code from metadata on the nodeimpl.
        RtTypedValue* sourceCodeData = nodeimpl.addMetadata(Tokens::SOURCECODE, RtType::STRING);
        string* contentPtr = &sourceCodeData->getValue().asString();
        if (contentPtr->empty())
        {
            // No source given so try loading it from file.
            const FilePath path = RtApi::get().getSearchPath().find(nodeimpl.getFile());
            *contentPtr = readFile(path);
            if (contentPtr->empty())
            {
                throw ExceptionRuntimeError("Failed to get source code from file '" + path.asString() +
                    "' used by implementation '" + nodeimpl.getName().str() + "'");
            }

            // Remove newline if it's an inline expression.
            if (sourceFragment->isInline())
            {
                contentPtr->erase(std::remove(contentPtr->begin(), contentPtr->end(), '\n'), contentPtr->end());
            }
        }

        // Assign this source code to the fragment.
        sourceFragment->setSourceCode(contentPtr);
    }

    return frag;
}

FragmentPtr FragmentGenerator::createFragmentGraph(const RtNode& node, bool publishAllInputs) const
{
    if (node.getPrim().hasApi<RtNodeGraph>())
    {
        RtNodeGraph nodegraph(node.getPrim());

        FragmentPtr graphFragment = FragmentGraph::create(nodegraph.getName());
        FragmentGraph* graph = graphFragment->asA<FragmentGraph>();
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

        return graphFragment;
    }

    return nullptr;
}

void FragmentGenerator::registerFragmentClass(const RtToken& className, FragmentCreatorFunction creator)
{
    // NOTE: Allow overwriting any previously registered creators
    // in order for derived generators to override a class creator function.
    _creatorFunctions[className] = creator;
}

uint32_t FragmentGenerator::getClassificationMask(const RtNode& node) const
{
    RtNodeDef nodedef = node.getNodeDef();
    if (!nodedef)
    {
        throw ExceptionRuntimeError("FragmentGenerator::getClassMask: No nodedef found for node '" + node.getName().str() + "'");
    }

    const RtToken nodeType = nodedef.getNode();
    const RtToken nodeGroup = nodedef.getNodeGroup();

    // Defaulting to texture node
    uint32_t mask = FragmentClassification::TEXTURE;

    // First, check for specific output types
    const RtToken primaryOutputType = node.getOutput().getType();
    if (primaryOutputType == RtType::SURFACESHADER)
    {
        mask = FragmentClassification::SURFACE | FragmentClassification::SHADER;
    }
    else if (primaryOutputType == RtType::LIGHTSHADER)
    {
        mask = FragmentClassification::LIGHT | FragmentClassification::SHADER;
    }
    else if (primaryOutputType == RtType::BSDF)
    {
        mask = FragmentClassification::BSDF | FragmentClassification::CLOSURE;

        // Add additional classifications if the BSDF is restricted to
        // only reflection or transmission

        const RtTypedValue* bsdf = nodedef.getMetadata(Tokens::BSDF, RtType::TOKEN);
        if (bsdf)
        {
            const RtToken val = bsdf->getValue().asToken();
            if (val == Tokens::BSDF_R)
            {
                mask |= FragmentClassification::BSDF_R;
            }
            else if (val == Tokens::BSDF_T)
            {
                mask |= FragmentClassification::BSDF_T;
            }
        }

        // Check for specific nodedefs.
        static const RtToken ND_layer_bsdf("ND_layer_bsdf");
        static const RtToken ND_thin_film_bsdf("ND_thin_film_bsdf");
        if (nodedef.getName() == ND_layer_bsdf)
        {
            mask |= FragmentClassification::LAYER;
        }
        // Check specifically for the thin-film node
        else if (nodedef.getName() == ND_thin_film_bsdf)
        {
            mask |= FragmentClassification::THINFILM;
        }
    }
    else if (primaryOutputType == RtType::EDF)
    {
        mask = FragmentClassification::EDF | FragmentClassification::CLOSURE;
    }
    else if (primaryOutputType == RtType::VDF)
    {
        mask = FragmentClassification::VDF | FragmentClassification::CLOSURE;
    }
    // Second, check for specific nodes types
    else if (nodeType == Tokens::CONSTANT)
    {
        mask = FragmentClassification::TEXTURE | FragmentClassification::CONSTANT;
    }
    else if (nodeType == Tokens::COMPARE)
    {
        mask = FragmentClassification::TEXTURE | FragmentClassification::CONDITIONAL | FragmentClassification::IFELSE;
    }
    else if (nodeType == Tokens::SWITCH)
    {
        mask = FragmentClassification::TEXTURE | FragmentClassification::CONDITIONAL | FragmentClassification::SWITCH;
    }
    // Third, check for file texture classification by group name
    else if (nodeGroup == Tokens::TEXTURE2D_GROUPNAME || nodeGroup == Tokens::TEXTURE3D_GROUPNAME)
    {
        mask = FragmentClassification::TEXTURE | FragmentClassification::FILETEXTURE;
    }

    // Add in group classification
    if (nodeGroup == Tokens::TEXTURE2D_GROUPNAME || nodeGroup == Tokens::PROCEDURAL2D_GROUPNAME)
    {
        mask |= FragmentClassification::SAMPLE2D;
    }
    else if (nodeGroup == Tokens::TEXTURE3D_GROUPNAME || nodeGroup == Tokens::PROCEDURAL3D_GROUPNAME)
    {
        mask |= FragmentClassification::SAMPLE3D;
    }

    return mask;
}

} // namespace Codegen
} // namespace MaterialX
