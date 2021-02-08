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

OslGenerator::OslGenerator() :
    _syntax(OslSyntax::create())
{
}
FragmentGeneratorPtr OslGenerator::create()
{
    return FragmentGeneratorPtr(new OslGenerator());
}

const RtToken& OslGenerator::getTarget() const
{
    return TARGET;
}

const Syntax& OslGenerator::getSyntax() const
{
    return *_syntax;
}
ContextPtr OslGenerator::createContext(OptionsPtr options)
{
    FragmentGeneratorPtr generator = shared_from_this();
    return std::make_shared<OslContext>(generator, options);
}

} // namepspace Codegen
} // namepspace MaterialX
