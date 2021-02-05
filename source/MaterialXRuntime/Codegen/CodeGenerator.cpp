//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXRuntime/Codegen/CodeGenerator.h>
#include <MaterialXRuntime/Codegen/CodegenContext.h>
#include <MaterialXRuntime/Codegen/OslSyntax.h>
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

CodegenResultPtr CodegenResult::create()
{
    return std::make_shared<CodegenResult>();
}

void CodegenResult::addFragment(const FragmentPtr& fragment)
{
    if (getFragment(fragment->getName()))
    {
        throw ExceptionShaderGenError("A fragment named '" + fragment->getName().str() + "' already exists");
    }

    _fragments[fragment->getName()] = fragment;
    _fragmentsOrder.push_back(fragment);
}

void CodegenResult::removeFragment(const RtToken& name)
{
    auto it = _fragments.find(name);
    if (it != _fragments.end())
    {
        _fragmentsOrder.erase(std::find(_fragmentsOrder.begin(), _fragmentsOrder.end(), it->second));
        _fragments.erase(it);
    }
}

size_t CodegenResult::numFragments() const
{
    return _fragmentsOrder.size();
}

FragmentPtr CodegenResult::getFragment(size_t index) const
{
    return _fragmentsOrder[index];
}

FragmentPtr CodegenResult::getFragment(const RtToken& name) const
{
    auto it = _fragments.find(name);
    return it != _fragments.end() ? it->second : nullptr;
}

CodegenOptionsPtr CodeGenerator::createOptions() const
{
    return std::make_shared<CodegenOptions>();
}

FragmentPtr CodeGenerator::createFragment(const RtNode& node) const
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

FragmentPtr CodeGenerator::createFragmentGraph(const RtNode& /*node*/) const
{
    return nullptr;
}

const RtToken OslGenerator::TARGET("genosl");

OslGenerator::OslGenerator() :
    _syntax(OslSyntax::create())
{
}

CodeGeneratorPtr OslGenerator::create()
{
    return CodeGeneratorPtr(new OslGenerator());
}

const RtToken& OslGenerator::getTarget() const
{
    return TARGET;
}

const Syntax& OslGenerator::getSyntax() const
{
    return *_syntax;
}

CodegenContextPtr OslGenerator::createContext(CodegenOptionsPtr options)
{
    CodeGeneratorPtr generator = shared_from_this();
    return std::make_shared<OslContext>(generator, options);
}

CodegenResultPtr OslGenerator::generate(const RtPrim& prim, const RtPath& /*path*/, CodegenContextPtr context) const
{
    if (!prim.hasApi<RtNode>())
    {
        throw ExceptionRuntimeError("Unsupported prim attached to OslGenerator API");
    }

    RtNode node(prim);
    FragmentPtr frag = createFragment(node);

    CodegenResultPtr result = CodegenResult::create();
    result->addFragment(frag);

    return result;
}

} // namepspace Codegen
} // namepspace MaterialX
