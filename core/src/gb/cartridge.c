/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/cartridge.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gb/logger.h"

struct Cartridge *cartridge_create(const char *rom)
{
    struct Cartridge *cartridge = malloc(sizeof(struct Cartridge));
    cartridge->data = NULL;

    FILE *file = fopen(rom, "rb");
    if (file == NULL)
    {
        logger_err("Failed to open rom file");
        free(cartridge);
        return NULL;
    }

    fseek(file, 0, SEEK_END);

    const usize file_size = ftell(file);
    if (file_size == 0)
    {
        logger_err("Failed to get size of rom file");
        fclose(file);
        free(cartridge);
        return NULL;
    }

    fseek(file, 0, SEEK_SET);

    cartridge->data = malloc(file_size);

    const usize read_size = fread(cartridge->data, sizeof(u8), file_size, file);
    if (read_size != file_size)
    {
        logger_err("Failed to read rom file");
        fclose(file);
        free(cartridge);
        return NULL;
    }

    fclose(file);

    return cartridge;
}

void cartridge_destroy(struct Cartridge *cartridge)
{
    free(cartridge->data);
    free(cartridge);
}

char *cartridge_get_title(const struct Cartridge *cartridge)
{
    char *title = malloc(sizeof(char) * 0x10);
    memcpy(title, &cartridge->data[0x0134], 0x10);
    return title;
}
