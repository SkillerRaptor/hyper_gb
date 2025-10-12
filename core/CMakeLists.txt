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
        src/ppu.c
        src/timer.c)

set(HEADERS
        include/cartridge.h
        include/cpu.h
        include/cpu_instructions.h
        include/gameboy.h
        include/logger.h
        include/mmu.h
        include/ppu.h
        include/prerequisites.h
        include/timer.h
        include/types.h)

add_library(core STATIC ${SOURCES} ${HEADERS})
target_link_libraries(core PRIVATE ProjectWarnings)
target_include_directories(core PUBLIC include)

add_library(test_core STATIC ${SOURCES} ${HEADERS})
target_link_libraries(test_core PRIVATE ProjectWarnings)
target_compile_definitions(test_core PRIVATE -DTESTS_ENABLED)
target_include_directories(test_core PUBLIC include)

if (WIN32)
    target_compile_definitions(
            core
            PRIVATE
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
            WIN32_LEAN_AND_MEAN)
    target_compile_definitions(
            test_core
            PRIVATE
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
            WIN32_LEAN_AND_MEAN)
endif ()