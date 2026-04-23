# Copyright (C) 2026 duyongquan <quandy2020@126.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

#============================================================================
# Initialize the project
#============================================================================
project(aerozen VERSION 0.1.0)

#============================================================================
# Find aerozen-cmake
#============================================================================
find_package(aerozen-cmake REQUIRED)

#============================================================================
# Configure the project
#============================================================================
set(skip_pybind11_default_value OFF)
option(SKIP_PYBIND11
      "Skip generating Python bindings via pybind11"
      ${skip_pybind11_default_value})

# Python interfaces vars
include(CMakeDependentOption)
cmake_dependent_option(USE_SYSTEM_PATHS_FOR_PYTHON_INSTALLATION
      "Install python modules in standard system paths in the system"
      OFF "NOT SKIP_PYBIND11" OFF)

cmake_dependent_option(USE_DIST_PACKAGES_FOR_PYTHON
      "Use dist-packages instead of site-package to install python modules"
      OFF "NOT SKIP_PYBIND11" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aerozen_configure_project(VERSION_SUFFIX)

#============================================================================
# Set project-specific options
#============================================================================

# aerozen currently has no options that are unique to it
if (UNIX AND NOT APPLE)
  set (EXTRA_TEST_LIB_DEPS stdc++fs)
else()
  set (EXTRA_TEST_LIB_DEPS)
endif()

#============================================================================
# Search for project-specific dependencies
#============================================================================
message(STATUS "\n\n-- ====== Finding Dependencies ======")

#--------------------------------------
# Python interfaces
if (SKIP_PYBIND11)
  message(STATUS "SKIP_PYBIND11 set - disabling python bindings")
  find_package(Python3 COMPONENTS Interpreter)
else()
  find_package(Python3
    COMPONENTS Interpreter
    OPTIONAL_COMPONENTS Development)
endif()

#--------------------------------------
# Find Protobuf
aerozen_find_package(AerozenProtobuf
                REQUIRED
                PRETTY Protobuf)

#--------------------------------------

########################################
# Find Zenoh
option(AEROZEN_ENABLE_ZENOH "Enable aerozen zenoh implementation" ON)

if (AEROZEN_ENABLE_ZENOH)
  aerozen_find_package(zenohc PRIVATE QUIET)
  aerozen_find_package(zenohcxx PRIVATE QUIET)

  if (NOT zenohcxx_FOUND)
    message (STATUS "Looking for zenohcxx - not found")
    set (HAVE_ZENOH OFF CACHE BOOL "HAVE ZENOH" FORCE)
  elseif (NOT zenohc_FOUND)
    message (STATUS "Looking for zenohc - not found")
    set (HAVE_ZENOH OFF CACHE BOOL "HAVE ZENOH" FORCE)
  else ()
    message (STATUS "Looking for zenohcxx and zenohc - found")
    set (HAVE_ZENOH ON CACHE BOOL "HAVE ZENOH" FORCE)
    list(APPEND EXTRA_TEST_LIB_DEPS "zenohcxx::zenohc")
    list(APPEND AEROZEN_IMPLEMENTATION_LIST "zenoh")
  endif ()
endif()

#--------------------------------------
# Find uuid
if (MSVC)
  set(skip_pybind11_default_value ON)
  message (STATUS "UUID: Using Windows RPC UuidCreate function\n")
else()
  aerozen_find_package(UUID REQUIRED)
endif()

#--------------------------------------
# Find aerozen-utils
aerozen_find_package(AerozenUtils VERSION 4 REQUIRED COMPONENTS cli)

#--------------------------------------
# Find ifaddrs
aerozen_find_package(IFADDRS QUIET)
if (IFADDRS_FOUND)
  set (HAVE_IFADDRS ON CACHE BOOL "HAVE IFADDRS" FORCE)
else ()
  set (HAVE_IFADDRS OFF CACHE BOOL "HAVE IFADDRS" FORCE)
endif()

#--------------------------------------
# Find if command is available. This is used to enable tests.
# Note that CLI files are installed regardless of whether the dependency is
# available during build time
find_program(HAVE_AEROZEN_TOOLS aerozen)
set (GZ_TOOLS_VER 2)

#--------------------------------------
# Find SQLite3
aerozen_find_package(SQLite3
  VERSION 3.7.13
  REQUIRED_BY log
  PRIVATE_FOR log
  PRETTY sqlite3)

#============================================================================
# Configure the build
#============================================================================
aerozen_configure_build(QUIT_IF_BUILD_ERRORS
  COMPONENTS log parameters)

#============================================================================
# aerozen command line support
#============================================================================
add_subdirectory(conf)

#============================================================================
# aerozen python bindings
#============================================================================
if (NOT SKIP_PYBIND11)
  if (Python3_Development_FOUND)
    add_subdirectory(python)
  else ()
    message(WARNING "Python development libraries are missing: Python interfaces are disabled.")
  endif()
endif()

#============================================================================
# Create package information
#============================================================================
aerozen_create_packages()

# #============================================================================
# # Configure documentation
# #============================================================================
# configure_file(${CMAKE_SOURCE_DIR}/api.md.in ${CMAKE_BINARY_DIR}/api.md)
# configure_file(${CMAKE_SOURCE_DIR}/tutorials.md.in ${CMAKE_BINARY_DIR}/tutorials.md)

# aerozen_create_docs(
#   API_MAINPAGE_MD "${CMAKE_BINARY_DIR}/api.md"
#   TUTORIALS_MAINPAGE_MD "${CMAKE_BINARY_DIR}/tutorials.md")