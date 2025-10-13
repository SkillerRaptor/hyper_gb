#-------------------------------------------------------------------------------------------
# Copyright (c) 2025-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/gb/cartridge.c
        src/gb/cpu.c
        src/gb/cpu_instructions.c
        src/gb/gameboy.c
        src/gb/logger.c
        src/gb/mmu.c
        src/gb/ppu.c
        src/gb/timer.c)

set(HEADERS
        include/gb/cartridge.h
        include/gb/cpu.h
        include/gb/cpu_instructions.h
        include/gb/gameboy.h
        include/gb/logger.h
        include/gb/mmu.h
        include/gb/ppu.h
        include/gb/prerequisites.h
        include/gb/timer.h
        include/gb/types.h
        include/gb/utils/bits.h)

add_library(gb_core STATIC ${SOURCES} ${HEADERS})
target_link_libraries(gb_core PRIVATE ProjectWarnings)
target_include_directories(gb_core PUBLIC include)

add_library(gb_test_core STATIC ${SOURCES} ${HEADERS})
target_link_libraries(gb_test_core PRIVATE ProjectWarnings)
target_compile_definitions(gb_test_core PRIVATE -DTESTS_ENABLED)
target_include_directories(gb_test_core PUBLIC include)

if (WIN32)
    target_compile_definitions(
            gb_core
            PRIVATE
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
            WIN32_LEAN_AND_MEAN)
    target_compile_definitions(
            gb_test_core
            PRIVATE
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
            WIN32_LEAN_AND_MEAN)
endif ()