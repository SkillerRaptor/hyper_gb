/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "cartridge.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void cartridge_init(struct Cartridge *cartridge, const char *rom)
{
    assert(cartridge != NULL);
    assert(rom != NULL);

    FILE *file = fopen(rom, "rb");
    assert(file != NULL);

    fseek(file, 0, SEEK_END);
    cartridge->rom_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    cartridge->rom = malloc(cartridge->rom_size);
    assert(cartridge->rom != NULL);

    if (fread(cartridge->rom, sizeof(uint8_t), cartridge->rom_size, file) != cartridge->rom_size)
    {
        // TODO: Handle error
        return;
    }

    fclose(file);
}

void cartridge_shutdown(struct Cartridge *cartridge)
{
    assert(cartridge != NULL);

    free(cartridge->rom);
}
