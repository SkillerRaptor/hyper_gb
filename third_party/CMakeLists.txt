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
# cimgui (v1.92.3)
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        cimgui
        SYSTEM
        GIT_REPOSITORY https://github.com/cimgui/cimgui.git
        GIT_TAG 207fca2d361179c349f3c9d1893b8274f4bbfebf)

set(IMGUI_STATIC ON CACHE INTERNAL "")

FetchContent_MakeAvailable(cimgui)

set(SOURCES
        ${CMAKE_SOURCE_DIR}/third_party/cimgui/cimgui.cpp
        ${CMAKE_SOURCE_DIR}/third_party/cimgui/cimgui_impl.cpp
        ${cimgui_SOURCE_DIR}/imgui/imgui.cpp
        ${cimgui_SOURCE_DIR}/imgui/imgui_demo.cpp
        ${cimgui_SOURCE_DIR}/imgui/imgui_draw.cpp
        ${cimgui_SOURCE_DIR}/imgui/imgui_tables.cpp
        ${cimgui_SOURCE_DIR}/imgui/imgui_widgets.cpp
        ${cimgui_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.cpp
        ${cimgui_SOURCE_DIR}/imgui/backends/imgui_impl_sdlrenderer3.cpp)

set(HEADERS
        ${CMAKE_SOURCE_DIR}/third_party/cimgui/cimgui.h
        ${CMAKE_SOURCE_DIR}/third_party/cimgui/cimgui_impl.h
        ${cimgui_SOURCE_DIR}/imgui/imconfig.h
        ${cimgui_SOURCE_DIR}/imgui/imgui.h
        ${cimgui_SOURCE_DIR}/imgui/imgui_internal.h
        ${cimgui_SOURCE_DIR}/imgui/imstb_rectpack.h
        ${cimgui_SOURCE_DIR}/imgui/imstb_textedit.h
        ${cimgui_SOURCE_DIR}/imgui/imstb_truetype.h
        ${cimgui_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.h
        ${cimgui_SOURCE_DIR}/imgui/backends/imgui_impl_sdlrenderer3.h)

add_library(imgui STATIC ${SOURCES} ${HEADERS})
target_include_directories(
        imgui
        SYSTEM
        PUBLIC
        ${CMAKE_SOURCE_DIR}/third_party/cimgui/
        ${cimgui_SOURCE_DIR}/
        ${cimgui_SOURCE_DIR}/imgui/
        ${cimgui_SOURCE_DIR}/imgui/backends/)
target_link_libraries(
        imgui
        PUBLIC
        SDL3::SDL3)
target_compile_definitions(
        imgui
        PUBLIC
        -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS=1
        -DCIMGUI_USE_SDL3
        -DCIMGUI_USE_SDLRENDERER3)

if (WIN32)
    target_compile_definitions(imgui PUBLIC "-DIMGUI_IMPL_API=extern \"C\" __declspec\(dllexport\)")
else ()
    target_compile_definitions(imgui PUBLIC "-DIMGUI_IMPL_API=extern \"C\" ")
endif ()

#-------------------------------------------------------------------------------------------
# json (v3.12.0)
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        json
        SYSTEM
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG 55f93686c01528224f448c19128836e7df245f72)

FetchContent_MakeAvailable(json)

#-------------------------------------------------------------------------------------------
# SDL (v3.2.24)
#-------------------------------------------------------------------------------------------
FetchContent_Declare(
        SDL
        SYSTEM
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG a8589a84226a6202831a3d49ff4edda4acab9acd)

set(SDL_SHARED OFF CACHE INTERNAL "")
set(SDL_STATIC ON CACHE INTERNAL "")
set(SDL_TEST_LIBRARY OFF CACHE INTERNAL "")
set(SDL_TESTS OFF CACHE INTERNAL "")
set(SDL_DISABLE_INSTALL ON CACHE INTERNAL "")
set(SDL_DISABLE_INSTALL_DOCS ON CACHE INTERNAL "")
set(SDL_INSTALL_TESTS OFF CACHE INTERNAL "")

set(SDL_AUDIO OFF CACHE INTERNAL "")
set(SDL_VIDEO ON CACHE INTERNAL "")
set(SDL_GPU OFF CACHE INTERNAL "")
set(SDL_RENDER ON CACHE INTERNAL "")
set(SDL_CAMERA OFF CACHE INTERNAL "")
set(SDL_JOYSTICK OFF CACHE INTERNAL "")
set(SDL_HAPTIC OFF CACHE INTERNAL "")
set(SDL_HIDAPI OFF CACHE INTERNAL "")
set(SDL_POWER OFF CACHE INTERNAL "")
set(SDL_SENSOR OFF CACHE INTERNAL "")
set(SDL_DIALOG ON CACHE INTERNAL "")
set(SDL_TRAY OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(SDL)

set_target_properties(
        SDL3-static
        PROPERTIES
        FOLDER " third_party")
set_target_properties(
        SDL_uclibc
        PROPERTIES
        FOLDER "third_party")