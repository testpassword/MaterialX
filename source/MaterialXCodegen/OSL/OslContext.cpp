//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/OSL/OslContext.h>
#include <MaterialXCodegen/OSL/OslSyntax.h>
#include <MaterialXCodegen/OSL/OslFragmentGenerator.h>
#include <MaterialXCodegen/OSL/OslFragmentCompiler.h>

namespace MaterialX
{
namespace Codegen
{

const RtToken OslContext::TARGET("genosl");

OslContext::OslContext(OptionsPtr options) :
    Context(options),
    _syntax(OslSyntax::create())
{
    _generator = OslFragmentGenerator::create(*this);
    _compiler = OslFragmentCompiler::create(*this);
}

ContextPtr OslContext::create(OptionsPtr options)
{
    return ContextPtr(new OslContext(options));
}

const RtToken& OslContext::getTarget() const
{
    return TARGET;
}

const Syntax& OslContext::getSyntax() const
{
    return *_syntax;
}

const FragmentGenerator& OslContext::getGenerator() const
{
    return *_generator;
}

const FragmentCompiler& OslContext::getCompiler() const
{
    return *_compiler;
}

} // namespace Codegen
} // namespace MaterialX
