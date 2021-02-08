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

OptionsPtr FragmentGenerator::createOptions() const
{
    return std::make_shared<Options>();
}

FragmentPtr FragmentGenerator::createFragment(const RtNode& node) const
{
    if (node.getPrim().hasApi<RtNodeGraph>())
    {
        throw ExceptionRuntimeError("Nodegraphs not supported yet!");
    }

    RtPrim nodedefPrim = node.getNodeDef();
    if (!(nodedefPrim && nodedefPrim.hasApi<RtNodeDef>()))
    {
        throw ExceptionRuntimeError("No valid nodedef found for node '" + node.getName().str() + "'");
    }

    RtNodeDef nodedef(nodedefPrim);
    const RtToken& target = getTarget();
    RtPrim nodeImplPrim = nodedef.getNodeImpl(target);
    if (!(nodeImplPrim && nodeImplPrim.hasApi<RtNodeImpl>()))
    {
        throw ExceptionRuntimeError("No valid implementation found for nodedef '" +
            nodedef.getName().str() + "' and target '" + target.str() + "'");
    }

    RtNodeImpl nodeimpl(nodeImplPrim);

    FragmentPtr frag = Fragment::create(nodeimpl.getName());
    for (RtAttribute attr : nodedef.getInputs())
    {
        Fragment::Input* input = frag->createInput(attr.getType(), attr.getName());
        RtValue::copy(input->type, attr.getValue(), input->value);
    }
    for (RtAttribute attr : nodedef.getOutputs())
    {
        frag->createOutput(attr.getType(), attr.getName());
    }

    const string& source = nodeimpl.getSourceCode();
    if (!source.empty())
    {
        frag->setSourceCode(source);
    }
    else
    {
        // TODO: Don't read the file for every new fragment. Store source string in the nodeimpl instead?
        const FilePath path = RtApi::get().getSearchPath().find(nodeimpl.getFile());
        string contents = readFile(path);
        if (contents.empty())
        {
            throw ExceptionShaderGenError("Failed to get source code from file '" + path.asString() +
                "' used by implementation '" + nodeimpl.getName().str() + "'");
        }
        frag->setSourceCode(contents);
    }

    return frag;
}

FragmentGraphPtr FragmentGenerator::createFragmentGraph(const RtNode& /*node*/) const
{
    return nullptr;
}

} // namepspace Codegen
} // namepspace MaterialX
