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
    gameboy->cartridge = cartridge_create(rom);
    gameboy->mmu = mmu_create(gameboy);
    gameboy->cpu = cpu_create(gameboy);
    gameboy->ppu = ppu_create(gameboy);

    char *cartridge_title = cartridge_get_title(gameboy->cartridge);
    logger_info("Loaded cartridge '%s'", cartridge_title);
    free(cartridge_title);

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

void gameboy_run(struct Gameboy *gameboy)
{
    for (;;)
    {
        cpu_tick(gameboy->cpu);
    }
}
