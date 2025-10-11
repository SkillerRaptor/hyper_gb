#-------------------------------------------------------------------------------------------
# Copyright (c) 2025-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/cartridge.c
        src/cpu.c
        src/cpu_instructions.c
        src/gameboy.c
        src/logger.c
        src/mmu.c
        src/ppu.c)

set(HEADERS
        include/cartridge.h
        include/cpu.h
        include/cpu_instructions.h
        include/gameboy.h
        include/logger.h
        include/mmu.h
        include/ppu.h
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

add_library(hyper_gb_core STATIC ${SOURCES} ${HEADERS})
target_link_libraries(hyper_gb_core PRIVATE ProjectOptions ProjectWarnings)
target_include_directories(hyper_gb_core PUBLIC include)

if (WIN32)
    target_compile_definitions(
            hyper_gb_core
            PRIVATE
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
            WIN32_LEAN_AND_MEAN)
endif ()