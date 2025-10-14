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

#include "gb/utils/log.h"

struct Cartridge *cartridge_create(const char *rom)
{
    struct Cartridge *cartridge = malloc(sizeof(struct Cartridge));
    cartridge->data = NULL;

    FILE *file = fopen(rom, "rb");
    if (file == NULL)
    {
        gb_log(GB_LOG_ERROR, "Failed to open rom file\n");
        free(cartridge);
        return NULL;
    }

    fseek(file, 0, SEEK_END);

    const size_t file_size = ftell(file);
    if (file_size == 0)
    {
        gb_log(GB_LOG_ERROR, "Failed to get size of rom file\n");
        fclose(file);
        free(cartridge);
        return NULL;
    }

    fseek(file, 0, SEEK_SET);

    cartridge->data = malloc(file_size);

    const size_t read_size = fread(cartridge->data, sizeof(uint8_t), file_size, file);
    if (read_size != file_size)
    {
        gb_log(GB_LOG_ERROR, "Failed to read rom file\n");
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
