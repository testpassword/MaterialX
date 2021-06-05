#
# SPIRV-CROSS Configuration
#
include(ExternalProject)
set_property(DIRECTORY PROPERTY EP_BASE ${KHR_TOOLS_DIR})
set(SPIRVCROSS_GIT_REPO  https://github.com/KhronosGroup/SPIRV-Cross  CACHE STRING "spirv-cross Git repo")
ExternalProject_Add(khr_spirv-cross
    GIT_REPOSITORY  ${SPIRVCROSS_GIT_REPO}
    GIT_TAG 2021-01-15
    GIT_SHALLOW  ON
    GIT_PROGRESS OFF
    BUILD_ALWAYS OFF
    CMAKE_ARGS
        -DSPIRV_CROSS_ENABLE_TESTS=OFF
        -DSPIRV_CROSS_FORCE_PIC=ON
        -DCMAKE_DEBUG_POSTFIX=${DCMAKE_DEBUG_POSTFIX}
        -DCMAKE_INSTALL_PREFIX=${KHR_TOOLS_DIR}
    UPDATE_COMMAND "" # Skip updates
)
