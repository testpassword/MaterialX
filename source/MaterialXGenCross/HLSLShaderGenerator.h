//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//
#ifndef MATERIALX_GENCROSS_HLSL_H
#define MATERIALX_GENCROSS_HLSL_H

#include <MaterialXGenCross/Export.h>
#include <MaterialXGenGlsl/GlslShaderGenerator.h>

namespace MaterialX
{

    /// Place holder syntax class for HLSL
    class MX_GENCROSS_API HLSLSyntax : public Syntax
    {
    public:
        HLSLSyntax() {}
        static SyntaxPtr create() { return std::make_shared<HLSLSyntax>(); }
        const string& getInputQualifier() const override { return EMPTY_STRING; }
        const string& getOutputQualifier() const override { return EMPTY_STRING; }
        const string& getConstantQualifier() const override { return EMPTY_STRING; };
        const string& getUniformQualifier() const override { return EMPTY_STRING; };
        const string& getSourceFileExtension() const override { return EMPTY_STRING; };
    };

    class MX_GENCROSS_API HLSLShaderGenerator : public GlslShaderGenerator
    {
    private:
        void crosscompile(ShaderStage& stage, std::string& crosscompileSource) const;

    public:
        HLSLShaderGenerator();

        static ShaderGeneratorPtr create() { return std::make_shared<HLSLShaderGenerator>(); }
        ShaderPtr generate(const string& name, ElementPtr element, GenContext& context) const;
    };
}
#endif //MATERIALX_GENCROSS_HLSL_H
