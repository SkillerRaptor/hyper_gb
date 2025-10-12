/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "ppu.h"

#include <stdlib.h>

struct Ppu *ppu_create(struct Gameboy *gb)
{
    struct Ppu *ppu = malloc(sizeof(struct Ppu));
    ppu->gb = gb;
    ppu->vram = calloc(1, sizeof(u8) * 0x2000);
    ppu->lcd_control = 0;
    ppu->lcd_status = 0;
    ppu->scy = 0;
    ppu->scx = 0;
    ppu->ly = 0x90; // FIXME: It is hardcoded to V-Blank
    ppu->lyc = 0;
    ppu->bgp = 0;
    ppu->obp0 = 0;
    ppu->obp1 = 0;
    ppu->wy = 0;
    ppu->wx = 0;

    return ppu;
}

void ppu_destroy(struct Ppu *ppu)
{
    free(ppu->vram);
    free(ppu);
}

void ppu_tick(struct Ppu *ppu, const u8 t_cycles)
{
    // Increase dots by t-cycles
    // If dots is higher than 456, than -456 and increase ly
}
