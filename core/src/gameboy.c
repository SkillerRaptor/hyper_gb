/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gameboy.h"

#include <stdlib.h>

#include "cartridge.h"
#include "cpu.h"
#include "logger.h"
#include "mmu.h"
#include "ppu.h"

struct Gameboy *gameboy_create(const char *rom)
{
    struct Gameboy *gameboy = calloc(1, sizeof(struct Gameboy));
    gameboy->cartridge = rom ? cartridge_create(rom) : NULL;
    gameboy->mmu = mmu_create(gameboy);
    gameboy->cpu = cpu_create(gameboy);
    gameboy->ppu = ppu_create(gameboy);

    if (gameboy->cartridge)
    {
        char *cartridge_title = cartridge_get_title(gameboy->cartridge);
        logger_info("Loaded cartridge '%s'", cartridge_title);
        free(cartridge_title);
    }

    return gameboy;
}

void gameboy_destroy(struct Gameboy *gameboy)
{
    ppu_destroy(gameboy->ppu);
    cpu_destroy(gameboy->cpu);
    mmu_destroy(gameboy->mmu);
    cartridge_destroy(gameboy->cartridge);
    free(gameboy);
}

void gameboy_tick(struct Gameboy *gameboy)
{
    // NOTE: The cycles are given as m-cycles
    const u8 cycles = cpu_tick(gameboy->cpu);
    ppu_tick(gameboy->ppu, cycles);
    // FIXME: Add Timer and Tick it for n cycles
}
