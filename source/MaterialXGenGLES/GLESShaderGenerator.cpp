//
// TM & (c) 2032 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenGLES/GLESShaderGenerator.h>

#include <MaterialXGenShader/Nodes/HwImageNode.h>

namespace MaterialX
{

const string GLESShaderGenerator::TARGET = "gles";


GLESShaderGenerator::GLESShaderGenerator()
    : GlslShaderGenerator()
{
    // GL ES specific keywords... etc
    const StringSet reservedWords = { "precision", "highp", "lowp" }; 
    _syntax->registerReservedWords(reservedWords);

    // Register override implementations
    // <!-- <image> -->
    registerImplementation("IM_image_float_" + GLESShaderGenerator::TARGET, HwImageNode::create);
    registerImplementation("IM_image_color3_" + GLESShaderGenerator::TARGET, HwImageNode::create);
    registerImplementation("IM_image_color4_" + GLESShaderGenerator::TARGET, HwImageNode::create);
    registerImplementation("IM_image_vector2_" + GLESShaderGenerator::TARGET, HwImageNode::create);
    registerImplementation("IM_image_vector3_" + GLESShaderGenerator::TARGET, HwImageNode::create);
    registerImplementation("IM_image_vector4_" + GLESShaderGenerator::TARGET, HwImageNode::create);
}

void GLESShaderGenerator::emitVariableDeclaration(const ShaderPort* variable, const string& qualifier,
                                                  GenContext& context, ShaderStage& stage,
                                                  bool assignValue) const
{
    GlslShaderGenerator::emitVariableDeclaration(variable, qualifier, context, stage, assignValue);
}


}
