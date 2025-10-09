/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gameboy.h"

#include <assert.h>

void gameboy_init(struct Gameboy *gameboy, const char *rom)
{
    assert(gameboy != NULL);
    assert(rom != NULL);

    cartridge_init(&gameboy->cartridge, rom);
}

void gameboy_shutdown(struct Gameboy *gameboy)
{
    assert(gameboy != NULL);

    cartridge_shutdown(&gameboy->cartridge);
}

void gameboy_run(struct Gameboy *gameboy) { assert(gameboy != NULL); }
