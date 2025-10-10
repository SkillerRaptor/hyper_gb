#-------------------------------------------------------------------------------------------
# Copyright (c) 2025-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------

include(FetchContent)

#-------------------------------------------------------------------------------------------
# Catch2 (v3.8.1)
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        Catch2
        SYSTEM
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG 2b60af89e23d28eefc081bc930831ee9d45ea58b)

set(CATCH_INSTALL_DOCS OFF CACHE INTERNAL "")
set(CATCH_INSTALL_EXTRAS ON CACHE INTERNAL "")
set(CATCH_DEVELOPMENT_BUILD OFF CACHE INTERNAL "")
set(CATCH_ENABLE_REPRODUCIBLE_BUILD ON CACHE INTERNAL "")

FetchContent_MakeAvailable(Catch2)

#-------------------------------------------------------------------------------------------
# json (v3.12.0)
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        json
        SYSTEM
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG 55f93686c01528224f448c19128836e7df245f72)

FetchContent_MakeAvailable(json)