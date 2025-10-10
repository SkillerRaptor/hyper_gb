/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gameboy.h"

#include <stdlib.h>

#include "cartridge.h"
#include "cpu.h"
#include "mmu.h"

struct Gameboy *gameboy_create(const char *rom)
{
    struct Gameboy *gameboy = malloc(sizeof(struct Gameboy));
    gameboy->cartridge = cartridge_create(rom);
    gameboy->mmu = mmu_create(gameboy->cartridge);
    gameboy->cpu = cpu_create(gameboy->mmu);

    return gameboy;
}

void gameboy_destroy(struct Gameboy *gameboy)
{
    mmu_destroy(gameboy->mmu);
    cpu_destroy(gameboy->cpu);
    cartridge_destroy(gameboy->cartridge);

    free(gameboy);
}

void gameboy_run(struct Gameboy *gameboy)
{
    for (;;)
    {
        cpu_tick(gameboy->cpu);
    }
}
