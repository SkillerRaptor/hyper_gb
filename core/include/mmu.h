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

struct Gameboy;

struct Mmu
{
    struct Gameboy *gb;

#if TESTS_ENABLED
    u8 *memory;
#else
    u8 *wram;
    u8 *io;
    u8 *hram;
#endif
};

struct Mmu *mmu_create(struct Gameboy *gb);
void mmu_destroy(struct Mmu *);

void mmu_write(struct Mmu *, u16 address, u8 value);
u8 mmu_read(struct Mmu *, u16 address);

#ifdef __cplusplus
}
#endif
