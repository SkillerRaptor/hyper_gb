/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

struct Cartridge
{
    uint8_t *rom;
    size_t rom_size;
};

void cartridge_init(struct Cartridge *cartridge, const char *rom);
void cartridge_shutdown(struct Cartridge *cartridge);
