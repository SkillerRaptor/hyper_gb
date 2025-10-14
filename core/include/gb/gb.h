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

struct GbCartridge;
struct GbCpu;
struct GbMmu;
struct GbPpu;
struct GbTimer;

struct Gb
{
    struct GbCartridge *cartridge;
    struct GbMmu *mmu;
    struct GbCpu *cpu;
    struct GbPpu *ppu;
    struct GbTimer *timer;
};

struct Gb *gb_create(const char *rom);
void gb_destroy(struct Gb *);

void gb_run_frame(struct Gb *);

#ifdef __cplusplus
}
#endif
