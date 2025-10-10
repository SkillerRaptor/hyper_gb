/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "mmu.h"

#include <stdlib.h>

#include "cartridge.h"

struct Mmu *mmu_create(struct Cartridge *cartridge)
{
    struct Mmu *mmu = malloc(sizeof(struct Mmu));
    mmu->cartridge = cartridge;

    return mmu;
}

void mmu_destroy(struct Mmu *mmu) { free(mmu); }

void mmu_write(struct Mmu *mmu, const u16 address, const u8 value)
{
    if (address <= 0x7fff)
    {
        return;
    }
}

u8 mmu_read(struct Mmu *mmu, const u16 address)
{
    if (address <= 0x7fff)
    {
        return mmu->cartridge->rom[address];
    }

    return 0;
}
