#-------------------------------------------------------------------------------------------
# Copyright (c) 2025-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/instruction_tests.cpp)

set(HEADERS
)

add_executable(gb_tests ${SOURCES} ${HEADERS})
target_link_libraries(gb_tests PRIVATE ProjectWarnings)
target_include_directories(gb_tests PUBLIC include)
target_link_libraries(
        gb_tests
        PRIVATE
        gb_test_core
        Catch2::Catch2WithMain
        nlohmann_json)
