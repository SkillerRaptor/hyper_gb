/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/gameboy.h"

#include <stdlib.h>

#include "gb/cartridge.h"
#include "gb/cpu.h"
#include "gb/logger.h"
#include "gb/mmu.h"
#include "gb/ppu.h"
#include "gb/timer.h"

struct Gameboy *gameboy_create(const char *rom)
{
    struct Gameboy *gameboy = calloc(1, sizeof(struct Gameboy));
    gameboy->cartridge = rom ? cartridge_create(rom) : NULL;
    gameboy->mmu = mmu_create(gameboy);
    gameboy->cpu = cpu_create(gameboy);
    gameboy->ppu = ppu_create(gameboy);
    gameboy->timer = timer_create(gameboy);

    if (gameboy->cartridge)
    {
        char *cartridge_title = cartridge_get_title(gameboy->cartridge);
        logger_info("Loaded cartridge '%s'", cartridge_title);
        free(cartridge_title);
    }

    return gameboy;
}

void gameboy_destroy(struct Gameboy *gb)
{
    timer_destroy(gb->timer);
    ppu_destroy(gb->ppu);
    cpu_destroy(gb->cpu);
    mmu_destroy(gb->mmu);
    cartridge_destroy(gb->cartridge);
    free(gb);
}

void gameboy_run_frame(struct Gameboy *gb)
{
    uint32_t cycles_this_frame = 0;
    while (cycles_this_frame < GAMEBOY_FRAME_CYCLES)
    {
        // NOTE: The cycles are given as m-cycles
        const uint8_t m_cycles = cpu_tick(gb->cpu);
        const uint8_t t_cycles = m_cycles * 4;
        ppu_tick(gb->ppu, t_cycles);
        timer_tick(gb->timer, t_cycles);
        cycles_this_frame += t_cycles;
    }
}
