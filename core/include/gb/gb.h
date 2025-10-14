/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

struct Cartridge;
struct Cpu;
struct Mmu;
struct Ppu;
struct Timer;

struct Gb
{
    struct Cartridge *cartridge;
    struct Mmu *mmu;
    struct Cpu *cpu;
    struct Ppu *ppu;
    struct Timer *timer;
};

struct Gb *gb_create(const char *rom);
void gb_destroy(struct Gb *);

void gb_run_frame(struct Gb *);

#ifdef __cplusplus
}
#endif
