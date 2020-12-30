//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXRuntime/Codegen/RtCodegenContext.h>
#include <MaterialXRuntime/Codegen/RtCodeGenerator.h>

namespace MaterialX
{

RtOslContext::RtOslContext(RtCodeGeneratorConstPtr generator, RtCodegenOptionsPtr options) :
    RtCodegenContext(generator, options)
{
    static const string T_FILE_TRANSFORM_UV = "$fileTransformUv";

    // Set the include file to use for uv transformations,
    // depending on the vertical flip flag.
    if (options->fileTextureVerticalFlip)
    {
        _substitutions[T_FILE_TRANSFORM_UV] = "stdlib/" + RtOslGenerator::TARGET.str() + "/lib/mx_transform_uv_vflip.osl";
    }
    else
    {
        _substitutions[T_FILE_TRANSFORM_UV] = "stdlib/" + RtOslGenerator::TARGET.str() + "/lib/mx_transform_uv.osl";
    }
}

}
