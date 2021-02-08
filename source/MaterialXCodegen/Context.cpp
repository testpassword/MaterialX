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

Options::Options() :
    fileTextureVerticalFlip(false)
{}

OptionsPtr Options::create()
{
    return OptionsPtr(new Options());
}

Context::Context(FragmentGeneratorPtr generator) :
    _generator(generator)
{}

Context::~Context()
{
}

} // namespace Codegen
} // namespace MaterialX
