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
    mmu->test_memory = NULL;

    if (mmu->cartridge == NULL)
    {
        mmu->test_memory = malloc(sizeof(u8) * 0x10000);
    }

    return mmu;
}

void mmu_destroy(struct Mmu *mmu)
{
    if (mmu->test_memory)
    {
        free(mmu->test_memory);
    }
    free(mmu);
}

void mmu_write(struct Mmu *mmu, const u16 address, const u8 value)
{
    if (mmu->test_memory)
    {
        mmu->test_memory[address] = value;
        return;
    }

    if (address <= 0x7fff)
    {
        return;
    }
}

u8 mmu_read(struct Mmu *mmu, const u16 address)
{
    if (mmu->test_memory)
    {
        return mmu->test_memory[address];
    }

    if (address <= 0x7fff)
    {
        return mmu->cartridge->rom[address];
    }

    return 0;
}
