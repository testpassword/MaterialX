
//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//
#ifndef MATERIALX_GENCROSS_H
#define MATERIALX_GENCROSS_H

#include <MaterialXGenCross/Export.h>
#include <string>
#include <vector>
#include <utility>

namespace MaterialX
{
    namespace Stage
    {
        /// Identifier for cross compiled pixel stage.
        extern MX_GENCROSS_API const string CROSS_PIXEL;
    }

    namespace Cross
    {
        /// Initialize global state necessary for cross-compilation.
        void initialize();

        /// Finalize global state necessary for cross-compilation.
        void finalize();

        /// Cross-compile GLSL fragment code to HLSL.
        /// @param glslUniforms GLSL definitions for MaterialX uniforms.
        ///     This GLSL code is not included in the GLSL VP2 fragment because
        ///     GLSL uniforms are generated in the final shader by VP2 based on
        ///     respective XML wrapper properties. However we need to make GLSL
        ///     uniforms known to the HLSL cross-compilation toolchain or else GLSL
        ///     parsing would fail on unknown symbols.
        ///     While public MaterialX uniforms are passed as arguments to the
        ///     fragment's root function, private uniforms are not. Also, both
        ///     public and private uniform samplers are often referenced directly
        ///     from arbitrary MaterialX-generated functions.
        /// @param glslFragment The GLSL code included in the fragment.
        /// @return HLSL fragment code to be included in the fragment.
        

        std::pair<std::string, std::string> glslToHlsl(const std::string& glslFragment, const std::string& fragmentName);

        std::vector<uint32_t> glslToSpirv(const std::string& glslFragment, const std::string& glslFragmentEntryPoint);

        std::pair<std::string, std::string> spirvToHlsl(std::vector<uint32_t>&& spirv, const std::string& fragmentEntryPoint);

    }
}

#endif //MATERIALX_GENCROSS_H
