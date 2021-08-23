# Install script for directory: /Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/pugalol/Desktop/matx/MaterialX/installed")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Library/Developer/CommandLineTools/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/pugalol/Desktop/matx/MaterialX/bin/libraries/stdlib/reference/osl/")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/bin/libraries/stdlib/reference/osl" TYPE DIRECTORY FILES "")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/pugalol/Desktop/matx/MaterialX/bin/libraries/stdlib/reference/osl/")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/bin/libraries/stdlib/reference/osl" TYPE DIRECTORY FILES "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/../../libraries/stdlib/osl/" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/pugalol/Desktop/matx/MaterialX/bin//mdl/")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/bin//mdl" TYPE DIRECTORY FILES "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/../../source/MaterialXGenMdl/mdl/")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXCore/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXFormat/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXGenShader/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXGenGlsl/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXGenOsl/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXGenMdl/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXGenOgsXml/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXGenOgsFx/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXGenArnold/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXRender/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXRenderGlsl/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXRenderOsl/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXRenderOgsFx/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/MaterialXRenderArnold/cmake_install.cmake")

endif()

