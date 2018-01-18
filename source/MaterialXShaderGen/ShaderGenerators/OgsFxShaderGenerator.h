#ifndef MATERIALX_OGSFX_CODEGENERATOR_H
#define MATERIALX_OGSFX_CODEGENERATOR_H

#include <MaterialXShaderGen/ShaderGenerators/GlslShaderGenerator.h>
#include <MaterialXShaderGen/Shader.h>

namespace MaterialX
{

/// A GLSL shader generator targeting the OgsFX file format
class OgsFxShaderGenerator : public GlslShaderGenerator
{
    DECLARE_SHADER_GENERATOR(OgsFxShaderGenerator)
public:
    /// Constructor
    OgsFxShaderGenerator();

    /// Return the v-direction used by the target system
    Shader::VDirection getTargetVDirection() const override { return Shader::VDirection::DOWN; }

    /// Generate a shader starting from the given element, translating 
    /// the element and all dependencies upstream into shader code.
    ShaderPtr generate(const string& shaderName, ElementPtr element) override;

    void emitShaderBody(Shader &shader) override;

    /// Emit a shader uniform input variable
    void emitUniform(const string& name, const string& type, const ValuePtr& value, Shader& shader) override;

    ///
    bool shouldPublish(const ValueElement* port, string& publicName) const override;

    /// Emit the final output expression
    void emitFinalOutput(Shader& shader) const override;
};

}

#endif