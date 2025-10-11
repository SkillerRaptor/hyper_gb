/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "ppu.h"

#include <stdlib.h>

struct Ppu *ppu_create()
{
    struct Ppu *ppu = malloc(sizeof(struct Ppu));
    ppu->lcd_control = 0;
    ppu->lcd_status = 0;
    ppu->scy = 0;
    ppu->scx = 0;
    ppu->ly = 0x94; // FIXME: It is hardcoded to V-Blank
    ppu->lyc = 0;
    ppu->bgp = 0;
    ppu->obp0 = 0;
    ppu->obp1 = 0;
    ppu->wy = 0;
    ppu->wx = 0;
    ppu->vram = malloc(sizeof(u8) * 0x2000);

    return ppu;
}

void ppu_destroy(struct Ppu *ppu)
{
    free(ppu->vram);
    free(ppu);
}
