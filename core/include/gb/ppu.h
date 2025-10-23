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

struct GbCpu;
struct GbMmu;

enum GbColor
{
    GB_COLOR_WHITE = 0,
    GB_COLOR_LIGHT_GRAY,
    GB_COLOR_DARK_GRAY,
    GB_COLOR_BLACK,
};

enum GbPpuMode
{
    GB_PPU_MODE_OAM_SCAN,
    GB_PPU_MODE_DRAWING,
    GB_PPU_MODE_HBLANK,
    GB_PPU_MODE_VBLANK,
};

struct GbPpu
{
    struct GbMmu *mmu;
    struct GbCpu *cpu;

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
    enum GbPpuMode mode;

    enum GbColor *screen;
};

struct GbPpu *gb_ppu_create(void);
void gb_ppu_destroy(struct GbPpu *);

void gb_ppu_write(struct GbPpu *, uint16_t address, uint8_t value);
uint8_t gb_ppu_read(struct GbPpu *, uint16_t address);

void gb_ppu_tick(struct GbPpu *, uint8_t t_cycles);

#ifdef __cplusplus
}
#endif
