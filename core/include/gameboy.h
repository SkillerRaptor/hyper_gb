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

#define GAMEBOY_MASTER_CLOCK_HZ 4194304.0
#define GAMEBOY_FRAME_HZ 59.73
#define GAMEBOY_FRAME_TIME (1.0 / GAMEBOY_FRAME_HZ)
#define GAMEBOY_FRAME_CYCLES (GAMEBOY_MASTER_CLOCK_HZ / GAMEBOY_FRAME_HZ)

struct Cartridge;
struct Cpu;
struct Mmu;
struct Ppu;
struct Timer;

struct Gameboy
{
    struct Cartridge *cartridge;
    struct Mmu *mmu;
    struct Cpu *cpu;
    struct Ppu *ppu;
    struct Timer *timer;
};

struct Gameboy *gameboy_create(const char *rom);
void gameboy_destroy(struct Gameboy *);

void gameboy_run_frame(struct Gameboy *);

#ifdef __cplusplus
}
#endif
