/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/ppu.h"

#include <stdlib.h>

#include "gb/prerequisites.h"
#include "gb/utils/logger.h"

#define PPU_MODE_OAM_SCAN_DOTS 80
#define PPU_MODE_DRAWING_DOTS 289
#define PPU_MODE_H_BLANK_DOTS 204
#define PPU_MODE_V_BLANK_DOTS 4560

struct Ppu *ppu_create(struct Gameboy *gb)
{
    struct Ppu *ppu = malloc(sizeof(struct Ppu));
    ppu->gb = gb;
    ppu->vram = calloc(1, sizeof(uint8_t) * 0x2000);
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
    ppu->dots_counter = 0;
    ppu->mode = PPU_MODE_OAM_SCAN;

    return ppu;
}

void ppu_destroy(struct Ppu *ppu)
{
    free(ppu->vram);
    free(ppu);
}

void ppu_tick(struct Ppu *ppu, const uint8_t t_cycles)
{
    ppu->dots_counter += t_cycles;

    switch (ppu->mode)
    {
    case PPU_MODE_OAM_SCAN:
        if (ppu->dots_counter >= PPU_MODE_OAM_SCAN_DOTS)
        {
            ppu->dots_counter -= PPU_MODE_OAM_SCAN_DOTS;
            ppu->mode = PPU_MODE_DRAWING;
        }
        break;
    case PPU_MODE_DRAWING:
        if (ppu->dots_counter >= PPU_MODE_DRAWING_DOTS)
        {
            ppu->dots_counter -= PPU_MODE_DRAWING_DOTS;
            ppu->mode = PPU_MODE_H_BLANK;
        }
        break;
    case PPU_MODE_H_BLANK:
        if (ppu->dots_counter >= PPU_MODE_H_BLANK_DOTS)
        {
            ppu->dots_counter -= PPU_MODE_H_BLANK_DOTS;

            ppu->ly += 1;

            if (ppu->ly >= 0x90)
            {
                ppu->mode = PPU_MODE_V_BLANK;
            }
            else
            {
                ppu->mode = PPU_MODE_OAM_SCAN;
            }
        }
        break;
    case PPU_MODE_V_BLANK:
        if (ppu->dots_counter >= PPU_MODE_V_BLANK_DOTS)
        {
            ppu->dots_counter -= PPU_MODE_V_BLANK_DOTS;

            ppu->ly += 1;

            if (ppu->ly >= 0x9a)
            {
                ppu->mode = PPU_MODE_OAM_SCAN;
            }
        }
        break;
    default: break;
    }
}
