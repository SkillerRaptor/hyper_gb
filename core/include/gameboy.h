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

#define GAMEBOY_WIDTH 160
#define GAMEBOY_HEIGHT 144

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

void gameboy_tick(struct Gameboy *);

#ifdef __cplusplus
}
#endif
