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

struct GbCartridge
{
    uint8_t *rom;
};

struct GbCartridge *gb_cartridge_create(const char *rom);
void gb_cartridge_destroy(struct GbCartridge *);

void gb_cartridge_write(struct GbCartridge *, uint16_t address, uint8_t value);
uint8_t gb_cartridge_read(const struct GbCartridge *, uint16_t address);

#ifdef __cplusplus
}
#endif
