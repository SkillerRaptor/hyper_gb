/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/ppu.h"

#include <stdlib.h>

#include "gb/cpu.h"
#include "gb/definitions.h"
#include "gb/mmu.h"
#include "gb/utils/bits.h"

#define PPU_MODE_OAM_SCAN_DOTS 80
#define PPU_MODE_DRAWING_DOTS 289
#define PPU_MODE_H_BLANK_DOTS 204
#define PPU_MODE_V_BLANK_DOTS 4560

struct GbPpu *gb_ppu_create(void)
{
    struct GbPpu *ppu = malloc(sizeof(struct GbPpu));
    ppu->mmu = NULL;
    ppu->cpu = NULL;
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
    ppu->mode = GB_PPU_MODE_OAM_SCAN;
    ppu->screen = calloc(1, sizeof(enum GbColor) * GB_SCREEN_WIDTH * GB_SCREEN_HEIGHT);

    return ppu;
}

void gb_ppu_destroy(struct GbPpu *ppu)
{
    free(ppu->screen);
    free(ppu->vram);
    free(ppu);
}

void gb_ppu_write(struct GbPpu *ppu, const uint16_t address, const uint8_t value) { ppu->vram[address] = value; }

uint8_t gb_ppu_read(struct GbPpu *ppu, const uint16_t address) { return ppu->vram[address]; }

static void handle_oam_scan(struct GbPpu *ppu) {}

static void handle_drawing(struct GbPpu *ppu) {}

static void handle_hblank(struct GbPpu *ppu)
{
    const bool hblank_interrupt = GB_BIT_CHECK(ppu->lcd_status, 3);
    if (hblank_interrupt)
    {
        gb_cpu_request_interrupt(ppu->cpu, GB_INTERRUPT_LCD);
    }

    // Draw Scanline

    // Draw Background

    if (GB_BIT_CHECK(ppu->lcd_control, 0))
    {
        const uint32_t tile_set_address = GB_BIT_CHECK(ppu->lcd_control, 4) ? 0x8000 : 0x8800;
        const uint32_t tile_map_address = (!GB_BIT_CHECK(ppu->lcd_control, 3)) ? 0x9800 : 0x9c00;

        const uint32_t y = ppu->ly;
        for (uint32_t x = 0; x < GB_SCREEN_WIDTH; ++x)
        {
            const uint32_t scrolled_x = x + ppu->scx;
            const uint32_t scrolled_y = y + ppu->scy;

            const uint32_t background_map_x = scrolled_x % 256;
            const uint32_t background_map_y = scrolled_y % 256;

            const uint32_t tile_x = background_map_x / GB_TILE_SIZE;
            const uint32_t tile_y = background_map_y / GB_TILE_SIZE;

            const uint32_t tile_pixel_x = background_map_x % GB_TILE_SIZE;
            const uint32_t tile_pixel_y = background_map_y % GB_TILE_SIZE;

            // This number is from the tile map
#define TILES_PER_LINE 32
            const uint32_t tile_index = tile_y * TILES_PER_LINE + tile_x;
            const uint32_t tile_id_address = tile_map_address + tile_index;

            const uint8_t tile_id = gb_mmu_read(ppu->mmu, tile_id_address);

#define TILE_BYTES (2 * 8)
            const uint32_t tile_data_memory_offset
                = GB_BIT_CHECK(ppu->lcd_control, 4) ? (tile_id * TILE_BYTES) : ((((int8_t) tile_id) + 128) * TILE_BYTES);
            const uint32_t tile_data_line_offset = tile_pixel_y * 2;

            const uint32_t tile_line_data_start_address = tile_set_address + tile_data_memory_offset + tile_data_line_offset;

            const uint8_t pixels_1 = gb_mmu_read(ppu->mmu, tile_line_data_start_address);
            const uint8_t pixels_2 = gb_mmu_read(ppu->mmu, tile_line_data_start_address + 1);

            const uint8_t color = (GB_BIT_VALUE(pixels_2, 7 - tile_pixel_x) << 1) | GB_BIT_VALUE(pixels_1, 7 - tile_pixel_x);
            ppu->screen[y * GB_SCREEN_WIDTH + x] = (enum GbColor) color;
        }
    }

    // Draw Window
    if (GB_BIT_CHECK(ppu->lcd_control, 5))
    {
    }
}

static void handle_vblank(struct GbPpu *ppu) { gb_cpu_request_interrupt(ppu->cpu, 0); }

void gb_ppu_tick(struct GbPpu *ppu, const uint8_t t_cycles)
{
    ppu->dots_counter += t_cycles;

    switch (ppu->mode)
    {
    case GB_PPU_MODE_OAM_SCAN:
        if (ppu->dots_counter >= PPU_MODE_OAM_SCAN_DOTS)
        {
            ppu->dots_counter -= PPU_MODE_OAM_SCAN_DOTS;
            ppu->mode = GB_PPU_MODE_DRAWING;

            handle_drawing(ppu);
        }
        break;
    case GB_PPU_MODE_DRAWING:
        if (ppu->dots_counter >= PPU_MODE_DRAWING_DOTS)
        {
            ppu->dots_counter -= PPU_MODE_DRAWING_DOTS;
            ppu->mode = GB_PPU_MODE_HBLANK;

            handle_hblank(ppu);
        }
        break;
    case GB_PPU_MODE_HBLANK:
        if (ppu->dots_counter >= PPU_MODE_H_BLANK_DOTS)
        {
            ppu->dots_counter -= PPU_MODE_H_BLANK_DOTS;

            ppu->ly += 1;

            if (ppu->ly >= 0x90)
            {
                ppu->mode = GB_PPU_MODE_VBLANK;
                handle_vblank(ppu);
            }
            else
            {
                ppu->mode = GB_PPU_MODE_OAM_SCAN;
                handle_oam_scan(ppu);
            }
        }
        break;
    case GB_PPU_MODE_VBLANK:
        if (ppu->dots_counter >= PPU_MODE_V_BLANK_DOTS)
        {
            ppu->dots_counter -= PPU_MODE_V_BLANK_DOTS;
            ppu->ly += 1;

            if (ppu->ly >= 0x9a)
            {
                ppu->mode = GB_PPU_MODE_OAM_SCAN;
                ppu->ly = 0;
                handle_oam_scan(ppu);
            }
        }
        break;
    default: break;
    }
}
