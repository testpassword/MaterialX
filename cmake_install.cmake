# Install script for directory: /Users/pugalol/Desktop/matx/MaterialX

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
   "/Users/pugalol/Desktop/matx/MaterialX/installed/CHANGELOG.md;/Users/pugalol/Desktop/matx/MaterialX/installed/README.md")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/Users/pugalol/Desktop/matx/MaterialX/installed" TYPE FILE FILES
    "/Users/pugalol/Desktop/matx/MaterialX/CHANGELOG.md"
    "/Users/pugalol/Desktop/matx/MaterialX/README.md"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/MaterialXConfig.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/MaterialXConfig.cmake"
         "/Users/pugalol/Desktop/matx/MaterialX/CMakeFiles/Export/cmake/MaterialXConfig.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/MaterialXConfig-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/MaterialXConfig.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "/Users/pugalol/Desktop/matx/MaterialX/CMakeFiles/Export/cmake/MaterialXConfig.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "/Users/pugalol/Desktop/matx/MaterialX/CMakeFiles/Export/cmake/MaterialXConfig-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "/Users/pugalol/Desktop/matx/MaterialX/cmake/MaterialXConfigVersion.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXCore/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXFormat/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/libraries/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXGenShader/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXGenGlsl/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXGenOsl/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXGenMdl/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXGenOgsXml/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXGenOgsFx/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXGenEssl/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXGenArnold/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXRender/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXRenderOsl/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXRenderHw/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXRenderGlsl/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/resources/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXContrib/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXView/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/MaterialXTest/cmake_install.cmake")
  include("/Users/pugalol/Desktop/matx/MaterialX/source/JsMaterialX/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/pugalol/Desktop/matx/MaterialX/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
