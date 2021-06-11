
//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include "MaterialXGenCross.h"

// glslang headers.
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>

// SPIRV-Cross headers.
#include <spirv_cross/spirv_hlsl.hpp>
#include <spirv_cross/spirv_parser.hpp>
#include <spirv_cross/spirv_reflect.hpp>

namespace MaterialX
{
    namespace Stage
    {
        const string CROSS_PIXEL = "cross_pixel";
    }

    namespace Cross
    {
        const TBuiltInResource defaultTBuiltInResource = {
            /* .MaxLights = */ 32,
            /* .MaxClipPlanes = */ 6,
            /* .MaxTextureUnits = */ 32,
            /* .MaxTextureCoords = */ 32,
            /* .MaxVertexAttribs = */ 64,
            /* .MaxVertexUniformComponents = */ 4096,
            /* .MaxVaryingFloats = */ 64,
            /* .MaxVertexTextureImageUnits = */ 32,
            /* .MaxCombinedTextureImageUnits = */ 80,
            /* .MaxTextureImageUnits = */ 32,
            /* .MaxFragmentUniformComponents = */ 4096,
            /* .MaxDrawBuffers = */ 32,
            /* .MaxVertexUniformVectors = */ 128,
            /* .MaxVaryingVectors = */ 8,
            /* .MaxFragmentUniformVectors = */ 16,
            /* .MaxVertexOutputVectors = */ 16,
            /* .MaxFragmentInputVectors = */ 15,
            /* .MinProgramTexelOffset = */ -8,
            /* .MaxProgramTexelOffset = */ 7,
            /* .MaxClipDistances = */ 8,
            /* .MaxComputeWorkGroupCountX = */ 65535,
            /* .MaxComputeWorkGroupCountY = */ 65535,
            /* .MaxComputeWorkGroupCountZ = */ 65535,
            /* .MaxComputeWorkGroupSizeX = */ 1024,
            /* .MaxComputeWorkGroupSizeY = */ 1024,
            /* .MaxComputeWorkGroupSizeZ = */ 64,
            /* .MaxComputeUniformComponents = */ 1024,
            /* .MaxComputeTextureImageUnits = */ 16,
            /* .MaxComputeImageUniforms = */ 8,
            /* .MaxComputeAtomicCounters = */ 8,
            /* .MaxComputeAtomicCounterBuffers = */ 1,
            /* .MaxVaryingComponents = */ 60,
            /* .MaxVertexOutputComponents = */ 64,
            /* .MaxGeometryInputComponents = */ 64,
            /* .MaxGeometryOutputComponents = */ 128,
            /* .MaxFragmentInputComponents = */ 128,
            /* .MaxImageUnits = */ 8,
            /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
            /* .MaxCombinedShaderOutputResources = */ 8,
            /* .MaxImageSamples = */ 0,
            /* .MaxVertexImageUniforms = */ 0,
            /* .MaxTessControlImageUniforms = */ 0,
            /* .MaxTessEvaluationImageUniforms = */ 0,
            /* .MaxGeometryImageUniforms = */ 0,
            /* .MaxFragmentImageUniforms = */ 8,
            /* .MaxCombinedImageUniforms = */ 8,
            /* .MaxGeometryTextureImageUnits = */ 16,
            /* .MaxGeometryOutputVertices = */ 256,
            /* .MaxGeometryTotalOutputComponents = */ 1024,
            /* .MaxGeometryUniformComponents = */ 1024,
            /* .MaxGeometryVaryingComponents = */ 64,
            /* .MaxTessControlInputComponents = */ 128,
            /* .MaxTessControlOutputComponents = */ 128,
            /* .MaxTessControlTextureImageUnits = */ 16,
            /* .MaxTessControlUniformComponents = */ 1024,
            /* .MaxTessControlTotalOutputComponents = */ 4096,
            /* .MaxTessEvaluationInputComponents = */ 128,
            /* .MaxTessEvaluationOutputComponents = */ 128,
            /* .MaxTessEvaluationTextureImageUnits = */ 16,
            /* .MaxTessEvaluationUniformComponents = */ 1024,
            /* .MaxTessPatchComponents = */ 120,
            /* .MaxPatchVertices = */ 32,
            /* .MaxTessGenLevel = */ 64,
            /* .MaxViewports = */ 16,
            /* .MaxVertexAtomicCounters = */ 0,
            /* .MaxTessControlAtomicCounters = */ 0,
            /* .MaxTessEvaluationAtomicCounters = */ 0,
            /* .MaxGeometryAtomicCounters = */ 0,
            /* .MaxFragmentAtomicCounters = */ 8,
            /* .MaxCombinedAtomicCounters = */ 8,
            /* .MaxAtomicCounterBindings = */ 1,
            /* .MaxVertexAtomicCounterBuffers = */ 0,
            /* .MaxTessControlAtomicCounterBuffers = */ 0,
            /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
            /* .MaxGeometryAtomicCounterBuffers = */ 0,
            /* .MaxFragmentAtomicCounterBuffers = */ 1,
            /* .MaxCombinedAtomicCounterBuffers = */ 1,
            /* .MaxAtomicCounterBufferSize = */ 16384,
            /* .MaxTransformFeedbackBuffers = */ 4,
            /* .MaxTransformFeedbackInterleavedComponents = */ 64,
            /* .MaxCullDistances = */ 8,
            /* .MaxCombinedClipAndCullDistances = */ 8,
            /* .MaxSamples = */ 4,
            /* .maxMeshOutputVerticesNV = */ 256,
            /* .maxMeshOutputPrimitivesNV = */ 512,
            /* .maxMeshWorkGroupSizeX_NV = */ 32,
            /* .maxMeshWorkGroupSizeY_NV = */ 1,
            /* .maxMeshWorkGroupSizeZ_NV = */ 1,
            /* .maxTaskWorkGroupSizeX_NV = */ 32,
            /* .maxTaskWorkGroupSizeY_NV = */ 1,
            /* .maxTaskWorkGroupSizeZ_NV = */ 1,
            /* .maxMeshViewCountNV = */ 4,
            /* .maxDualSourceDrawBuffersEXT = */ 1,

            /* .limits = */ {
                /* .nonInductiveForLoops = */ 1,
                /* .whileLoops = */ 1,
                /* .doWhileLoops = */ 1,
                /* .generalUniformIndexing = */ 1,
                /* .generalAttributeMatrixVectorIndexing = */ 1,
                /* .generalVaryingIndexing = */ 1,
                /* .generalSamplerIndexing = */ 1,
                /* .generalVariableIndexing = */ 1,
                /* .generalConstantMatrixVectorIndexing = */ 1,
            } };
        /// Parse GLSL sources with glslang API and generate a SPIR-V binary blob.
        /// See glslToHlsl() for parameter meanings.
        ///
        std::vector<uint32_t> glslToSpirv(const std::string& glslFragment,
                                          const std::string& glslFragmentEntryPoint)
        {
            const char* shaderStrings[]{
                glslFragment.data(),
            };

            const int stringLengths[]{
                static_cast<int>(glslFragment.size()),
            };

            glslang::TShader shader(EShLangFragment);
            shader.setStringsWithLengths(shaderStrings, stringLengths, 1);
            shader.setEntryPoint(glslFragmentEntryPoint.c_str());
            shader.setSourceEntryPoint(glslFragmentEntryPoint.c_str());

            // Permits uniforms without explicit layout locations
            shader.setAutoMapLocations(true);
            shader.setAutoMapBindings(true);
            shader.setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);

            constexpr auto messages =
                static_cast<EShMessages>(EShMsgSpvRules | EShMsgKeepUncalled | EShMsgDebugInfo);

            {
                // The MaterialX GLSL generator is hardcoded to output GLSL 4.0
                constexpr int defaultVersion = 450;
                constexpr bool forwardCompatible = false;

                // Don't support includes in the GLSL source.
                glslang::TShader::ForbidIncluder forbidIncluder;

                if (!shader.parse(&defaultTBuiltInResource, defaultVersion, forwardCompatible,
                    messages, forbidIncluder))
                {
                    const char* const log = shader.getInfoLog();
                    throw std::runtime_error(
                        std::string("glslang failed to parse the GLSL fragment:\n") + log);
                }
            }

            glslang::TProgram program;
            program.addShader(&shader);
            if (!program.link(messages))
            {
                const char* const log = program.getInfoLog();
                throw std::runtime_error(std::string("glslang failed to link the GLSL fragment:\n") + log);
            }

            std::vector<uint32_t> spirv;
            {
                glslang::SpvOptions options;
                options.generateDebugInfo = true;   // necessary to preserve symbols
                options.disableOptimizer = true;
                options.optimizeSize = false;

                glslang::GlslangToSpv(*program.getIntermediate(EShLangFragment), spirv, &options);
            }

            return spirv;
        }

        /// Generate HLSL fragment code from a SPIR-V binary blob.
        std::pair<std::string, std::string> spirvToHlsl(std::vector<uint32_t>&& spirv, const std::string& fragmentEntryPoint)
        {
            std::string hlslsource;
            std::string reflection;
            auto crossCompiler = std::make_shared<spirv_cross::CompilerHLSL>(spirv);
            crossCompiler->set_entry_point(fragmentEntryPoint, spv::ExecutionModelFragment);

            crossCompiler->set_resource_binding_flags(spirv_cross::HLSL_BINDING_AUTO_NONE_BIT);

            spirv_cross::CompilerHLSL::Options hlslOptions = crossCompiler->get_hlsl_options();

            hlslOptions.shader_model = 51;

            // Custom modification in our SPIRV-Cross fork to skip register assignment
            //TODO: Fixme
            //hlslOptions.assign_register_slots = true;

            // Another custom modification in our SPIRV-Cross fork. The cross-compiler
            // will generate code for call graphs rooted at the functions specified in
            // this list even if they are not called from the entry point.
            // The default behavior is to traverse from the entry point only.
            // hlslOptions.exported_functions.insert(fragmentName);
            try
            {
                crossCompiler->set_hlsl_options(hlslOptions);
                hlslsource = crossCompiler->compile();

                // Reflect
                spirv_cross::Parser spirv_parser(spirv);
                spirv_parser.parse();

                spirv_cross::CompilerReflection compiler(std::move(spirv_parser.get_parsed_ir()));
                compiler.set_format("json");
                reflection = compiler.compile();
            }
            catch (const std::exception& exc)
            {
                // catch anything thrown within try block that derives from std::exception
                printf("Cross Compiler error %s\n", exc.what());
            }
            return { hlslsource, reflection };
        }

        void initialize()
        {
            glslang::InitializeProcess();
        }

        void finalize()
        {
            glslang::FinalizeProcess();
        }

        std::pair<std::string, std::string> glslToHlsl(const std::string& glslFragment, const std::string& fragmentName)
        {
            std::vector<uint32_t> spirv = Cross::glslToSpirv(glslFragment, fragmentName);
            return spirvToHlsl(std::move(spirv), fragmentName);
        }
    }
}

