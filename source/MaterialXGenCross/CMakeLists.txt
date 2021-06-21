file(GLOB_RECURSE materialx_source "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE materialx_header "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

assign_source_group("Header Files" ${materialx_header})
assign_source_group("Source Files" ${materialx_source})

# setup location for khronos tools
set(KHR_TOOLS_DIR ${CMAKE_BINARY_DIR}/khr_tools)
add_subdirectory(External/glslang)
add_subdirectory(External/spirv-cross)

add_library(MaterialXGenCross ${materialx_source} ${materialx_header})

# add glslang and spirv-cross dependencies.
add_dependencies(MaterialXGenCross khr_spirv-cross khr_glslang)
add_definitions(-DMATERIALX_GENCROSS_EXPORTS)

set_target_properties(
    MaterialXGenCross PROPERTIES
    OUTPUT_NAME MaterialXGenCross
    COMPILE_FLAGS "${EXTERNAL_COMPILE_FLAGS}"
    LINK_FLAGS "${EXTERNAL_LINK_FLAGS}"
    VERSION "${MATERIALX_LIBRARY_VERSION}"
    SOVERSION "${MATERIALX_MAJOR_VERSION}")

set(GLSLANG_LIBS
    optimized ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}OGLCompiler${CMAKE_STATIC_LIBRARY_SUFFIX}
    debug     ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}OGLCompiler${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    optimized ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}OSDependent${CMAKE_STATIC_LIBRARY_SUFFIX}
    debug     ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}OSDependent${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    optimized ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}GenericCodeGen${CMAKE_STATIC_LIBRARY_SUFFIX}
    debug     ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}GenericCodeGen${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    optimized ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}MachineIndependent${CMAKE_STATIC_LIBRARY_SUFFIX}
    debug     ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}MachineIndependent${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    optimized ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}glslang${CMAKE_STATIC_LIBRARY_SUFFIX}
    debug     ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}glslang${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    optimized ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}glslang-default-resource-limits${CMAKE_STATIC_LIBRARY_SUFFIX}
    debug     ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}glslang-default-resource-limits${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    optimized ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}SPIRV${CMAKE_STATIC_LIBRARY_SUFFIX}
    debug     ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}SPIRV${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
)

set(SPIRV_CROSS_LIBS
    optimized ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}spirv-cross-core${CMAKE_STATIC_LIBRARY_SUFFIX}
    debug     ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}spirv-cross-core${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    optimized ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}spirv-cross-glsl${CMAKE_STATIC_LIBRARY_SUFFIX}
    debug     ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}spirv-cross-glsl${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    optimized ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}spirv-cross-hlsl${CMAKE_STATIC_LIBRARY_SUFFIX}
    debug     ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}spirv-cross-hlsl${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
    optimized ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}spirv-cross-reflect${CMAKE_STATIC_LIBRARY_SUFFIX}
    debug     ${KHR_TOOLS_DIR}/lib/${CMAKE_STATIC_LIBRARY}spirv-cross-reflect${CMAKE_DEBUG_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
)

target_link_libraries(
    MaterialXGenCross
    MaterialXGenShader
    MaterialXGenGlsl
    MaterialXCore
    ${GLSLANG_LIBS}
    ${SPIRV_CROSS_LIBS}
    ${CMAKE_DL_LIBS})

target_include_directories(MaterialXGenCross
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<INSTALL_INTERFACE:${MATERIALX_INSTALL_INCLUDE_PATH}>
    PRIVATE
        $<BUILD_INTERFACE:${KHR_TOOLS_DIR}/include>
)

# install
install(TARGETS MaterialXGenCross
    EXPORT MaterialX
    ARCHIVE DESTINATION ${MATERIALX_INSTALL_LIB_PATH}
    LIBRARY DESTINATION ${MATERIALX_INSTALL_LIB_PATH}
    RUNTIME DESTINATION bin)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${MATERIALX_INSTALL_INCLUDE_PATH}/MaterialXGenCross/ MESSAGE_NEVER
    FILES_MATCHING PATTERN "*.h*")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/MaterialXGenCross.pdb"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/${MATERIALX_INSTALL_LIB_PATH}/" OPTIONAL)

# install Khronos tools to MaterialX install
install(DIRECTORY ${KHR_TOOLS_DIR} DESTINATION ".")

