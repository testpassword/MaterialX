//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//
#ifndef MATERIALX_GENCROSS_SPIRV_H
#define MATERIALX_GENCROSS_SPIRV_H

#include <MaterialXGenCross/Export.h>
#include <MaterialXGenGlsl/GlslShaderGenerator.h>

namespace MaterialX
{
    /// Place holder syntax class for SPIRV
    class MX_GENCROSS_API SPIRVSyntax : public Syntax
    {
    public:
        SPIRVSyntax() {}
        static SyntaxPtr create() { return std::make_shared<SPIRVSyntax>(); }
        const string& getInputQualifier() const override { return EMPTY_STRING; }
        const string& getOutputQualifier() const override { return EMPTY_STRING; }
        const string& getConstantQualifier() const override { return EMPTY_STRING; };
        const string& getUniformQualifier() const override { return EMPTY_STRING; };
        const string& getSourceFileExtension() const override { return EMPTY_STRING; };
    };

    class MX_GENCROSS_API SPIRVShaderGenerator : public GlslShaderGenerator
    {

    public:
        SPIRVShaderGenerator();
        
        static ShaderGeneratorPtr create() { return std::make_shared<SPIRVShaderGenerator>(); }

        ShaderPtr generate(const string& name, ElementPtr element, GenContext& context) const;
    };
}
#endif //MATERIALX_GENCROSS_SPIRV_H
