//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/OSL/OslGenerator.h>
#include <MaterialXCodegen/OSL/OslSyntax.h>
#include <MaterialXCodegen/OSL/OslContext.h>

#include <MaterialXRuntime/RtApi.h>

namespace MaterialX
{
namespace Codegen
{

OslGenerator::OslGenerator(const Context& context) :
    FragmentGenerator(context)
{
}

FragmentGeneratorPtr OslGenerator::create(const Context& context)
{
    return FragmentGeneratorPtr(new OslGenerator(context));
}

} // namespace Codegen
} // namespace MaterialX
