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

#include <MaterialXFormat/File.h>
#include <MaterialXFormat/Util.h>

#include <MaterialXGenShader/Util.h>

#include <sstream>

namespace MaterialX
{
namespace Codegen
{

FragmentGenerator::FragmentGenerator(OptionsPtr options) :
    _options(options)
{
}

FragmentPtr FragmentGenerator::createFragment(const RtToken& name) const
{
    return FragmentPtr(new Fragment(name));
}

FragmentPtr FragmentGenerator::createFragment(const RtNode& node) const
{
    const RtToken& target = getTarget();

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
    frag->setFunctionName(function);
    frag->setSourceCode(source);

    return frag;
}


FragmentGraphPtr FragmentGenerator::createFragmentGraph(const RtToken& name) const
{
    FragmentGraphPtr frag(new FragmentGraph(name));
    return frag;
}

FragmentGraphPtr FragmentGenerator::createFragmentGraph(const RtNode& node) const
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

        graph->finalize();

        return graph;
    }

    return nullptr;
}

} // namespace Codegen
} // namespace MaterialX
