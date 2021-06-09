//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//
#include <MaterialXGenCross/MaterialXGenCross.h>
#include <MaterialXGenCross/SPIRVShaderGenerator.h>
#include <MaterialXGenShader/HwShaderGenerator.h>
#include <MaterialXGenShader/Shader.h>

namespace MaterialX
{
    SPIRVShaderGenerator::SPIRVShaderGenerator() :
        GlslShaderGenerator()
    {}

    ShaderPtr SPIRVShaderGenerator::generate(const string& name, ElementPtr element,
        GenContext& context) const
    {

        MaterialX::ShaderPtr hwShader = GlslShaderGenerator::generate(name, element, context);
        MaterialX::ShaderStage& ps = hwShader->getStage(MaterialX::Stage::PIXEL);
        std::string crosscompileSource;

        //TODO: figure out a better way to reuse stage
        //      configure cross compile 
        ShaderGraphPtr graph = ShaderGraph::create(nullptr, name, element, context);
        ShaderPtr shader = std::make_shared<Shader>(name, graph);
        createStage(MaterialX::Stage::CROSS_PIXEL, *shader);

        crosscompile(ps, crosscompileSource);
        MaterialX::ShaderStage& spv = hwShader->getStage(MaterialX::Stage::CROSS_PIXEL);
        emitString(crosscompileSource, spv);
        return hwShader;
    }

    void SPIRVShaderGenerator::crosscompile(ShaderStage& stage, std::string& crosscompileSource) const
    {
        Cross::initialize();
        const std::string functionName(stage.getFunctionName());
        std::vector<uint32_t> spirvCode = Cross::glslToSpirv("", stage.getSourceCode());
        Cross::finalize();

        // TODO: Performance Encode as string 
        std::stringstream s;
        std::copy(spirvCode.begin(), spirvCode.end(), std::ostream_iterator<uint32_t>(s, " "));
        crosscompileSource = s.str();
    }
}
