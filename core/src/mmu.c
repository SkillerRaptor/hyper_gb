/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "mmu.h"

#include <stdio.h>
#include <stdlib.h>

#include "cartridge.h"
#include "cpu.h"
#include "ppu.h"

struct Mmu *mmu_create(struct Cartridge *cartridge)
{
    struct Mmu *mmu = malloc(sizeof(struct Mmu));
    mmu->cartridge = cartridge;
    mmu->wram = malloc(sizeof(u8) * 0x2000);
    mmu->io = malloc(sizeof(u8) * 0x80);
    mmu->hram = malloc(sizeof(u8) * 0x7f);
    mmu->test_memory = NULL;

    if (mmu->cartridge == NULL)
    {
        mmu->test_memory = malloc(sizeof(u8) * 0x10000);
    }

    return mmu;
}

void mmu_destroy(struct Mmu *mmu)
{
    if (mmu->test_memory)
    {
        free(mmu->test_memory);
    }

    free(mmu->hram);
    free(mmu->io);
    free(mmu->wram);
    free(mmu);
}

// FIXME: Implement mappers

// Start    End     Description	                    Notes
// 0000     3FFF	16 KiB ROM bank 00	            From cartridge, usually a fixed bank
// 4000     7FFF	16 KiB ROM Bank 01–NN	        From cartridge, switchable bank via mapper (if any)
// 8000     9FFF	8 KiB Video RAM (VRAM)	        In CGB mode, switchable bank 0/1
// A000     BFFF	8 KiB External RAM	            From cartridge, switchable bank if any
// C000     CFFF	4 KiB Work RAM (WRAM)
// D000     DFFF	4 KiB Work RAM (WRAM)	        In CGB mode, switchable bank 1–7
// E000     FDFF	Echo RAM (mirror of C000–DDFF)	Nintendo says use of this area is prohibited.
// FE00     FE9F	Object attribute memory (OAM)
// FEA0     FEFF	Not Usable	                    Nintendo says use of this area is prohibited.
// FF00     FF7F	I/O Registers
// FF80     FFFE	High RAM (HRAM)
// FFFF     FFFF	Interrupt Enable register (IE)

static void mmu_write_io(struct Mmu *mmu, const u16 address, const u8 value)
{
    switch (address)
    {
    case 0xff00: mmu->io[address - 0xff00] = value; break; // Joypad
    case 0xff01:
    case 0xff02: mmu->io[address - 0xff00] = value; break; // Serial Transfer
    case 0xff0f: mmu->cpu->interrupt_flag = value; break;
    case 0xff40: mmu->ppu->lcd_control = value; break;
    case 0xff41: mmu->ppu->lcd_status = value; break;
    case 0xff42: mmu->ppu->scy = value; break;
    case 0xff43: mmu->ppu->scx = value; break;
    case 0xff44: mmu->ppu->ly = 0x00; break; // Write will cause to reset
    case 0xff45: mmu->ppu->lyc = value; break;
    case 0xff46: printf("Implement DMA transfer"); break;
    case 0xff47: mmu->ppu->bgp = value; break;
    case 0xff48: mmu->ppu->obp0 = value; break;
    case 0xff49: mmu->ppu->obp1 = value; break;
    case 0xff4a: mmu->ppu->wy = value; break;
    case 0xff4b: mmu->ppu->wx = value; break;
    default:
        mmu->io[address - 0xff00] = value;
        printf("Unhandled write to IO at 0x%04x with 0x%02x\n", address, value);
        break;
    }
}

void mmu_write(struct Mmu *mmu, const u16 address, const u8 value)
{
    if (mmu->test_memory)
    {
        mmu->test_memory[address] = value;
        return;
    }

    if (address <= 0x7fff)
    {
        return;
    }

    if (address >= 0x8000 && address <= 0x9fff)
    {
        mmu->ppu->vram[address - 0x8000] = value;
        return;
    }

    if (address >= 0xc000 && address <= 0xdfff)
    {
        mmu->wram[address - 0xc000] = value;
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
        mmu->cpu->interrupt_enable = value;
        return;
    }

    printf("Unhandled write at 0x%04x with 0x%02x\n", address, value);
}

static u8 mmu_read_io(struct Mmu *mmu, const u16 address)
{
    switch (address)
    {
    case 0xff00: return mmu->io[address - 0xff00]; // Joypad
    case 0xff01:
    case 0xff02: return mmu->io[address - 0xff00]; // Serial Transfer
    case 0xff0f: return mmu->cpu->interrupt_flag;
    case 0xff40: return mmu->ppu->lcd_control;
    case 0xff41: return mmu->ppu->lcd_status;
    case 0xff42: return mmu->ppu->scy;
    case 0xff43: return mmu->ppu->scx;
    case 0xff44: printf("0x%02x\n", mmu->ppu->ly); return mmu->ppu->ly;
    case 0xff45: return mmu->ppu->lyc;
    case 0xff46: printf("Attempted to read from write-only DMA\n"); return 0xff;
    case 0xff47: return mmu->ppu->bgp;
    case 0xff48: return mmu->ppu->obp0;
    case 0xff49: return mmu->ppu->obp1;
    case 0xff4a: return mmu->ppu->wy;
    case 0xff4b: return mmu->ppu->wx;
    default: printf("Unhandled read to IO at 0x%04x\n", address); return mmu->io[address - 0xff00];
    }
}

u8 mmu_read(struct Mmu *mmu, const u16 address)
{
    if (mmu->test_memory)
    {
        return mmu->test_memory[address];
    }

    if (address <= 0x7fff)
    {
        return mmu->cartridge->rom[address];
    }

    if (address >= 0x8000 && address <= 0x9fff)
    {
        return mmu->ppu->vram[address - 0x8000];
    }

    if (address >= 0xc000 && address <= 0xdfff)
    {
        return mmu->wram[address - 0xc000];
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
        return mmu->cpu->interrupt_enable;
    }

    printf("Unhandled read at 0x%04x\n", address);

    return 0;
}
