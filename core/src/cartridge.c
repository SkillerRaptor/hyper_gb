/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "cartridge.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Cartridge *cartridge_create(const char *rom)
{
    struct Cartridge *cartridge = malloc(sizeof(struct Cartridge));

    FILE *file = fopen(rom, "rb");
    if (file == NULL)
    {
        free(cartridge);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    const usize size = ftell(file);
    fseek(file, 0, SEEK_SET);

    cartridge->rom = malloc(size);
    assert(cartridge->rom != NULL);

    if (fread(cartridge->rom, sizeof(u8), size, file) != size)
    {
        fclose(file);
        free(cartridge);
        return NULL;
    }

    fclose(file);

    return cartridge;
}

void cartridge_destroy(struct Cartridge *cartridge)
{
    free(cartridge->rom);
    free(cartridge);
}
