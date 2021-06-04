#
# SPIRV-CROSS Configuration
# 
include(ExternalProject)
set(SPIRVCROSS_GIT_REPO  https://github.com/KhronosGroup/SPIRV-Cross  CACHE STRING "spirv-cross Git repo")
ExternalProject_Add(khr_spirv-cross
    GIT_REPOSITORY  ${SPIRVCROSS_GIT_REPO}
    GIT_TAG 2021-01-15
    GIT_SHALLOW  ON
    GIT_PROGRESS OFF
    BUILD_ALWAYS OFF
    CMAKE_ARGS 
        -DSPIRV_CROSS_ENABLE_TESTS=OFF
        -DCMAKE_DEBUG_POSTFIX=d
        -DCMAKE_INSTALL_PREFIX=${KHR_TOOLS_DIR}
    UPDATE_COMMAND "" # Skip updates
)