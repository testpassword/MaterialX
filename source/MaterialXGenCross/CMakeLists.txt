file(GLOB_RECURSE materialx_source "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE materialx_header "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

assign_source_group("Header Files" ${materialx_header})
assign_source_group("Source Files" ${materialx_source})

include(FetchContent)
include(ExternalProject)
#set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/khronos-tools)

add_subdirectory(External/glslang)
add_subdirectory(External/spirv-cross)

#[[
ExternalProject_Add_Step(
  spriv-cross CopyToBin
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/humblelogging/bin ${GLOBAL_OUTPUT_PATH}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${GLOBAL_OUTPUT_PATH}/humblelogging/lib ${GLOBAL_OUTPUT_PATH}
  DEPENDEES install
)]]
ExternalProject_Get_Property(khr_glslang install_dir)
get_filename_component(GLSLANG_DIR ${install_dir}/.. ABSOLUTE)
message (STATUS glsldir ${GLSLANG_DIR})

ExternalProject_Get_Property(khr_spirv-cross install_dir)
set (SPIRVCROSS_DIR ${install_dir}/..)
get_filename_component(SPIRVCROSS_DIR ${install_dir}/.. ABSOLUTE)

add_library(MaterialXGenCross 
            ${materialx_source} 
            ${materialx_header}
            )

add_dependencies(MaterialXGenCross khr_glslang)
add_dependencies(MaterialXGenCross khr_spirv-cross)
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
        $<BUILD_INTERFACE:${GLSLANG_DIR}/lib>
        $<BUILD_INTERFACE:${SPIRVCROSS_DIR}/lib>
)
#[[
target_link_libraries(
    MaterialXGenCross 
    $<BUILD_INTERFACE:${GLSLANG_DIR}/lib/glslang
    $<BUILD_INTERFACE:${GLSLANG_DIR}/lib/OSDependent 
    $<BUILD_INTERFACE:${GLSLANG_DIR}/lib/OGLCompiler 
    $<BUILD_INTERFACE:${GLSLANG_DIR}/lib/HLSL 
    $<BUILD_INTERFACE:${SPIRVCROSS_DIR}/lib/SPIRV
    $<BUILD_INTERFACE:${SPIRVCROSS_DIR}/spirv-cross-core 
    $<BUILD_INTERFACE:${SPIRVCROSS_DIR}/spirv-cross-hlsl
    ${CMAKE_DL_LIBS})
]]
target_link_libraries(
    MaterialXGenCross 
    glslang
    spirv-cross-core
    spirv-cross-hlsl
    ${CMAKE_DL_LIBS})
target_include_directories(MaterialXGenCross
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
        $<INSTALL_INTERFACE:${MATERIALX_INSTALL_INCLUDE_PATH}>
    PRIVATE
        $<BUILD_INTERFACE:${GLSLANG_DIR}/include>
        $<BUILD_INTERFACE:${SPIRVCROSS_DIR}/include>
)

# install
#[[
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
install(DIRECTORY ${EXTERNAL_INSTALL_LOCATION} DESTINATION ".") 
]]
