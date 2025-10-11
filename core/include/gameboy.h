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

struct Gameboy
{
    struct Cartridge *cartridge;
    struct Mmu *mmu;
    struct Cpu *cpu;
    struct Ppu *ppu;
};

struct Gameboy *gameboy_create(const char *rom);
void gameboy_destroy(struct Gameboy *);

void gameboy_run(struct Gameboy *);

#ifdef __cplusplus
}
#endif
