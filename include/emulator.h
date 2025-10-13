/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <SDL3/SDL.h>

struct Gameboy;

struct Emulator
{
    // SDL Objects
    SDL_Window *window;
    SDL_Renderer *renderer;

    // Textures
    SDL_Texture *game_screen_texture;
    SDL_Texture *tile_data_texture;
    SDL_Texture *tile_maps_texture;
    SDL_Texture *oam_texture;

    // GameBoy
    struct Gameboy *gb;

    // Others
    bool should_close;
};

struct Emulator *emulator_create(void);
void emulator_destroy(struct Emulator *);

void emulator_run(struct Emulator *);
