/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "types.h"

struct Cartridge;

struct Mmu
{
    struct Cartridge *cartridge;
};

struct Mmu *mmu_create(struct Cartridge *);
void mmu_destroy(struct Mmu *);

void mmu_write(struct Mmu *, u16 address, u8 value);
u8 mmu_read(struct Mmu *, u16 address);
