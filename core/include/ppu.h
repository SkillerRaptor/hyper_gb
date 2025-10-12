/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define VSYNC_FRAME_RATE 59.73

struct Gameboy;

struct Ppu
{
    struct Gameboy *gb;

    u8 *vram;

    // Registers
    u8 lcd_control; // 0xff40 - LCD control
    u8 lcd_status; // 0xff41 - LCD status
    u8 scy; // 0xff42 - Background viewport Y
    u8 scx; // 0xff43 - Background viewport X
    u8 ly; // 0xff44 - LCD Y coordinate
    u8 lyc; // 0xff45 - LY compare
    u8 bgp; // 0xff47 - BG palette data
    u8 obp0; // 0xff48 - OBJ palette 0 data
    u8 obp1; // 0xff49 - OBJ palette 1 data
    u8 wy; // 0xff4a - Window Y position
    u8 wx; // 0xff4b - Window X position plus 7
};

struct Ppu *ppu_create(struct Gameboy *);
void ppu_destroy(struct Ppu *);

void ppu_tick(struct Ppu *, u8 t_cycles);

#ifdef __cplusplus
}
#endif
