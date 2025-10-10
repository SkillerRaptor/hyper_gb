/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "types.h"

struct Cartridge
{
    usize rom_size;
    u8 *rom;
};

struct Cartridge *cartridge_create(const char *rom);
void cartridge_destroy(struct Cartridge *);
