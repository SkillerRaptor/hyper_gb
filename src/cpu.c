/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "cpu.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "mmu.h"

enum Register8
{
    R8_A,
    R8_B,
    R8_C,
    R8_D,
    R8_E,
    R8_H,
    R8_L,
};

enum Register16
{
    R16_AF,
    R16_BC,
    R16_DE,
    R16_HL,
};

enum Flag
{
    FLAG_Z = 1 << 7,
    FLAG_N = 1 << 6,
    FLAG_H = 1 << 5,
    FLAG_C = 1 << 4,
};

static void cpu_set_register8(struct Cpu *cpu, const enum Register8 reg, const uint8_t value)
{
    switch (reg)
    {
    case R8_A: cpu->registers.a = value; break;
    case R8_B: cpu->registers.b = value; break;
    case R8_C: cpu->registers.c = value; break;
    case R8_D: cpu->registers.d = value; break;
    case R8_E: cpu->registers.e = value; break;
    case R8_H: cpu->registers.h = value; break;
    case R8_L: cpu->registers.l = value; break;
    default: break;
    }
}

static uint8_t cpu_get_register8(const struct Cpu *cpu, const enum Register8 reg)
{
    switch (reg)
    {
    case R8_A: return cpu->registers.a;
    case R8_B: return cpu->registers.b;
    case R8_C: return cpu->registers.c;
    case R8_D: return cpu->registers.d;
    case R8_E: return cpu->registers.e;
    case R8_H: return cpu->registers.h;
    case R8_L: return cpu->registers.l;
    default: return 0x00;
    }
}

static void cpu_set_register16(struct Cpu *cpu, const enum Register16 reg, const uint16_t value)
{
    switch (reg)
    {
    case R16_AF: cpu->registers.af = value; break;
    case R16_BC: cpu->registers.bc = value; break;
    case R16_DE: cpu->registers.de = value; break;
    case R16_HL: cpu->registers.hl = value; break;
    default: break;
    }
}

static uint16_t cpu_get_register16(const struct Cpu *cpu, const enum Register16 reg)
{
    switch (reg)
    {
    case R16_AF: return cpu->registers.af;
    case R16_BC: return cpu->registers.bc;
    case R16_DE: return cpu->registers.de;
    case R16_HL: return cpu->registers.hl;
    default: return 0x0000;
    }
}

static void cpu_set_flag(struct Cpu *cpu, const enum Flag flag, const bool value)
{
    if (value)
    {
        cpu->registers.f |= flag;
    }
    else
    {
        cpu->registers.f &= ~flag;
    }
}

static bool cpu_is_flag(const struct Cpu *cpu, const enum Flag flag) { return (cpu->registers.f & flag) != 0; }

struct Cpu *cpu_create(struct Mmu *mmu)
{
    struct Cpu *cpu = malloc(sizeof(struct Cpu));
    cpu->mmu = mmu;

    cpu->registers.a = 0x01;

    cpu_set_flag(cpu, FLAG_Z, true);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, true);
    cpu_set_flag(cpu, FLAG_C, true);

    cpu->registers.b = 0x00;
    cpu->registers.c = 0x13;
    cpu->registers.d = 0x00;
    cpu->registers.e = 0xd8;
    cpu->registers.h = 0x01;
    cpu->registers.l = 0x4d;
    cpu->registers.pc = 0x0100;
    cpu->registers.sp = 0xfffe;

    return cpu;
}

void cpu_destroy(struct Cpu *cpu) { free(cpu); }

static u8 cpu_fetch8(struct Cpu *cpu)
{
    const u8 byte = mmu_read(cpu->mmu, cpu->registers.pc);
    cpu->registers.pc += 1;

    return byte;
}

static u16 cpu_fetch16(struct Cpu *cpu)
{
    const u16 lower_byte = cpu_fetch8(cpu);
    const u16 higher_byte = cpu_fetch8(cpu);

    return (higher_byte << 8) | lower_byte;
}

void cpu_tick(struct Cpu *cpu)
{
    const u8 opcode = cpu_fetch8(cpu);

    switch (opcode)
    {
    default: printf("Unhandled opcode: 0x%02x\n", opcode); break;
    }
}
