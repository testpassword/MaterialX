//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/OSL/OslGenerator.h>
#include <MaterialXCodegen/OSL/OslSyntax.h>
#include <MaterialXCodegen/OSL/OslContext.h>

namespace MaterialX
{
namespace Codegen
{

const RtToken OslGenerator::TARGET("genosl");

OslGenerator::OslGenerator(OptionsPtr options) :
    FragmentGenerator(options),
    _syntax(OslSyntax::create())
{
}

FragmentGeneratorPtr OslGenerator::create(OptionsPtr options)
{
    return FragmentGeneratorPtr(new OslGenerator(options));
}

const RtToken& OslGenerator::getTarget() const
{
    return TARGET;
}

const Syntax& OslGenerator::getSyntax() const
{
    return *_syntax;
}

} // namespace Codegen
} // namespace MaterialX
