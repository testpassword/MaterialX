#
# GLSLANG Configuration
#
include(ExternalProject)
set(GLSLANG_GIT_REPO     https://github.com/KhronosGroup/glslang CACHE STRING "glslang Git repo")
ExternalProject_Add(khr_glslang
    GIT_REPOSITORY   ${GLSLANG_GIT_REPO}
    GIT_TAG 6bdcb4be344d7903bd92fd464e496c3199b91484
    GIT_SHALLOW  ON
    GIT_PROGRESS OFF
    BUILD_ALWAYS OFF
    CMAKE_ARGS
        -DBUILD_EXTERNAL=OFF
        -DENABLE_HLSL=OFF
        -DENABLE_CTEST=OFF
        -DBUILD_TESTING=OFF
        -DENABLE_OPT=OFF
        -DCMAKE_DEBUG_POSTFIX=d
        -DCMAKE_INSTALL_PREFIX=${KHR_TOOLS_DIR}
    UPDATE_COMMAND "" # Skip updates
)
