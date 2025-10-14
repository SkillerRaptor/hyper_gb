/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct Cartridge
{
    uint8_t *rom;
};

struct Cartridge *cartridge_create(const char *rom);
void cartridge_destroy(struct Cartridge *);

void cartridge_write(struct Cartridge *, uint16_t address, uint8_t value);
uint8_t cartridge_read(const struct Cartridge *, uint16_t address);

#ifdef __cplusplus
}
#endif
