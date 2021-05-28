#
# SPIRV-CROSS Configuration
# 

set(SPIRVCROSS_GIT_REPO  https://github.com/KhronosGroup/SPIRV-Cross  CACHE STRING "spirv-cross Git repo")

ExternalProject_Add(khr_spirv-cross
    PREFIX spirv-cross
    GIT_REPOSITORY  ${SPIRVCROSS_GIT_REPO}
    GIT_TAG 9acb9ec31f5a8ef80ea6b994bb77be787b08d3d1
    GIT_SHALLOW  ON
    GIT_PROGRESS OFF
    BUILD_ALWAYS OFF
    CMAKE_ARGS 
        -DSPIRV_CROSS_ENABLE_TESTS=OFF
        -DCMAKE_DEBUG_POSTFIX=d
    UPDATE_COMMAND "" # Skip updates
)
