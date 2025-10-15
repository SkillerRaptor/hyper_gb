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

struct GbCartridge;
struct GbCpu;
struct GbPpu;
struct GbTimer;

struct GbMmu
{
    struct GbCartridge *cartridge;
    struct GbCpu *cpu;
    struct GbPpu *ppu;
    struct GbTimer *timer;

#if TESTS_ENABLED
    uint8_t *memory;
#else
    uint8_t *wram;
    uint8_t *oam;
    uint8_t *io;
    uint8_t *hram;
#endif
};

struct GbMmu *gb_mmu_create(void);
void gb_mmu_destroy(struct GbMmu *);

void gb_mmu_write(struct GbMmu *, uint16_t address, uint8_t value);
uint8_t gb_mmu_read(struct GbMmu *, uint16_t address);

#ifdef __cplusplus
}
#endif
