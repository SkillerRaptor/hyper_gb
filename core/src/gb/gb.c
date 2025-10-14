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
    gb->cartridge = cartridge_create(rom);
    gb->mmu = mmu_create();
    gb->cpu = cpu_create();
    gb->ppu = ppu_create();
    gb->timer = timer_create();

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
    timer_destroy(gb->timer);
    ppu_destroy(gb->ppu);
    cpu_destroy(gb->cpu);
    mmu_destroy(gb->mmu);
    cartridge_destroy(gb->cartridge);

    free(gb);
}

void gb_run_frame(struct Gb *gb)
{
    uint32_t cycles_this_frame = 0;
    while (cycles_this_frame < GB_FRAME_CYCLES)
    {
        // NOTE: The cycles are given as m-cycles
        const uint8_t m_cycles = cpu_tick(gb->cpu);
        const uint8_t t_cycles = m_cycles * 4;
        ppu_tick(gb->ppu, t_cycles);
        timer_tick(gb->timer, t_cycles);
        cycles_this_frame += t_cycles;
    }
}
