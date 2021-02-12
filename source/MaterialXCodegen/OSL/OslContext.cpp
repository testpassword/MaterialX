//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/OSL/OslContext.h>
#include <MaterialXCodegen/OSL/OslSyntax.h>
#include <MaterialXCodegen/OSL/OslGenerator.h>
#include <MaterialXCodegen/OSL/OslCompiler.h>

namespace MaterialX
{
namespace Codegen
{

const RtToken OslContext::TARGET("genosl");

OslContext::OslContext(OptionsPtr options) :
    Context(options),
    _syntax(OslSyntax::create())
{
    static const string T_FILE_TRANSFORM_UV = "$fileTransformUv";

    // Set the include file to use for uv transformations,
    // depending on the vertical flip flag.
    if (options->fileTextureVerticalFlip)
    {
        _substitutions[T_FILE_TRANSFORM_UV] = "stdlib/" + TARGET.str() + "/lib/mx_transform_uv_vflip.osl";
    }
    else
    {
        _substitutions[T_FILE_TRANSFORM_UV] = "stdlib/" + TARGET.str() + "/lib/mx_transform_uv.osl";
    }
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

FragmentGeneratorPtr OslContext::createGenerator() const
{
    return OslGenerator::create(*this);
}

FragmentCompilerPtr OslContext::createCompiler() const
{
    return OslCompiler::create(*this);
}

} // namespace Codegen
} // namespace MaterialX
