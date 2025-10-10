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

struct Cartridge;

struct Mmu
{
    struct Cartridge *cartridge;

    // Tests Only
    u8 *test_memory;
};

struct Mmu *mmu_create(struct Cartridge *);
void mmu_destroy(struct Mmu *);

void mmu_write(struct Mmu *, u16 address, u8 value);
u8 mmu_read(struct Mmu *, u16 address);

#ifdef __cplusplus
}
#endif
