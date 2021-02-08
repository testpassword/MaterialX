//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/OSL/OslContext.h>
#include <MaterialXCodegen/OSL/OslGenerator.h>

namespace MaterialX
{
namespace Codegen
{

OslContext::OslContext(FragmentGeneratorPtr generator, OptionsPtr options) :
    Context(generator, options)
{
    static const string T_FILE_TRANSFORM_UV = "$fileTransformUv";

    // Set the include file to use for uv transformations,
    // depending on the vertical flip flag.
    if (options->fileTextureVerticalFlip)
    {
        _substitutions[T_FILE_TRANSFORM_UV] = "stdlib/" + OslGenerator::TARGET.str() + "/lib/mx_transform_uv_vflip.osl";
    }
    else
    {
        _substitutions[T_FILE_TRANSFORM_UV] = "stdlib/" + OslGenerator::TARGET.str() + "/lib/mx_transform_uv.osl";
    }
}

} // namepspace Codegen
} // namepspace MaterialX
