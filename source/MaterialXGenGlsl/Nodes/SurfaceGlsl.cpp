#include <MaterialXGenGlsl/Nodes/SurfaceGlsl.h>

#include <MaterialXGenGlsl/GlslShaderGenerator.h>

namespace MaterialX
{

namespace
{
    static const string LIGHT_LOOP_BEGIN =
        "vec3 V = normalize(u_viewPosition - vd.positionWorld);\n"
        "int numLights = numActiveLightSources();\n"
        "lightshader lightShader;\n"
        "for (int activeLightIndex = 0; activeLightIndex < numLights; ++activeLightIndex)\n";

    static const string LIGHT_CONTRIBUTION =
        "sampleLightSource(u_lightData[activeLightIndex], vd.positionWorld, lightShader);\n"
        "vec3 L = lightShader.direction;\n";
}

SgImplementationPtr SurfaceGlsl::create()
{
    return std::make_shared<SurfaceGlsl>();
}

void SurfaceGlsl::createVariables(const SgNode& /*node*/, ShaderGenerator& /*shadergen*/, Shader& shader_)
{
    // TODO: 
    // The surface shader needs position, normal, view position and light sources. We should solve this by adding some 
    // dependency mechanism so this implementation can be set to depend on the PositionGlsl, NormalGlsl
    // ViewDirectionGlsl and LightGlsl nodes instead? This is where the MaterialX attribute "internalgeomprops" 
    // is needed.
    //
    HwShader& shader = static_cast<HwShader&>(shader_);

    shader.createAppData(Type::VECTOR3, "i_position");
    shader.createAppData(Type::VECTOR3, "i_normal");
    shader.createVertexData(Type::VECTOR3, "positionWorld");
    shader.createVertexData(Type::VECTOR3, "normalWorld");
    shader.createUniform(HwShader::VERTEX_STAGE, HwShader::PRIVATE_UNIFORMS, Type::MATRIX44, "u_worldInverseTransposeMatrix");
    shader.createUniform(HwShader::PIXEL_STAGE, HwShader::PRIVATE_UNIFORMS, Type::VECTOR3, "u_viewPosition");
    shader.createUniform(HwShader::PIXEL_STAGE, HwShader::PRIVATE_UNIFORMS, Type::INTEGER, "u_numActiveLightSources",
        EMPTY_STRING, Value::createValue<int>(0));
}

void SurfaceGlsl::emitFunctionCall(const SgNode& node, const SgNodeContext& /*context*/, ShaderGenerator& shadergen, Shader& shader_)
{
    HwShader& shader = static_cast<HwShader&>(shader_);

    const string& blockInstance = shader.getVertexDataBlock().instance;
    const string blockPrefix = blockInstance.length() ? blockInstance + "." : EMPTY_STRING;

    BEGIN_SHADER_STAGE(shader, HwShader::VERTEX_STAGE)
        if (!shader.isCalculated("positionWorld"))
        {
            shader.addLine(blockPrefix + "positionWorld = hPositionWorld.xyz");
            shader.setCalculated("positionWorld");
        }
        if (!shader.isCalculated("normalWorld"))
        {
            shader.setCalculated("normalWorld");
            shader.addLine(blockPrefix + "normalWorld = normalize((u_worldInverseTransposeMatrix * vec4(i_normal, 0)).xyz)");
        }
    END_SHADER_STAGE(shader, HwShader::VERTEX_STAGE)

    BEGIN_SHADER_STAGE(shader, HwShader::PIXEL_STAGE)

    GlslShaderGenerator& glslgen = static_cast<GlslShaderGenerator&>(shadergen);

    // Declare the output variable
    shader.beginLine();
    glslgen.emitOutput(node.getOutput(), true, true, shader);
    shader.endLine();

    shader.beginScope();

    if (shader.hasTransparency())
    {
        shader.beginLine();
        shader.addStr("float surfaceOpacity = ");
        glslgen.emitInput(node.getInput("opacity"), shader);
        shader.endLine();
        // Early out for 100% cutout transparency
        shader.addLine("if (surfaceOpacity < 0.001)", false);
        shader.beginScope();
        shader.addLine("discard");
        shader.endScope();
        shader.newLine();
    }

    const SgOutput* output = node.getOutput();
    const string outColor = output->name + ".color";
    const string outTransparency = output->name + ".transparency";

    //
    // Handle direct lighting
    //

    const string normalWorld = blockPrefix + "normalWorld";
    const string positionWorld = blockPrefix + "positionWorld";

    shader.addComment("Light loop");
    shader.addBlock(LIGHT_LOOP_BEGIN, shadergen);
    shader.beginScope();

    shader.addBlock(LIGHT_CONTRIBUTION, shadergen);
    shader.newLine();

    shader.addComment("Calculate the BSDF response for this light source");
    string bsdf;
    glslgen.emitBsdfNodes(node, GlslShaderGenerator::NODE_CONTEXT_BSDF_REFLECTION, 
        GlslShaderGenerator::LIGHT_DIR, GlslShaderGenerator::VIEW_DIR, shader, bsdf);
    shader.newLine();

    shader.addComment("Accumulate the light's contribution");
    shader.addLine(outColor + " += lightShader.intensity * " + bsdf);

    shader.endScope();
    shader.newLine();

    //
    // Handle indirect lighting.
    //

    shader.addComment("Add surface emission");
    shader.beginScope();
    string emission;
    glslgen.emitEdfNodes(node, normalWorld, normalWorld, shader, emission);
    shader.addLine(outColor + " += " + emission);
    shader.endScope();
    shader.newLine();

    shader.addComment("Add indirect contribution");
    shader.beginScope();
    glslgen.emitBsdfNodes(node, GlslShaderGenerator::NODE_CONTEXT_BSDF_INDIRECT, 
        GlslShaderGenerator::VIEW_DIR, GlslShaderGenerator::VIEW_DIR, shader, bsdf);
    shader.newLine();
    shader.addLine(outColor + " += " + bsdf);
    shader.endScope();
    shader.newLine();

    // Handle surface transparency
    //
    if (shader.hasTransparency())
    {
        shader.addComment("Calculate the BSDF transmission for viewing direction");
        shader.beginScope();
        glslgen.emitBsdfNodes(node, GlslShaderGenerator::NODE_CONTEXT_BSDF_TRANSMISSION,
            GlslShaderGenerator::VIEW_DIR, GlslShaderGenerator::VIEW_DIR, shader, bsdf);
        shader.addLine(outTransparency + " = " + bsdf);
        shader.endScope();
        shader.newLine();

        shader.addComment("Mix in opacity which affect the total result");
        shader.addLine(outColor + " *= surfaceOpacity");
        shader.addLine(outTransparency + " = mix(vec3(1.0), " + outTransparency + ", surfaceOpacity)");
    }
    else
    {
        shader.addLine(outTransparency + " = vec3(0.0)");
    }

    shader.endScope();
    shader.newLine();

    END_SHADER_STAGE(shader, HwShader::PIXEL_STAGE)
}

} // namespace MaterialX