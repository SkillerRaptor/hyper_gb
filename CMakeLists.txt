#-------------------------------------------------------------------------------------------
# Copyright (c) 2025-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------

#-------------------------------------------------------------------------------------------
# CMake Info
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1...4.1)
project(hyper_gb LANGUAGES C)

#-------------------------------------------------------------------------------------------
# Standard Settings
#-------------------------------------------------------------------------------------------
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
add_library(ProjectOptions INTERFACE)
target_compile_features(ProjectOptions INTERFACE c_std_17)

add_library(ProjectWarnings INTERFACE)

#-------------------------------------------------------------------------------------------
# Warnings
#-------------------------------------------------------------------------------------------
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(cmake/CompilerWarnings.cmake)
enable_warnings(ProjectWarnings)

#-------------------------------------------------------------------------------------------
# Source
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/cartridge.c
        src/cpu.c
        src/gameboy.c
        src/main.c
        src/mmu.c)

set(HEADERS
        include/cartridge.h
        include/cpu.h
        include/gameboy.h
        include/mmu.h
        include/prerequisites.h
        include/types.h)

foreach (item IN ITEMS ${SOURCES})
    get_filename_component(src_path "${item}" PATH)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" group_path "${src_path}")
    string(REPLACE ".." "\\" group_path "${group_path}")
    source_group("${group_path}" FILES "${item}")
endforeach ()

foreach (item IN ITEMS ${HEADERS})
    get_filename_component(src_path "${item}" PATH)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" group_path "${src_path}")
    string(REPLACE ".." "\\" group_path "${group_path}")
    source_group("${group_path}" FILES "${item}")
endforeach ()

add_executable(hyper_gb ${SOURCES} ${HEADERS})
target_link_libraries(hyper_gb PRIVATE ProjectOptions ProjectWarnings)
target_include_directories(hyper_gb PRIVATE include)

if (WIN32)
    target_compile_definitions(
            hyper_gb
            PRIVATE
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
            WIN32_LEAN_AND_MEAN)
endif ()