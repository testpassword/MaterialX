//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/Context.h>
#include <MaterialXCodegen/FragmentGenerator.h>

namespace MaterialX
{
namespace Codegen
{

Context::Context(FragmentGeneratorPtr generator, OptionsPtr options) :
    _generator(generator),
    _options(options)
{}

Context::~Context()
{
}

} // namepspace Codegen
} // namepspace MaterialX
