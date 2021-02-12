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
    fileTextureVerticalFlip(false),
    directionalAlbedoMethod(DIRECTIONAL_ALBEDO_CURVE_FIT)
{}

OptionsPtr Options::create()
{
    return OptionsPtr(new Options());
}

Context::Context(const OptionsPtr& options) :
    _options(options)
{}

Context::~Context()
{
}

} // namespace Codegen
} // namespace MaterialX
