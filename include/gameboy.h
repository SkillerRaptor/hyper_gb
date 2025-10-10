/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

struct Cartridge;
struct Cpu;
struct Mmu;

struct Gameboy
{
    struct Cartridge *cartridge;
    struct Mmu *mmu;
    struct Cpu *cpu;
};

struct Gameboy *gameboy_create(const char *rom);
void gameboy_destroy(struct Gameboy *);

void gameboy_run(struct Gameboy *);
