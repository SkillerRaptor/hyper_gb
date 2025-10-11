/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct Cartridge
{
    u8 *data;
};

struct Cartridge *cartridge_create(const char *rom);
void cartridge_destroy(struct Cartridge *);

char *cartridge_get_title(const struct Cartridge *);

#ifdef __cplusplus
}
#endif
