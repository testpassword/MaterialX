# Install script for directory: /Users/pugalol/Desktop/matx/MaterialX/source/JsMaterialX

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
   "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXCore")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX" TYPE EXECUTABLE FILES "/Users/pugalol/Desktop/matx/MaterialX/bin/JsMaterialXCore")
  if(EXISTS "$ENV{DESTDIR}/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXCore" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXCore")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/pugalol/Desktop/matx/MaterialX/installed/lib"
      "$ENV{DESTDIR}/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXCore")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -u -r "$ENV{DESTDIR}/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXCore")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXGenShader")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX" TYPE EXECUTABLE FILES "/Users/pugalol/Desktop/matx/MaterialX/bin/JsMaterialXGenShader")
  if(EXISTS "$ENV{DESTDIR}/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXGenShader" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXGenShader")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "/Users/pugalol/Desktop/matx/MaterialX/installed/lib"
      "$ENV{DESTDIR}/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXGenShader")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Library/Developer/CommandLineTools/usr/bin/strip" -u -r "$ENV{DESTDIR}/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXGenShader")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXCore.wasm")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX" TYPE FILE FILES "/Users/pugalol/Desktop/matx/MaterialX/bin/JsMaterialXCore.wasm")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXGenShader.wasm;/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXGenShader.data")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX" TYPE FILE FILES
    "/Users/pugalol/Desktop/matx/MaterialX/bin/JsMaterialXGenShader.wasm"
    "/Users/pugalol/Desktop/matx/MaterialX/bin/JsMaterialXGenShader.data"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/pugalol/Desktop/matx/MaterialX/source/JsMaterialX/JsMaterialXCore-1.38.1.js")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/source/JsMaterialX" TYPE FILE RENAME "JsMaterialXCore-1.38.1.js" FILES "/Users/pugalol/Desktop/matx/MaterialX/bin/JsMaterialXCore.js")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXCore-1.38.1.js")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX" TYPE FILE RENAME "JsMaterialXCore-1.38.1.js" FILES "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXCore.js")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/pugalol/Desktop/matx/MaterialX/source/JsMaterialX/JsMaterialXGenShader-1.38.1.js")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/source/JsMaterialX" TYPE FILE RENAME "JsMaterialXGenShader-1.38.1.js" FILES "/Users/pugalol/Desktop/matx/MaterialX/bin/JsMaterialXGenShader.js")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXGenShader-1.38.1.js")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX" TYPE FILE RENAME "JsMaterialXGenShader-1.38.1.js" FILES "/Users/pugalol/Desktop/matx/MaterialX/installed/JavaScript/MaterialX/JsMaterialXGenShader.js")
endif()

