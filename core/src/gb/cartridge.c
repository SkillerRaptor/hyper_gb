/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/cartridge.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "gb/utils/log.h"

struct GbCartridge *gb_cartridge_create(const char *rom)
{
    struct GbCartridge *cartridge = malloc(sizeof(struct GbCartridge));
    cartridge->rom = NULL;

    if (rom != NULL)
    {
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

        cartridge->rom = malloc(file_size);

        const size_t read_size = fread(cartridge->rom, sizeof(uint8_t), file_size, file);
        if (read_size != file_size)
        {
            gb_log(GB_LOG_ERROR, "Failed to read rom file\n");
            fclose(file);
            free(cartridge);
            return NULL;
        }

        fclose(file);
    }

    return cartridge;
}

void gb_cartridge_destroy(struct GbCartridge *cartridge)
{
    free(cartridge->rom);
    free(cartridge);
}

void gb_cartridge_write(struct GbCartridge *cartridge, const uint16_t address, const uint8_t value)
{
    // FIXME: Implement Mappers
    (void) cartridge;
    (void) address;
    (void) value;
}

uint8_t gb_cartridge_read(struct GbCartridge *cartridge, const uint16_t address) { return cartridge->rom[address]; }
