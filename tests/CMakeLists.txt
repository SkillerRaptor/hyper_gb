#-------------------------------------------------------------------------------------------
# Copyright (c) 2025-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------
set(SOURCES
        src/instruction_tests.cpp)

set(HEADERS
)

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

add_executable(hyper_gb_tests ${SOURCES} ${HEADERS})
target_link_libraries(hyper_gb_tests PRIVATE ProjectOptions ProjectWarnings hyper_gb_core Catch2::Catch2WithMain nlohmann_json)
target_include_directories(hyper_gb_tests PUBLIC include)