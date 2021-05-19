//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_GLESSHADERGENERATOR_H
#define MATERIALX_GLESSHADERGENERATOR_H

/// @file
/// OpenGL ES shader generator

#include <MaterialXGenGlsl/GlslShaderGenerator.h>

namespace MaterialX
{

using GLESShaderGeneratorPtr  = shared_ptr<class GLESShaderGenerator>;

/// @class ArnoldShaderGenerator 
/// An OpenGL ES generator 
class GLESShaderGenerator : public GlslShaderGenerator
{
  public:
    GLESShaderGenerator();

    static ShaderGeneratorPtr create() { return std::make_shared<GLESShaderGenerator>(); }

    /// Return a unique identifyer for the target this generator is for
    const string& getTarget() const override { return TARGET; }

    /// Unique identifyer for this generator target
    static const string TARGET;

   protected:
    /// Emit a shader input variable
    void emitVariableDeclaration(const ShaderPort* variable, const string& qualifier,
        GenContext& context, ShaderStage& stage,
        bool assignValue = true) const override;
};

}

#endif
