############################################################################
# Copyright (c) 2019, Martin Renou                                         #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################

cmake_minimum_required(VERSION 3.4.3)
project(pybind11_json)

set(PYBIND11_JSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 2)
set(${PROJECT_NAME}_VERSION_PATCH 7)
set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})

################
# Dependencies #
################

set(pybind11_REQUIRED_VERSION 2.2.4)
set(nlohmann_json_REQUIRED_VERSION 3.2.0)

if (NOT TARGET pybind11)
find_package(pybind11 ${pybind11_REQUIRED_VERSION} REQUIRED)
endif()
if (NOT TARGET nlohmann_json)
find_package(nlohmann_json ${nlohmann_json_REQUIRED_VERSION} REQUIRED)
endif()

##########
# Target #
##########

include(GNUInstallDirs)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib; ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

set(PYBIND11_JSON_HEADERS
    include/pybind11_json/pybind11_json.hpp
)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
                           INTERFACE
                           ${PYTHON_INCLUDE_DIRS}
                           $<BUILD_INTERFACE:${PYBIND11_JSON_INCLUDE_DIR}>
                           $<INSTALL_INTERFACE:include>)

# The PUBLIC_HEADER property can be set for an INTERFACE library
# as of cmake 3.15
if(NOT (CMAKE_VERSION VERSION_LESS 3.15))  # CMake >= 3.15
set_target_properties(${PROJECT_NAME}
        PROPERTIES
        PUBLIC_HEADER "${PYBIND11_JSON_HEADERS}")
endif()

#########
# Tests #
#########

OPTION(BUILD_TESTS "pybind11_json test suite" OFF)
OPTION(DOWNLOAD_GTEST "build gtest from downloaded sources" OFF)

if(DOWNLOAD_GTEST OR GTEST_SRC_DIR)
    set(BUILD_TESTS ON)
endif()

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

################
# Installation #
################

include(CMakePackageConfigHelpers)

set(PYBIND11_JSON_CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}" CACHE STRING "install path for pybind11_jsonConfig.cmake")

# Install pybind11_json
if(CMAKE_VERSION VERSION_LESS 3.15)  # CMake < 3.15
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets)
else()
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
endif()

# Makes the project importable from the build directory
export(EXPORT ${PROJECT_NAME}-targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

if(CMAKE_VERSION VERSION_LESS 3.15)  # CMake < 3.15
install(FILES ${PYBIND11_JSON_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
endif()

# Configure 'pybind11_jsonConfig.cmake' for a build tree
set(PYBIND11_JSON_CONFIG_CODE "####### Expanded from \@PYBIND11_JSON_CONFIG_CODE\@ #######\n")
set(PYBIND11_JSON_CONFIG_CODE "${PYBIND11_JSON_CONFIG_CODE}set(CMAKE_MODULE_PATH \"${CMAKE_CURRENT_SOURCE_DIR}/cmake;\${CMAKE_MODULE_PATH}\")\n")
set(PYBIND11_JSON_CONFIG_CODE "${PYBIND11_JSON_CONFIG_CODE}##################################################")
configure_package_config_file(${PROJECT_NAME}Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                              INSTALL_DESTINATION ${PROJECT_BINARY_DIR})

# Configure 'pybind11_jsonConfig.cmake' for an install tree
set(PYBIND11_JSON_CONFIG_CODE "")
configure_package_config_file(${PROJECT_NAME}Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}Config.cmake"
                              INSTALL_DESTINATION ${PYBIND11_JSON_CMAKECONFIG_INSTALL_DIR})

# ARCH_INDEPENDENT feature was introduced in cmake 3.14
if (${CMAKE_VERSION} VERSION_LESS "3.14.0")
  write_basic_package_version_file(
      ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
      VERSION ${${PROJECT_NAME}_VERSION}
      COMPATIBILITY AnyNewerVersion)
else ()
  write_basic_package_version_file(
      ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
      VERSION ${${PROJECT_NAME}_VERSION}
      COMPATIBILITY AnyNewerVersion
      ARCH_INDEPENDENT)
endif ()

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${PYBIND11_JSON_CMAKECONFIG_INSTALL_DIR})
install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${PYBIND11_JSON_CMAKECONFIG_INSTALL_DIR})
