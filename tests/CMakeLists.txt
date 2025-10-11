#-------------------------------------------------------------------------------------------
# Copyright (c) 2025-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/instruction_tests.cpp)

set(HEADERS
)

add_executable(tests ${SOURCES} ${HEADERS})
target_link_libraries(tests PRIVATE ProjectWarnings)
target_include_directories(tests PUBLIC include)
target_link_libraries(
        tests
        PRIVATE
        test_core
        Catch2::Catch2WithMain
        nlohmann_json)
