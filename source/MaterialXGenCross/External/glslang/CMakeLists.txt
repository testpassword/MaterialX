#
# GLSLANG Configuration
#
include(ExternalProject)
set_property(DIRECTORY PROPERTY EP_BASE ${KHR_TOOLS_DIR})
set(GLSLANG_GIT_REPO     https://github.com/KhronosGroup/glslang CACHE STRING "glslang Git repo")

# ENABLE_GLSLANG_BINARIES for glsllang resource limits

ExternalProject_Add(khr_glslang
    GIT_REPOSITORY   ${GLSLANG_GIT_REPO}
    GIT_TAG 11.4.0
    GIT_SHALLOW  ON
    GIT_PROGRESS OFF
    CMAKE_ARGS
        -DENABLE_SPVREMAPPER=OFF
        -DENABLE_GLSLANG_BINARIES=ON
        -DENABLE_HLSL=OFF
        -DENABLE_OPT=OFF
        -DENABLE_CTEST=OFF
        -DBUILD_TESTING=OFF
        -DBUILD_EXTERNAL=OFF
        -DCMAKE_DEBUG_POSTFIX=${CMAKE_DEBUG_POSTFIX}
        -DCMAKE_INSTALL_PREFIX=${KHR_TOOLS_DIR}
    UPDATE_COMMAND "" # Skip updates
)
