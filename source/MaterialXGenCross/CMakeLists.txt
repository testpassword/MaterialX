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

target_link_directories(MaterialXGenCross
    PRIVATE
        $<BUILD_INTERFACE:${KHR_TOOLS_DIR}/lib>
)

set(GLSLANG_LIBS
                optimized OGLCompiler 
                debug     OGLCompilerd  
                optimized OSDependent 
                debug     OSDependentd 
                optimized GenericCodeGen
                debug     GenericCodeGend
                optimized MachineIndependent
                debug     MachineIndependentd
                optimized glslang
                debug     glslangd
                optimized SPIRV
                debug     SPIRVd
                )

set(SPIRV_CROSS_LIBS 
                optimized spirv-cross-core 
                debug     spirv-cross-cored
                optimized spirv-cross-glsl 
                debug     spirv-cross-glsld
                optimized spirv-cross-hlsl
                debug     spirv-cross-hlsld
                )

target_link_libraries(
    MaterialXGenCross 
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

