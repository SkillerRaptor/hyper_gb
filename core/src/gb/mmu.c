/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/mmu.h"

#include <stdio.h>
#include <stdlib.h>

#include "gb/cartridge.h"
#include "gb/cpu.h"
#include "gb/gameboy.h"
#include "gb/logger.h"
#include "gb/ppu.h"
#include "gb/timer.h"

static void mmu_write_io(struct Mmu *mmu, u16 address, u8 value);
static u8 mmu_read_io(struct Mmu *mmu, u16 address);

struct Mmu *mmu_create(struct Gameboy *gb)
{
    struct Mmu *mmu = malloc(sizeof(struct Mmu));
    mmu->gb = gb;

#if TESTS_ENABLED
    mmu->memory = malloc(sizeof(u8) * 0x10000);
#else
    mmu->wram = calloc(1, sizeof(u8) * 0x2000);
    mmu->oam = malloc(sizeof(u8) * 0xa0);
    mmu->io = malloc(sizeof(u8) * 0x80);
    mmu->hram = calloc(1, sizeof(u8) * 0x7f);
#endif

    return mmu;
}

void mmu_destroy(struct Mmu *mmu)
{
#if TESTS_ENABLED
    free(mmu->memory);
#else
    free(mmu->hram);
    free(mmu->io);
    free(mmu->oam);
    free(mmu->wram);
#endif

    free(mmu);
}

void mmu_write(struct Mmu *mmu, const u16 address, const u8 value)
{
#if TESTS_ENABLED
    mmu->memory[address] = value;
#else
    if (address <= 0x7fff)
    {
        return;
    }

    if (address >= 0x8000 && address <= 0x9fff)
    {
        mmu->gb->ppu->vram[address - 0x8000] = value;
        return;
    }

    if (address >= 0xc000 && address <= 0xdfff)
    {
        mmu->wram[address - 0xc000] = value;
        return;
    }

    if (address >= 0xfe00 && address <= 0xfeff)
    {
        mmu->oam[address - 0xfe00] = value;
        return;
    }

    if (address >= 0xff00 && address <= 0xff7f)
    {
        mmu_write_io(mmu, address, value);
        return;
    }

    if (address >= 0xff80 && address <= 0xfffe)
    {
        mmu->hram[address - 0xff80] = value;
        return;
    }

    if (address == 0xffff)
    {
        mmu->gb->cpu->interrupt_enable = value;
        return;
    }

    logger_warn("Unhandled write at 0x%04x with 0x%02x", address, value);
#endif
}

u8 mmu_read(struct Mmu *mmu, const u16 address)
{
#if TESTS_ENABLED
    return mmu->memory[address];
#else

    if (address <= 0x7fff)
    {
        return mmu->gb->cartridge->data[address];
    }

    if (address >= 0x8000 && address <= 0x9fff)
    {
        return mmu->gb->ppu->vram[address - 0x8000];
    }

    if (address >= 0xc000 && address <= 0xdfff)
    {
        return mmu->wram[address - 0xc000];
    }

    if (address >= 0xfe00 && address <= 0xfeff)
    {
        return mmu->oam[address - 0xfe00];
    }

    if (address >= 0xff00 && address <= 0xff7f)
    {
        return mmu_read_io(mmu, address);
    }

    if (address >= 0xff80 && address <= 0xfffe)
    {
        return mmu->hram[address - 0xff80];
    }

    if (address == 0xffff)
    {
        return mmu->gb->cpu->interrupt_enable;
    }

    return 0x00;
#endif
}

#if !TESTS_ENABLED
static void mmu_write_io(struct Mmu *mmu, const u16 address, const u8 value)
{
    struct Cpu *cpu = mmu->gb->cpu;
    struct Ppu *ppu = mmu->gb->ppu;
    struct Timer *timer = mmu->gb->timer;

    switch (address)
    {
    case 0xff01: printf("%c", value); break;
    case 0xff02: break;
    case 0xff04: timer->div = 0xff; break;
    case 0xff05: timer->tima = value; break;
    case 0xff06: timer->tma = value; break;
    case 0xff07: timer->tac = value; break;
    case 0xff0f: cpu->interrupt_flag = value; break;
    case 0xff40: ppu->lcd_control = value; break;
    case 0xff41: ppu->lcd_status = value; break;
    case 0xff42: ppu->scy = value; break;
    case 0xff43: ppu->scx = value; break;
    case 0xff44: ppu->ly = 0x00; break; // Write will cause to reset
    case 0xff45: ppu->lyc = value; break;
    case 0xff46: logger_warn("Attempted to start DMA transfer"); break;
    case 0xff47: ppu->bgp = value; break;
    case 0xff48: ppu->obp0 = value; break;
    case 0xff49: ppu->obp1 = value; break;
    case 0xff4a: ppu->wy = value; break;
    case 0xff4b: ppu->wx = value; break;
    default:
        mmu->io[address - 0xff00] = value;
        logger_warn("Unhandled I/O-write at 0x%04x with 0x%02x", address, value);
        break;
    }
}

static u8 mmu_read_io(struct Mmu *mmu, const u16 address)
{
    const struct Cpu *cpu = mmu->gb->cpu;
    const struct Ppu *ppu = mmu->gb->ppu;
    const struct Timer *timer = mmu->gb->timer;

    switch (address)
    {
    case 0xff00: return 0xff;
    case 0xff04: return timer->div;
    case 0xff05: return timer->tima;
    case 0xff06: return timer->tma;
    case 0xff07: return timer->tac;
    case 0xff0f: return cpu->interrupt_flag;
    case 0xff40: return ppu->lcd_control;
    case 0xff41: return ppu->lcd_status;
    case 0xff42: return ppu->scy;
    case 0xff43: return ppu->scx;
    case 0xff44: return ppu->ly;
    case 0xff45: return ppu->lyc;
    case 0xff46: logger_warn("Attempted to read from write-only DMA register"); return 0xff;
    case 0xff47: return ppu->bgp;
    case 0xff48: return ppu->obp0;
    case 0xff49: return ppu->obp1;
    case 0xff4a: return ppu->wy;
    case 0xff4b: return ppu->wx;
    default: logger_warn("Unhandled I/O-Read at 0x%04x", address); return mmu->io[address - 0xff00];
    }
}
#endif
