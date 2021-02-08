//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/Fragment.h>
#include <MaterialXCodegen/FragmentGenerator.h>
#include <MaterialXCodegen/Context.h>

#include <MaterialXRuntime/RtApi.h>

#include <MaterialXFormat/File.h>

#include <sstream>

namespace MaterialX
{
namespace Codegen
{

Fragment::Fragment(const RtToken& name) :
    _name(name)
{
}

Fragment::Input* Fragment::createInput(const RtToken& type, const RtToken& name)
{
    Input* input = new Input();
    input->parent = this;
    input->type = type;
    input->name = name;
    string variable = _name.str() + "_" + name.str();
    input->variable = RtToken(variable);
    input->value = RtValue::createNew(type, _allocator);
    input->connection = nullptr;
    _inputs.push_back(input);
    return input;
}

Fragment::Output* Fragment::createOutput(const RtToken& type, const RtToken& name)
{
    Output* output = new Output();
    output->parent = this;
    output->type = type;
    string variable = _name.str() + "_" + name.str();
    output->variable = RtToken(variable);
    _outputs.push_back(output);
    return output;
}

void Fragment::setSourceCodeFunction(const RtToken& functionName, const string& sourceCode)
{
    _sourceCode = sourceCode;
    _functionName = functionName;
}

void Fragment::setSourceCodeInline(const string& sourceCode)
{
    _sourceCode = sourceCode;
    _functionName = EMPTY_TOKEN;
}

/*
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
*/

} // namespace Codegen
} // namespace MaterialX
