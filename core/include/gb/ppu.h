/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct Cpu;
struct Mmu;

enum Color
{
    COLOR_WHITE = 0,
    COLOR_LIGHT_GRAY,
    COLOR_DARK_GRAY,
    COLOR_BLACK,
};

enum PpuMode
{
    PPU_MODE_OAM_SCAN,
    PPU_MODE_DRAWING,
    PPU_MODE_H_BLANK,
    PPU_MODE_V_BLANK,
};

struct Ppu
{
    struct Mmu *mmu;
    struct Cpu *cpu;

    // Memory
    uint8_t *vram;

    // Registers
    uint8_t lcd_control; // 0xff40 - LCD control
    uint8_t lcd_status; // 0xff41 - LCD status
    uint8_t scy; // 0xff42 - Background viewport Y
    uint8_t scx; // 0xff43 - Background viewport X
    uint8_t ly; // 0xff44 - LCD Y coordinate
    uint8_t lyc; // 0xff45 - LY compare
    uint8_t bgp; // 0xff47 - BG palette data
    uint8_t obp0; // 0xff48 - OBJ palette 0 data
    uint8_t obp1; // 0xff49 - OBJ palette 1 data
    uint8_t wy; // 0xff4a - Window Y position
    uint8_t wx; // 0xff4b - Window X position plus 7

    // Others
    uint16_t dots_counter;
    enum PpuMode mode;

    enum Color *screen;
};

struct Ppu *ppu_create();
void ppu_destroy(struct Ppu *);

void ppu_tick(struct Ppu *, uint8_t t_cycles);

#ifdef __cplusplus
}
#endif
