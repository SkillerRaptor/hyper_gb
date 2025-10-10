/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "types.h"

struct Cartridge
{
    u8 *rom;
};

struct Cartridge *cartridge_create(const char *rom);
void cartridge_destroy(struct Cartridge *);

#ifdef __cplusplus
}
#endif
