###############################################################################
# Copyright (c) ATTX LLC 2024. All Rights Reserved.
#
# This software and associated documentation (the "Software") are the 
# proprietary and confidential information of ATTX, LLC. The Software is 
# furnished under a license agreement between ATTX and the user organization 
# and may be used or copied only in accordance with the terms of the agreement.
# Refer to 'license/attx_license.adoc' for standard license terms.
#
# EXPORT CONTROL NOTICE: THIS SOFTWARE MAY INCLUDE CONTENT CONTROLLED UNDER THE
# INTERNATIONAL TRAFFIC IN ARMS REGULATIONS (ITAR) OR THE EXPORT ADMINISTRATION 
# REGULATIONS (EAR99). No part of the Software may be used, reproduced, or 
# transmitted in any form or by any means, for any purpose, without the express 
# written permission of ATTX, LLC.
###############################################################################
# Set our build flags
##############################################################################
# Set our project name. This flag should be set to the same name as the project
# name in git
set(PROJECT_NAME modelspace-release)

###############################################################################
# Detect/identify our architecture -- this will be used to select our binaries
##############################################################################
execute_process(COMMAND uname "-m" RESULT_VARIABLE rv OUTPUT_VARIABLE out OUTPUT_STRIP_TRAILING_WHITESPACE)
set(SYSTEM_ARCHITECTURE "${out}")

if(SYSTEM_ARCHITECTURE STREQUAL "x86_64")
    set(ARCHITECTURE "x86_64")
elseif(SYSTEM_ARCHITECTURE STREQUAL "aarch64")
    set(ARCHITECTURE "arm64")
else()
    message("ERROR: System architecture ${SYSTEM_ARCHITECTURE} not supported by ModelSpace")
    message("Contact help@attx.tech and we will add your architecture to support")
    message(FATAL_ERROR "Exiting.")
endif()

###############################################################################
# Set our build flags
##############################################################################
# Set a flag for our swig language, if we want one. Default is Python
# Right now the supported list is: Python (default), NONE
option(SWIG_LANGUAGE "What SWIG language should project be compiled into (if any)")
if(NOT SWIG_LANGUAGE)
    set(SWIG_LANGUAGE python)
endif()

# Set a flag for whether to enable and run the modelspace unit tests provided
# by the modelspace development team. The default is to not do so.
# Right now the supported list is: Python (default), NONE
option(ENABLE_MS_TESTS "Flag to indicate whether ModelSpace unit tests should be run (default is false)")

##########################################################################
# Project configuration variables. These variables set the project number
# and analysis script targets for build
##########################################################################

# ATTX ModelSpace version 0.1
cmake_minimum_required(VERSION 3.8)
project(${PROJECT_NAME} VERSION 1.0)
cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)
set(USE_SWIG_DEPENDENCIES TRUE)

# Set basic standards for the project
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-g -O2 -Wall -fPIC -std=c++11")

# Export our project dir for access in C++
add_definitions(-DMODELSPACE_ROOT_DIR="${PROJECT_SOURCE_DIR}/modelspace")
add_definitions(-DCLOCKWERK_ROOT_DIR="${PROJECT_SOURCE_DIR}/modelspace/clockwerk")
add_definitions(-DPROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Export our project dir for access in Python
# TODO: Make this better. Right now cmake is being dumb so we're going to
# do it via bashrc
execute_process(COMMAND touch ${PROJECT_BINARY_DIR}/__init__.py)

##############################################################################
# Set up dependencies
##############################################################################
include_directories(${PROJECT_SOURCE_DIR}/modelspace/cpp/src)
include_directories(${PROJECT_SOURCE_DIR}/modelspace/clockwerk/src)
include_directories(${PROJECT_SOURCE_DIR}/modelspace/cspice/include)

# We have a dependency on Python (and other languages) for CMake. Find python
# here and ensure it is included
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
include_directories(${Python3_INCLUDE_DIRS})

if(NOT DEFINED HIGHFIVE_USE_BOOST)
  set(HIGHFIVE_USE_BOOST Off)
endif()

# Include the subdirectory and use the target HighFive.
add_subdirectory(${PROJECT_SOURCE_DIR}/highfive)

# Set up for our visuals dependency
find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

##############################################################################
# Library Source Code
##############################################################################
# Source code for our FORTRAN libraries for MSIS
set(SRC_DIR ${PROJECT_SOURCE_DIR}/cpp/src)
include_directories(${SRC_DIR})

# Mark the source directory for our models, tasks, monitors, events, systems
# This block builds a library from our source to be linked into test and 
# analysis scripts
# Note we're pulling linked libraries from our system architecture
link_directories(${PROJECT_SOURCE_DIR}/modelspace/lib/${CMAKE_SYSTEM_NAME}/${ARCHITECTURE}/)
file(GLOB_RECURSE CXX_SRC ${SRC_DIR}/*.cpp)
add_library(${PROJECT_NAME} ${CXX_SRC})
target_link_libraries(${PROJECT_NAME} modelspace HighFive OgreBites clockwerk msis gfortran cspice)

##############################################################################
# Script Source Code
##############################################################################
# Mark the scripts directory for our project -- this is where our analysis 
# scripts live
set(SCRIPT_DIR ${PROJECT_SOURCE_DIR}/cpp/scripts)
include_directories(${SCRIPT_DIR})
file(GLOB_RECURSE CXX_SCRIPT ${SCRIPT_DIR}/*/script.cpp)

# Loop through our analysis scripts and output them to /path/to/script
foreach(var ${CXX_SCRIPT})
    file(RELATIVE_PATH path ${PROJECT_SOURCE_DIR} ${var})
    string(REPLACE "script.cpp" "" path_f ${path})
    string(REPLACE "cpp/scripts/" "" exe_name ${path_f})
    string(REPLACE "/" "_" exe_name ${exe_name})
    add_executable(${exe_name}script.o ${var})
    target_link_libraries(${exe_name}script.o ${PROJECT_NAME} modelspace HighFive OgreBites clockwerk msis gfortran cspice)
    set_target_properties(${exe_name}script.o PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${path_f}")
endforeach()

##############################################################################
# Testing Code
##############################################################################
# Set our test directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(TEST_DIR ${PROJECT_SOURCE_DIR}/cpp/test)
set(MS_TEST_DIR ${PROJECT_SOURCE_DIR}/modelspace/cpp/test)
set(CWK_TEST_DIR ${PROJECT_SOURCE_DIR}/modelspace/clockwerk/test)
if(ENABLE_MS_TESTS)
    file(GLOB_RECURSE CXX_TEST ${TEST_DIR}/*.cpp ${MS_TEST_DIR}/*.cpp ${CWK_TEST_DIR}/*.cpp)
else()
    file(GLOB_RECURSE CXX_TEST ${TEST_DIR}/*.cpp)
endif()

# Fetch our googletest support code
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable our testing
enable_testing()

# Build all of our tests. These lines loop through the test directory and 
# build all cpp tests
add_executable(${PROJECT_NAME}_test ${CXX_TEST})
set_target_properties(${PROJECT_NAME}_test PROPERTIES COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
target_link_libraries(${PROJECT_NAME}_test gtest_main modelspace HighFive OgreBites clockwerk msis gfortran gcov cspice)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)

##############################################################################
# SWIG Code
##############################################################################
# Wrap this full section in an if check -- don't use swig if set to NONE
if(NOT SWIG_LANGUAGE STREQUAL "NONE")

# First, we need to auto-generate our swig .i files for our models. Get a 
# handle to all the header files in our directory
file(GLOB_RECURSE ITEM_HEADERS ${SRC_DIR}/*.h)

# Loop through our headers -- note the python script identifies what swig wrappers
# are needed and which are not
# A swig wrapper is defined as "not needed" if a file with an identical name exists
# in the swig/ directory
foreach(tgt ${ITEM_HEADERS})
    message("Generating SWIG wrapper for file ${tgt}")
    execute_process(COMMAND python3 AutogenSwigFiles.py 
                            "--include_path=${tgt}" 
                            "--incl_swig_dir=${PROJECT_SOURCE_DIR}/modelspace/swig"
                            "--out_dir=${PROJECT_BINARY_DIR}/swig_auto"
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/swig 
                    RESULT_VARIABLE rv
                    OUTPUT_VARIABLE out)
    # message("${OUTPUT_VARIABLE}")
    # message("rv='${rv}'")
endforeach()

find_package (SWIG)
include (UseSWIG)

set(AUTO_SWIG_DIR ${PROJECT_BINARY_DIR}/swig_auto)
file(GLOB_RECURSE AUTO_SWIG ${AUTO_SWIG_DIR}/*.i)

foreach(tgt ${AUTO_SWIG})
    set_property(SOURCE ${tgt} PROPERTY CPLUSPLUS ON)
    string(REPLACE ${AUTO_SWIG_DIR}/ "" tgt_name ${tgt})
    string(REPLACE ".i" "" tgt_name ${tgt_name})
    message("Configuring CMake for SWIG target module ${tgt_name}")
    swig_add_library(${tgt_name} LANGUAGE ${SWIG_LANGUAGE} SOURCES ${tgt})
    target_link_libraries(${tgt_name} clockwerk modelspace ${PROJECT_NAME})
endforeach()

set(MAN_SWIG_DIR ${PROJECT_SOURCE_DIR}/swig)
file(GLOB_RECURSE MAN_SWIG ${MAN_SWIG_DIR}/*.i)

foreach(tgt ${MAN_SWIG})
    set_property(SOURCE ${tgt} PROPERTY CPLUSPLUS ON)
    string(REPLACE ${MAN_SWIG_DIR}/ "" tgt_name ${tgt})
    string(REPLACE ".i" "" tgt_name ${tgt_name})
    message("Configuring CMake for SWIG target module ${tgt_name}")
    swig_add_library(${tgt_name} LANGUAGE ${SWIG_LANGUAGE} SOURCES ${tgt})
    target_link_libraries(${tgt_name} modelspace clockwerk ${PROJECT_NAME})
endforeach()

endif()

##############################################################################
# Doxygen documentation
##############################################################################
# ATTX uses Doxygen awesome for documentation to enhance its output -- that project
# is a gitmodule of clockwerk and should be included/inited

# Check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # Set input and output files
    set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/docs/docs_doxygen/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Note the option ALL which allows to build the docs together with the application
    add_custom_target(dox
        COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT} "-Duse_libclang=ON"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

##############################################################################
# AsciiDoc documentation
##############################################################################
# Note: AsciiDoc needs to run after doxygen generation because our asciidocs
# will link to doxygen.
add_custom_target(adoc
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/docs
    COMMAND ${CMAKE_COMMAND} -P ${PROJECT_SOURCE_DIR}/docs/CMakeAsciidoc.cmake ${PROJECT_SOURCE_DIR}
)

##############################################################################
# All docs documentation
##############################################################################
# This command generates all docs -- asciidoc and doxygen

# Call our doxygen and asciidoc together
add_custom_target(docs
    COMMAND make dox        # This makes our modelspace doxygen
    COMMAND make adoc       # This builds all of our ascii docs
    VERBATIM
)