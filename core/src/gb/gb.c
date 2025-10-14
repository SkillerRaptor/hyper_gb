/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/gb.h"

#include <stdlib.h>

#include "gb/cartridge.h"
#include "gb/cpu.h"
#include "gb/definitions.h"
#include "gb/mmu.h"
#include "gb/ppu.h"
#include "gb/timer.h"

struct Gb *gb_create(const char *rom)
{
    struct Gb *gb = malloc(sizeof(struct Gb));
    gb->cartridge = gb_cartridge_create(rom);
    gb->mmu = gb_mmu_create();
    gb->cpu = gb_cpu_create();
    gb->ppu = gb_ppu_create();
    gb->timer = gb_timer_create();

    gb->mmu->cartridge = gb->cartridge;
    gb->mmu->cpu = gb->cpu;
    gb->mmu->ppu = gb->ppu;
    gb->mmu->timer = gb->timer;

    gb->cpu->mmu = gb->mmu;

    gb->ppu->mmu = gb->mmu;
    gb->ppu->cpu = gb->cpu;

    gb->timer->cpu = gb->cpu;

    return gb;
}

void gb_destroy(struct Gb *gb)
{
    gb_timer_destroy(gb->timer);
    gb_ppu_destroy(gb->ppu);
    gb_cpu_destroy(gb->cpu);
    gb_mmu_destroy(gb->mmu);
    gb_cartridge_destroy(gb->cartridge);

    free(gb);
}

void gb_run_frame(struct Gb *gb)
{
    uint32_t cycles_this_frame = 0;
    while (cycles_this_frame < GB_FRAME_CYCLES)
    {
        // NOTE: The cycles are given as m-cycles
        const uint8_t m_cycles = gb_cpu_tick(gb->cpu);
        const uint8_t t_cycles = m_cycles * 4;
        gb_ppu_tick(gb->ppu, t_cycles);
        gb_timer_tick(gb->timer, t_cycles);
        cycles_this_frame += t_cycles;
    }
}
