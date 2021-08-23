# Install script for directory: /Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/pugalol/Desktop/matx/MaterialX/lib/libMaterialXCore.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libMaterialXCore.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libMaterialXCore.a")
    execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libMaterialXCore.a")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Definition.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Document.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Element.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Exception.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Export.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Geom.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Interface.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Library.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Look.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/LookUtil.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Material.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Node.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Property.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Traversal.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Types.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Unit.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Util.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Value.h;/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore/Variant.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/installed/include/MaterialXCore" TYPE FILE FILES
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Definition.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Document.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Element.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Exception.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Export.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Geom.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Interface.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Library.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Look.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/LookUtil.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Material.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Node.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Property.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Traversal.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Types.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Unit.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Util.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Value.h"
    "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Variant.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/pugalol/Desktop/matx/MaterialX/installed/lib/MaterialXCore.pdb")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/installed/lib" TYPE FILE OPTIONAL FILES "/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/Release/MaterialXCore.pdb")
endif()

