#-------------------------------------------------------------------------------------------
# Copyright (c) 2025-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------

#-------------------------------------------------------------------------------------------
# CMake Info
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1...4.1)
project(hyper_gb LANGUAGES C CXX)

#-------------------------------------------------------------------------------------------
# Standard Settings
#-------------------------------------------------------------------------------------------
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/Doxygen.cmake)

option(ENABLE_DOXYGEN "Enabling doxygen generation" OFF)
if (ENABLE_DOXYGEN)
    enable_doxygen()
endif ()

#-------------------------------------------------------------------------------------------
# Project Libraries
#-------------------------------------------------------------------------------------------
add_library(ProjectWarnings INTERFACE)

#-------------------------------------------------------------------------------------------
# Warnings
#-------------------------------------------------------------------------------------------
include(cmake/CompilerWarnings.cmake)
enable_warnings(ProjectWarnings)

#-------------------------------------------------------------------------------------------
# Source
#-------------------------------------------------------------------------------------------
add_subdirectory(third_party)
add_subdirectory(core)
add_subdirectory(tests)

set(SOURCES
        src/emulator.c
        src/main.c)

set(HEADERS
        include/emulator.h)

add_executable(gb_emu ${SOURCES} ${HEADERS})
target_link_libraries(gb_emu PRIVATE ProjectWarnings)
target_include_directories(gb_emu PRIVATE include)
target_link_libraries(
        gb_emu
        PRIVATE
        gb_core
        imgui
        SDL3::SDL3-static)
