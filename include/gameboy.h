/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "cartridge.h"

struct Gameboy
{
    struct Cartridge cartridge;
};

void gameboy_init(struct Gameboy *gameboy, const char *rom);
void gameboy_shutdown(struct Gameboy *gameboy);

void gameboy_run(struct Gameboy *gameboy);
