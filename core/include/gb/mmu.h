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

struct Cartridge;
struct Cpu;
struct Ppu;
struct Timer;

struct Mmu
{
    struct Cartridge *cartridge;
    struct Cpu *cpu;
    struct Ppu *ppu;
    struct Timer *timer;

#if TESTS_ENABLED
    uint8_t *memory;
#else
    uint8_t *wram;
    uint8_t *oam;
    uint8_t *io;
    uint8_t *hram;
#endif
};

struct Mmu *mmu_create();
void mmu_destroy(struct Mmu *);

void mmu_write(struct Mmu *, uint16_t address, uint8_t value);
uint8_t mmu_read(struct Mmu *, uint16_t address);

#ifdef __cplusplus
}
#endif
