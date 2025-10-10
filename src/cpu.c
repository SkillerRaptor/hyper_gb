/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "cpu.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu_instructions.h"
#include "mmu.h"

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

void cpu_set_register8(struct Cpu *cpu, const enum Register8 reg, const u8 value)
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

uint8_t cpu_get_register8(struct Cpu *cpu, const enum Register8 reg)
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

void cpu_set_register16(struct Cpu *cpu, const enum Register16 reg, const u16 value)
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

uint16_t cpu_get_register16(struct Cpu *cpu, const enum Register16 reg)
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

void cpu_set_flag(struct Cpu *cpu, const enum Flag flag, const bool value)
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

bool cpu_is_flag(struct Cpu *cpu, const enum Flag flag) { return (cpu->registers.f & flag) != 0; }

i8 cpu_fetch_i8(struct Cpu *cpu) { return (i8) cpu_fetch_u8(cpu); }

u8 cpu_fetch_u8(struct Cpu *cpu)
{
    const u8 byte = mmu_read(cpu->mmu, cpu->registers.pc);
    cpu->registers.pc += 1;

    return byte;
}

u16 cpu_fetch_u16(struct Cpu *cpu)
{
    const u16 lower_byte = cpu_fetch_u8(cpu);
    const u16 higher_byte = cpu_fetch_u8(cpu);

    return (higher_byte << 8) | lower_byte;
}

static void cpu_execute_cb_opcode(struct Cpu *cpu)
{
    const u8 opcode = cpu_fetch_u8(cpu);

    switch (opcode)
    {
    case 0x00: fprintf(stderr, "Unhandled opcode: 0xcb 0x00\n"); break;
    case 0x01: fprintf(stderr, "Unhandled opcode: 0xcb 0x01\n"); break;
    case 0x02: fprintf(stderr, "Unhandled opcode: 0xcb 0x02\n"); break;
    case 0x03: fprintf(stderr, "Unhandled opcode: 0xcb 0x03\n"); break;
    case 0x04: fprintf(stderr, "Unhandled opcode: 0xcb 0x04\n"); break;
    case 0x05: fprintf(stderr, "Unhandled opcode: 0xcb 0x05\n"); break;
    case 0x06: fprintf(stderr, "Unhandled opcode: 0xcb 0x06\n"); break;
    case 0x07: fprintf(stderr, "Unhandled opcode: 0xcb 0x07\n"); break;
    case 0x08: fprintf(stderr, "Unhandled opcode: 0xcb 0x08\n"); break;
    case 0x09: fprintf(stderr, "Unhandled opcode: 0xcb 0x09\n"); break;
    case 0x0a: fprintf(stderr, "Unhandled opcode: 0xcb 0x0a\n"); break;
    case 0x0b: fprintf(stderr, "Unhandled opcode: 0xcb 0x0b\n"); break;
    case 0x0c: fprintf(stderr, "Unhandled opcode: 0xcb 0x0c\n"); break;
    case 0x0d: fprintf(stderr, "Unhandled opcode: 0xcb 0x0d\n"); break;
    case 0x0e: fprintf(stderr, "Unhandled opcode: 0xcb 0x0e\n"); break;
    case 0x0f: fprintf(stderr, "Unhandled opcode: 0xcb 0x0f\n"); break;

    case 0x10: fprintf(stderr, "Unhandled opcode: 0xcb 0x10\n"); break;
    case 0x11: fprintf(stderr, "Unhandled opcode: 0xcb 0x11\n"); break;
    case 0x12: fprintf(stderr, "Unhandled opcode: 0xcb 0x12\n"); break;
    case 0x13: fprintf(stderr, "Unhandled opcode: 0xcb 0x13\n"); break;
    case 0x14: fprintf(stderr, "Unhandled opcode: 0xcb 0x14\n"); break;
    case 0x15: fprintf(stderr, "Unhandled opcode: 0xcb 0x15\n"); break;
    case 0x16: fprintf(stderr, "Unhandled opcode: 0xcb 0x16\n"); break;
    case 0x17: fprintf(stderr, "Unhandled opcode: 0xcb 0x17\n"); break;
    case 0x18: fprintf(stderr, "Unhandled opcode: 0xcb 0x18\n"); break;
    case 0x19: fprintf(stderr, "Unhandled opcode: 0xcb 0x19\n"); break;
    case 0x1a: fprintf(stderr, "Unhandled opcode: 0xcb 0x1a\n"); break;
    case 0x1b: fprintf(stderr, "Unhandled opcode: 0xcb 0x1b\n"); break;
    case 0x1c: fprintf(stderr, "Unhandled opcode: 0xcb 0x1c\n"); break;
    case 0x1d: fprintf(stderr, "Unhandled opcode: 0xcb 0x1d\n"); break;
    case 0x1e: fprintf(stderr, "Unhandled opcode: 0xcb 0x1e\n"); break;
    case 0x1f: fprintf(stderr, "Unhandled opcode: 0xcb 0x1f\n"); break;

    case 0x20: fprintf(stderr, "Unhandled opcode: 0xcb 0x20\n"); break;
    case 0x21: fprintf(stderr, "Unhandled opcode: 0xcb 0x21\n"); break;
    case 0x22: fprintf(stderr, "Unhandled opcode: 0xcb 0x22\n"); break;
    case 0x23: fprintf(stderr, "Unhandled opcode: 0xcb 0x23\n"); break;
    case 0x24: fprintf(stderr, "Unhandled opcode: 0xcb 0x24\n"); break;
    case 0x25: fprintf(stderr, "Unhandled opcode: 0xcb 0x25\n"); break;
    case 0x26: fprintf(stderr, "Unhandled opcode: 0xcb 0x26\n"); break;
    case 0x27: fprintf(stderr, "Unhandled opcode: 0xcb 0x27\n"); break;
    case 0x28: fprintf(stderr, "Unhandled opcode: 0xcb 0x28\n"); break;
    case 0x29: fprintf(stderr, "Unhandled opcode: 0xcb 0x29\n"); break;
    case 0x2a: fprintf(stderr, "Unhandled opcode: 0xcb 0x2a\n"); break;
    case 0x2b: fprintf(stderr, "Unhandled opcode: 0xcb 0x2b\n"); break;
    case 0x2c: fprintf(stderr, "Unhandled opcode: 0xcb 0x2c\n"); break;
    case 0x2d: fprintf(stderr, "Unhandled opcode: 0xcb 0x2d\n"); break;
    case 0x2e: fprintf(stderr, "Unhandled opcode: 0xcb 0x2e\n"); break;
    case 0x2f: fprintf(stderr, "Unhandled opcode: 0xcb 0x2f\n"); break;

    case 0x30: fprintf(stderr, "Unhandled opcode: 0xcb 0x30\n"); break;
    case 0x31: fprintf(stderr, "Unhandled opcode: 0xcb 0x31\n"); break;
    case 0x32: fprintf(stderr, "Unhandled opcode: 0xcb 0x32\n"); break;
    case 0x33: fprintf(stderr, "Unhandled opcode: 0xcb 0x33\n"); break;
    case 0x34: fprintf(stderr, "Unhandled opcode: 0xcb 0x34\n"); break;
    case 0x35: fprintf(stderr, "Unhandled opcode: 0xcb 0x35\n"); break;
    case 0x36: fprintf(stderr, "Unhandled opcode: 0xcb 0x36\n"); break;
    case 0x37: fprintf(stderr, "Unhandled opcode: 0xcb 0x37\n"); break;
    case 0x38: fprintf(stderr, "Unhandled opcode: 0xcb 0x38\n"); break;
    case 0x39: fprintf(stderr, "Unhandled opcode: 0xcb 0x39\n"); break;
    case 0x3a: fprintf(stderr, "Unhandled opcode: 0xcb 0x3a\n"); break;
    case 0x3b: fprintf(stderr, "Unhandled opcode: 0xcb 0x3b\n"); break;
    case 0x3c: fprintf(stderr, "Unhandled opcode: 0xcb 0x3c\n"); break;
    case 0x3d: fprintf(stderr, "Unhandled opcode: 0xcb 0x3d\n"); break;
    case 0x3e: fprintf(stderr, "Unhandled opcode: 0xcb 0x3e\n"); break;
    case 0x3f: fprintf(stderr, "Unhandled opcode: 0xcb 0x3f\n"); break;

    case 0x40: fprintf(stderr, "Unhandled opcode: 0xcb 0x40\n"); break;
    case 0x41: fprintf(stderr, "Unhandled opcode: 0xcb 0x41\n"); break;
    case 0x42: fprintf(stderr, "Unhandled opcode: 0xcb 0x42\n"); break;
    case 0x43: fprintf(stderr, "Unhandled opcode: 0xcb 0x43\n"); break;
    case 0x44: fprintf(stderr, "Unhandled opcode: 0xcb 0x44\n"); break;
    case 0x45: fprintf(stderr, "Unhandled opcode: 0xcb 0x45\n"); break;
    case 0x46: fprintf(stderr, "Unhandled opcode: 0xcb 0x46\n"); break;
    case 0x47: fprintf(stderr, "Unhandled opcode: 0xcb 0x47\n"); break;
    case 0x48: fprintf(stderr, "Unhandled opcode: 0xcb 0x48\n"); break;
    case 0x49: fprintf(stderr, "Unhandled opcode: 0xcb 0x49\n"); break;
    case 0x4a: fprintf(stderr, "Unhandled opcode: 0xcb 0x4a\n"); break;
    case 0x4b: fprintf(stderr, "Unhandled opcode: 0xcb 0x4b\n"); break;
    case 0x4c: fprintf(stderr, "Unhandled opcode: 0xcb 0x4c\n"); break;
    case 0x4d: fprintf(stderr, "Unhandled opcode: 0xcb 0x4d\n"); break;
    case 0x4e: fprintf(stderr, "Unhandled opcode: 0xcb 0x4e\n"); break;
    case 0x4f: fprintf(stderr, "Unhandled opcode: 0xcb 0x4f\n"); break;

    case 0x50: fprintf(stderr, "Unhandled opcode: 0xcb 0x50\n"); break;
    case 0x51: fprintf(stderr, "Unhandled opcode: 0xcb 0x51\n"); break;
    case 0x52: fprintf(stderr, "Unhandled opcode: 0xcb 0x52\n"); break;
    case 0x53: fprintf(stderr, "Unhandled opcode: 0xcb 0x53\n"); break;
    case 0x54: fprintf(stderr, "Unhandled opcode: 0xcb 0x54\n"); break;
    case 0x55: fprintf(stderr, "Unhandled opcode: 0xcb 0x55\n"); break;
    case 0x56: fprintf(stderr, "Unhandled opcode: 0xcb 0x56\n"); break;
    case 0x57: fprintf(stderr, "Unhandled opcode: 0xcb 0x57\n"); break;
    case 0x58: fprintf(stderr, "Unhandled opcode: 0xcb 0x58\n"); break;
    case 0x59: fprintf(stderr, "Unhandled opcode: 0xcb 0x59\n"); break;
    case 0x5a: fprintf(stderr, "Unhandled opcode: 0xcb 0x5a\n"); break;
    case 0x5b: fprintf(stderr, "Unhandled opcode: 0xcb 0x5b\n"); break;
    case 0x5c: fprintf(stderr, "Unhandled opcode: 0xcb 0x5c\n"); break;
    case 0x5d: fprintf(stderr, "Unhandled opcode: 0xcb 0x5d\n"); break;
    case 0x5e: fprintf(stderr, "Unhandled opcode: 0xcb 0x5e\n"); break;
    case 0x5f: fprintf(stderr, "Unhandled opcode: 0xcb 0x5f\n"); break;

    case 0x60: fprintf(stderr, "Unhandled opcode: 0xcb 0x60\n"); break;
    case 0x61: fprintf(stderr, "Unhandled opcode: 0xcb 0x61\n"); break;
    case 0x62: fprintf(stderr, "Unhandled opcode: 0xcb 0x62\n"); break;
    case 0x63: fprintf(stderr, "Unhandled opcode: 0xcb 0x63\n"); break;
    case 0x64: fprintf(stderr, "Unhandled opcode: 0xcb 0x64\n"); break;
    case 0x65: fprintf(stderr, "Unhandled opcode: 0xcb 0x65\n"); break;
    case 0x66: fprintf(stderr, "Unhandled opcode: 0xcb 0x66\n"); break;
    case 0x67: fprintf(stderr, "Unhandled opcode: 0xcb 0x67\n"); break;
    case 0x68: fprintf(stderr, "Unhandled opcode: 0xcb 0x68\n"); break;
    case 0x69: fprintf(stderr, "Unhandled opcode: 0xcb 0x69\n"); break;
    case 0x6a: fprintf(stderr, "Unhandled opcode: 0xcb 0x6a\n"); break;
    case 0x6b: fprintf(stderr, "Unhandled opcode: 0xcb 0x6b\n"); break;
    case 0x6c: fprintf(stderr, "Unhandled opcode: 0xcb 0x6c\n"); break;
    case 0x6d: fprintf(stderr, "Unhandled opcode: 0xcb 0x6d\n"); break;
    case 0x6e: fprintf(stderr, "Unhandled opcode: 0xcb 0x6e\n"); break;
    case 0x6f: fprintf(stderr, "Unhandled opcode: 0xcb 0x6f\n"); break;

    case 0x70: fprintf(stderr, "Unhandled opcode: 0xcb 0x70\n"); break;
    case 0x71: fprintf(stderr, "Unhandled opcode: 0xcb 0x71\n"); break;
    case 0x72: fprintf(stderr, "Unhandled opcode: 0xcb 0x72\n"); break;
    case 0x73: fprintf(stderr, "Unhandled opcode: 0xcb 0x73\n"); break;
    case 0x74: fprintf(stderr, "Unhandled opcode: 0xcb 0x74\n"); break;
    case 0x75: fprintf(stderr, "Unhandled opcode: 0xcb 0x75\n"); break;
    case 0x76: fprintf(stderr, "Unhandled opcode: 0xcb 0x76\n"); break;
    case 0x77: fprintf(stderr, "Unhandled opcode: 0xcb 0x77\n"); break;
    case 0x78: fprintf(stderr, "Unhandled opcode: 0xcb 0x78\n"); break;
    case 0x79: fprintf(stderr, "Unhandled opcode: 0xcb 0x79\n"); break;
    case 0x7a: fprintf(stderr, "Unhandled opcode: 0xcb 0x7a\n"); break;
    case 0x7b: fprintf(stderr, "Unhandled opcode: 0xcb 0x7b\n"); break;
    case 0x7c: fprintf(stderr, "Unhandled opcode: 0xcb 0x7c\n"); break;
    case 0x7d: fprintf(stderr, "Unhandled opcode: 0xcb 0x7d\n"); break;
    case 0x7e: fprintf(stderr, "Unhandled opcode: 0xcb 0x7e\n"); break;
    case 0x7f: fprintf(stderr, "Unhandled opcode: 0xcb 0x7f\n"); break;

    case 0x80: fprintf(stderr, "Unhandled opcode: 0xcb 0x80\n"); break;
    case 0x81: fprintf(stderr, "Unhandled opcode: 0xcb 0x81\n"); break;
    case 0x82: fprintf(stderr, "Unhandled opcode: 0xcb 0x82\n"); break;
    case 0x83: fprintf(stderr, "Unhandled opcode: 0xcb 0x83\n"); break;
    case 0x84: fprintf(stderr, "Unhandled opcode: 0xcb 0x84\n"); break;
    case 0x85: fprintf(stderr, "Unhandled opcode: 0xcb 0x85\n"); break;
    case 0x86: fprintf(stderr, "Unhandled opcode: 0xcb 0x86\n"); break;
    case 0x87: fprintf(stderr, "Unhandled opcode: 0xcb 0x87\n"); break;
    case 0x88: fprintf(stderr, "Unhandled opcode: 0xcb 0x88\n"); break;
    case 0x89: fprintf(stderr, "Unhandled opcode: 0xcb 0x89\n"); break;
    case 0x8a: fprintf(stderr, "Unhandled opcode: 0xcb 0x8a\n"); break;
    case 0x8b: fprintf(stderr, "Unhandled opcode: 0xcb 0x8b\n"); break;
    case 0x8c: fprintf(stderr, "Unhandled opcode: 0xcb 0x8c\n"); break;
    case 0x8d: fprintf(stderr, "Unhandled opcode: 0xcb 0x8d\n"); break;
    case 0x8e: fprintf(stderr, "Unhandled opcode: 0xcb 0x8e\n"); break;
    case 0x8f: fprintf(stderr, "Unhandled opcode: 0xcb 0x8f\n"); break;

    case 0x90: fprintf(stderr, "Unhandled opcode: 0xcb 0x90\n"); break;
    case 0x91: fprintf(stderr, "Unhandled opcode: 0xcb 0x91\n"); break;
    case 0x92: fprintf(stderr, "Unhandled opcode: 0xcb 0x92\n"); break;
    case 0x93: fprintf(stderr, "Unhandled opcode: 0xcb 0x93\n"); break;
    case 0x94: fprintf(stderr, "Unhandled opcode: 0xcb 0x94\n"); break;
    case 0x95: fprintf(stderr, "Unhandled opcode: 0xcb 0x95\n"); break;
    case 0x96: fprintf(stderr, "Unhandled opcode: 0xcb 0x96\n"); break;
    case 0x97: fprintf(stderr, "Unhandled opcode: 0xcb 0x97\n"); break;
    case 0x98: fprintf(stderr, "Unhandled opcode: 0xcb 0x98\n"); break;
    case 0x99: fprintf(stderr, "Unhandled opcode: 0xcb 0x99\n"); break;
    case 0x9a: fprintf(stderr, "Unhandled opcode: 0xcb 0x9a\n"); break;
    case 0x9b: fprintf(stderr, "Unhandled opcode: 0xcb 0x9b\n"); break;
    case 0x9c: fprintf(stderr, "Unhandled opcode: 0xcb 0x9c\n"); break;
    case 0x9d: fprintf(stderr, "Unhandled opcode: 0xcb 0x9d\n"); break;
    case 0x9e: fprintf(stderr, "Unhandled opcode: 0xcb 0x9e\n"); break;
    case 0x9f: fprintf(stderr, "Unhandled opcode: 0xcb 0x9f\n"); break;

    case 0xa0: fprintf(stderr, "Unhandled opcode: 0xcb 0xa0\n"); break;
    case 0xa1: fprintf(stderr, "Unhandled opcode: 0xcb 0xa1\n"); break;
    case 0xa2: fprintf(stderr, "Unhandled opcode: 0xcb 0xa2\n"); break;
    case 0xa3: fprintf(stderr, "Unhandled opcode: 0xcb 0xa3\n"); break;
    case 0xa4: fprintf(stderr, "Unhandled opcode: 0xcb 0xa4\n"); break;
    case 0xa5: fprintf(stderr, "Unhandled opcode: 0xcb 0xa5\n"); break;
    case 0xa6: fprintf(stderr, "Unhandled opcode: 0xcb 0xa6\n"); break;
    case 0xa7: fprintf(stderr, "Unhandled opcode: 0xcb 0xa7\n"); break;
    case 0xa8: fprintf(stderr, "Unhandled opcode: 0xcb 0xa8\n"); break;
    case 0xa9: fprintf(stderr, "Unhandled opcode: 0xcb 0xa9\n"); break;
    case 0xaa: fprintf(stderr, "Unhandled opcode: 0xcb 0xaa\n"); break;
    case 0xab: fprintf(stderr, "Unhandled opcode: 0xcb 0xab\n"); break;
    case 0xac: fprintf(stderr, "Unhandled opcode: 0xcb 0xac\n"); break;
    case 0xad: fprintf(stderr, "Unhandled opcode: 0xcb 0xad\n"); break;
    case 0xae: fprintf(stderr, "Unhandled opcode: 0xcb 0xae\n"); break;
    case 0xaf: fprintf(stderr, "Unhandled opcode: 0xcb 0xaf\n"); break;

    case 0xb0: fprintf(stderr, "Unhandled opcode: 0xcb 0xb0\n"); break;
    case 0xb1: fprintf(stderr, "Unhandled opcode: 0xcb 0xb1\n"); break;
    case 0xb2: fprintf(stderr, "Unhandled opcode: 0xcb 0xb2\n"); break;
    case 0xb3: fprintf(stderr, "Unhandled opcode: 0xcb 0xb3\n"); break;
    case 0xb4: fprintf(stderr, "Unhandled opcode: 0xcb 0xb4\n"); break;
    case 0xb5: fprintf(stderr, "Unhandled opcode: 0xcb 0xb5\n"); break;
    case 0xb6: fprintf(stderr, "Unhandled opcode: 0xcb 0xb6\n"); break;
    case 0xb7: fprintf(stderr, "Unhandled opcode: 0xcb 0xb7\n"); break;
    case 0xb8: fprintf(stderr, "Unhandled opcode: 0xcb 0xb8\n"); break;
    case 0xb9: fprintf(stderr, "Unhandled opcode: 0xcb 0xb9\n"); break;
    case 0xba: fprintf(stderr, "Unhandled opcode: 0xcb 0xba\n"); break;
    case 0xbb: fprintf(stderr, "Unhandled opcode: 0xcb 0xbb\n"); break;
    case 0xbc: fprintf(stderr, "Unhandled opcode: 0xcb 0xbc\n"); break;
    case 0xbd: fprintf(stderr, "Unhandled opcode: 0xcb 0xbd\n"); break;
    case 0xbe: fprintf(stderr, "Unhandled opcode: 0xcb 0xbe\n"); break;
    case 0xbf: fprintf(stderr, "Unhandled opcode: 0xcb 0xbf\n"); break;

    case 0xc0: fprintf(stderr, "Unhandled opcode: 0xcb 0xc0\n"); break;
    case 0xc1: fprintf(stderr, "Unhandled opcode: 0xcb 0xc1\n"); break;
    case 0xc2: fprintf(stderr, "Unhandled opcode: 0xcb 0xc2\n"); break;
    case 0xc3: fprintf(stderr, "Unhandled opcode: 0xcb 0xc3\n"); break;
    case 0xc4: fprintf(stderr, "Unhandled opcode: 0xcb 0xc4\n"); break;
    case 0xc5: fprintf(stderr, "Unhandled opcode: 0xcb 0xc5\n"); break;
    case 0xc6: fprintf(stderr, "Unhandled opcode: 0xcb 0xc6\n"); break;
    case 0xc7: fprintf(stderr, "Unhandled opcode: 0xcb 0xc7\n"); break;
    case 0xc8: fprintf(stderr, "Unhandled opcode: 0xcb 0xc8\n"); break;
    case 0xc9: fprintf(stderr, "Unhandled opcode: 0xcb 0xc9\n"); break;
    case 0xca: fprintf(stderr, "Unhandled opcode: 0xcb 0xca\n"); break;
    case 0xcb: fprintf(stderr, "Unhandled opcode: 0xcb 0xcb\n"); break;
    case 0xcc: fprintf(stderr, "Unhandled opcode: 0xcb 0xcc\n"); break;
    case 0xcd: fprintf(stderr, "Unhandled opcode: 0xcb 0xcd\n"); break;
    case 0xce: fprintf(stderr, "Unhandled opcode: 0xcb 0xce\n"); break;
    case 0xcf: fprintf(stderr, "Unhandled opcode: 0xcb 0xcf\n"); break;

    case 0xd0: fprintf(stderr, "Unhandled opcode: 0xcb 0xd0\n"); break;
    case 0xd1: fprintf(stderr, "Unhandled opcode: 0xcb 0xd1\n"); break;
    case 0xd2: fprintf(stderr, "Unhandled opcode: 0xcb 0xd2\n"); break;
    case 0xd3: fprintf(stderr, "Unhandled opcode: 0xcb 0xd3\n"); break;
    case 0xd4: fprintf(stderr, "Unhandled opcode: 0xcb 0xd4\n"); break;
    case 0xd5: fprintf(stderr, "Unhandled opcode: 0xcb 0xd5\n"); break;
    case 0xd6: fprintf(stderr, "Unhandled opcode: 0xcb 0xd6\n"); break;
    case 0xd7: fprintf(stderr, "Unhandled opcode: 0xcb 0xd7\n"); break;
    case 0xd8: fprintf(stderr, "Unhandled opcode: 0xcb 0xd8\n"); break;
    case 0xd9: fprintf(stderr, "Unhandled opcode: 0xcb 0xd9\n"); break;
    case 0xda: fprintf(stderr, "Unhandled opcode: 0xcb 0xda\n"); break;
    case 0xdb: fprintf(stderr, "Unhandled opcode: 0xcb 0xdb\n"); break;
    case 0xdc: fprintf(stderr, "Unhandled opcode: 0xcb 0xdc\n"); break;
    case 0xdd: fprintf(stderr, "Unhandled opcode: 0xcb 0xdd\n"); break;
    case 0xde: fprintf(stderr, "Unhandled opcode: 0xcb 0xde\n"); break;
    case 0xdf: fprintf(stderr, "Unhandled opcode: 0xcb 0xdf\n"); break;

    case 0xe0: fprintf(stderr, "Unhandled opcode: 0xcb 0xe0\n"); break;
    case 0xe1: fprintf(stderr, "Unhandled opcode: 0xcb 0xe1\n"); break;
    case 0xe2: fprintf(stderr, "Unhandled opcode: 0xcb 0xe2\n"); break;
    case 0xe3: fprintf(stderr, "Unhandled opcode: 0xcb 0xe3\n"); break;
    case 0xe4: fprintf(stderr, "Unhandled opcode: 0xcb 0xe4\n"); break;
    case 0xe5: fprintf(stderr, "Unhandled opcode: 0xcb 0xe5\n"); break;
    case 0xe6: fprintf(stderr, "Unhandled opcode: 0xcb 0xe6\n"); break;
    case 0xe7: fprintf(stderr, "Unhandled opcode: 0xcb 0xe7\n"); break;
    case 0xe8: fprintf(stderr, "Unhandled opcode: 0xcb 0xe8\n"); break;
    case 0xe9: fprintf(stderr, "Unhandled opcode: 0xcb 0xe9\n"); break;
    case 0xea: fprintf(stderr, "Unhandled opcode: 0xcb 0xea\n"); break;
    case 0xeb: fprintf(stderr, "Unhandled opcode: 0xcb 0xeb\n"); break;
    case 0xec: fprintf(stderr, "Unhandled opcode: 0xcb 0xec\n"); break;
    case 0xed: fprintf(stderr, "Unhandled opcode: 0xcb 0xed\n"); break;
    case 0xee: fprintf(stderr, "Unhandled opcode: 0xcb 0xee\n"); break;
    case 0xef: fprintf(stderr, "Unhandled opcode: 0xcb 0xef\n"); break;

    case 0xf0: fprintf(stderr, "Unhandled opcode: 0xcb 0xf0\n"); break;
    case 0xf1: fprintf(stderr, "Unhandled opcode: 0xcb 0xf1\n"); break;
    case 0xf2: fprintf(stderr, "Unhandled opcode: 0xcb 0xf2\n"); break;
    case 0xf3: fprintf(stderr, "Unhandled opcode: 0xcb 0xf3\n"); break;
    case 0xf4: fprintf(stderr, "Unhandled opcode: 0xcb 0xf4\n"); break;
    case 0xf5: fprintf(stderr, "Unhandled opcode: 0xcb 0xf5\n"); break;
    case 0xf6: fprintf(stderr, "Unhandled opcode: 0xcb 0xf6\n"); break;
    case 0xf7: fprintf(stderr, "Unhandled opcode: 0xcb 0xf7\n"); break;
    case 0xf8: fprintf(stderr, "Unhandled opcode: 0xcb 0xf8\n"); break;
    case 0xf9: fprintf(stderr, "Unhandled opcode: 0xcb 0xf9\n"); break;
    case 0xfa: fprintf(stderr, "Unhandled opcode: 0xcb 0xfa\n"); break;
    case 0xfb: fprintf(stderr, "Unhandled opcode: 0xcb 0xfb\n"); break;
    case 0xfc: fprintf(stderr, "Unhandled opcode: 0xcb 0xfc\n"); break;
    case 0xfd: fprintf(stderr, "Unhandled opcode: 0xcb 0xfd\n"); break;
    case 0xfe: fprintf(stderr, "Unhandled opcode: 0xcb 0xfe\n"); break;
    case 0xff: fprintf(stderr, "Unhandled opcode: 0xcb 0xff\n"); break;

    default: break;
    }
}

static void cpu_execute_opcode(struct Cpu *cpu)
{
    const u8 opcode = cpu_fetch_u8(cpu);

    switch (opcode)
    {
    case 0x00: fprintf(stderr, "Unhandled opcode: 0x00\n"); break;
    case 0x01: cpu_ld_r16_n16(cpu, R16_BC); break;
    case 0x02: cpu_ld_r16_a(cpu, R16_BC); break;
    case 0x03: fprintf(stderr, "Unhandled opcode: 0x03\n"); break;
    case 0x04: fprintf(stderr, "Unhandled opcode: 0x04\n"); break;
    case 0x05: fprintf(stderr, "Unhandled opcode: 0x05\n"); break;
    case 0x06: cpu_ld_r8_n8(cpu, R8_B); break;
    case 0x07: fprintf(stderr, "Unhandled opcode: 0x07\n"); break;
    case 0x08: fprintf(stderr, "Unhandled opcode: 0x08\n"); break;
    case 0x09: fprintf(stderr, "Unhandled opcode: 0x09\n"); break;
    case 0x0a: cpu_ld_a_r16(cpu, R16_BC); break;
    case 0x0b: fprintf(stderr, "Unhandled opcode: 0x0b\n"); break;
    case 0x0c: fprintf(stderr, "Unhandled opcode: 0x0c\n"); break;
    case 0x0d: fprintf(stderr, "Unhandled opcode: 0x0d\n"); break;
    case 0x0e: cpu_ld_r8_n8(cpu, R8_C); break;
    case 0x0f: fprintf(stderr, "Unhandled opcode: 0x0f\n"); break;

    case 0x10: fprintf(stderr, "Unhandled opcode: 0x10\n"); break;
    case 0x11: cpu_ld_r16_n16(cpu, R16_DE); break;
    case 0x12: cpu_ld_r16_a(cpu, R16_DE); break;
    case 0x13: fprintf(stderr, "Unhandled opcode: 0x13\n"); break;
    case 0x14: fprintf(stderr, "Unhandled opcode: 0x14\n"); break;
    case 0x15: fprintf(stderr, "Unhandled opcode: 0x15\n"); break;
    case 0x16: cpu_ld_r8_n8(cpu, R8_D); break;
    case 0x17: fprintf(stderr, "Unhandled opcode: 0x17\n"); break;
    case 0x18: fprintf(stderr, "Unhandled opcode: 0x18\n"); break;
    case 0x19: fprintf(stderr, "Unhandled opcode: 0x19\n"); break;
    case 0x1a: cpu_ld_a_r16(cpu, R16_DE); break;
    case 0x1b: fprintf(stderr, "Unhandled opcode: 0x1b\n"); break;
    case 0x1c: fprintf(stderr, "Unhandled opcode: 0x1c\n"); break;
    case 0x1d: fprintf(stderr, "Unhandled opcode: 0x1d\n"); break;
    case 0x1e: cpu_ld_r8_n8(cpu, R8_E); break;
    case 0x1f: fprintf(stderr, "Unhandled opcode: 0x1f\n"); break;

    case 0x20: fprintf(stderr, "Unhandled opcode: 0x20\n"); break;
    case 0x21: cpu_ld_r16_n16(cpu, R16_HL); break;
    case 0x22: cpu_ld_hli_a(cpu); break;
    case 0x23: fprintf(stderr, "Unhandled opcode: 0x23\n"); break;
    case 0x24: fprintf(stderr, "Unhandled opcode: 0x24\n"); break;
    case 0x25: fprintf(stderr, "Unhandled opcode: 0x25\n"); break;
    case 0x26: cpu_ld_r8_n8(cpu, R8_H); break;
    case 0x27: fprintf(stderr, "Unhandled opcode: 0x27\n"); break;
    case 0x28: fprintf(stderr, "Unhandled opcode: 0x28\n"); break;
    case 0x29: fprintf(stderr, "Unhandled opcode: 0x29\n"); break;
    case 0x2a: cpu_ld_a_hli(cpu); break;
    case 0x2b: fprintf(stderr, "Unhandled opcode: 0x2b\n"); break;
    case 0x2c: fprintf(stderr, "Unhandled opcode: 0x2c\n"); break;
    case 0x2d: fprintf(stderr, "Unhandled opcode: 0x2d\n"); break;
    case 0x2e: cpu_ld_r8_n8(cpu, R8_L); break;
    case 0x2f: fprintf(stderr, "Unhandled opcode: 0x2f\n"); break;

    case 0x30: fprintf(stderr, "Unhandled opcode: 0x30\n"); break;
    case 0x31: fprintf(stderr, "Unhandled opcode: 0x31\n"); break;
    case 0x32: cpu_ld_hld_a(cpu); break;
    case 0x33: fprintf(stderr, "Unhandled opcode: 0x33\n"); break;
    case 0x34: fprintf(stderr, "Unhandled opcode: 0x34\n"); break;
    case 0x35: fprintf(stderr, "Unhandled opcode: 0x35\n"); break;
    case 0x36: cpu_ld_hl_n8(cpu); break;
    case 0x37: fprintf(stderr, "Unhandled opcode: 0x37\n"); break;
    case 0x38: fprintf(stderr, "Unhandled opcode: 0x38\n"); break;
    case 0x39: fprintf(stderr, "Unhandled opcode: 0x39\n"); break;
    case 0x3a: cpu_ld_a_hld(cpu); break;
    case 0x3b: fprintf(stderr, "Unhandled opcode: 0x3b\n"); break;
    case 0x3c: fprintf(stderr, "Unhandled opcode: 0x3c\n"); break;
    case 0x3d: fprintf(stderr, "Unhandled opcode: 0x3d\n"); break;
    case 0x3e: cpu_ld_r8_n8(cpu, R8_A); break;
    case 0x3f: fprintf(stderr, "Unhandled opcode: 0x3f\n"); break;

    case 0x40: cpu_ld_r8_r8(cpu, R8_B, R8_B); break;
    case 0x41: cpu_ld_r8_r8(cpu, R8_B, R8_C); break;
    case 0x42: cpu_ld_r8_r8(cpu, R8_B, R8_D); break;
    case 0x43: cpu_ld_r8_r8(cpu, R8_B, R8_E); break;
    case 0x44: cpu_ld_r8_r8(cpu, R8_B, R8_H); break;
    case 0x45: cpu_ld_r8_r8(cpu, R8_B, R8_L); break;
    case 0x46: cpu_ld_r8_hl(cpu, R8_B); break;
    case 0x47: cpu_ld_r8_r8(cpu, R8_B, R8_A); break;
    case 0x48: cpu_ld_r8_r8(cpu, R8_C, R8_B); break;
    case 0x49: cpu_ld_r8_r8(cpu, R8_C, R8_C); break;
    case 0x4a: cpu_ld_r8_r8(cpu, R8_C, R8_D); break;
    case 0x4b: cpu_ld_r8_r8(cpu, R8_C, R8_E); break;
    case 0x4c: cpu_ld_r8_r8(cpu, R8_C, R8_H); break;
    case 0x4d: cpu_ld_r8_r8(cpu, R8_C, R8_L); break;
    case 0x4e: cpu_ld_r8_hl(cpu, R8_C); break;
    case 0x4f: cpu_ld_r8_r8(cpu, R8_C, R8_A); break;

    case 0x50: cpu_ld_r8_r8(cpu, R8_D, R8_B); break;
    case 0x51: cpu_ld_r8_r8(cpu, R8_D, R8_C); break;
    case 0x52: cpu_ld_r8_r8(cpu, R8_D, R8_D); break;
    case 0x53: cpu_ld_r8_r8(cpu, R8_D, R8_E); break;
    case 0x54: cpu_ld_r8_r8(cpu, R8_D, R8_H); break;
    case 0x55: cpu_ld_r8_r8(cpu, R8_D, R8_L); break;
    case 0x56: cpu_ld_r8_hl(cpu, R8_D); break;
    case 0x57: cpu_ld_r8_r8(cpu, R8_D, R8_A); break;
    case 0x58: cpu_ld_r8_r8(cpu, R8_E, R8_B); break;
    case 0x59: cpu_ld_r8_r8(cpu, R8_E, R8_C); break;
    case 0x5a: cpu_ld_r8_r8(cpu, R8_E, R8_D); break;
    case 0x5b: cpu_ld_r8_r8(cpu, R8_E, R8_E); break;
    case 0x5c: cpu_ld_r8_r8(cpu, R8_E, R8_H); break;
    case 0x5d: cpu_ld_r8_r8(cpu, R8_E, R8_L); break;
    case 0x5e: cpu_ld_r8_hl(cpu, R8_E); break;
    case 0x5f: cpu_ld_r8_r8(cpu, R8_E, R8_A); break;

    case 0x60: cpu_ld_r8_r8(cpu, R8_H, R8_B); break;
    case 0x61: cpu_ld_r8_r8(cpu, R8_H, R8_C); break;
    case 0x62: cpu_ld_r8_r8(cpu, R8_H, R8_D); break;
    case 0x63: cpu_ld_r8_r8(cpu, R8_H, R8_E); break;
    case 0x64: cpu_ld_r8_r8(cpu, R8_H, R8_H); break;
    case 0x65: cpu_ld_r8_r8(cpu, R8_H, R8_L); break;
    case 0x66: cpu_ld_r8_hl(cpu, R8_H); break;
    case 0x67: cpu_ld_r8_r8(cpu, R8_H, R8_A); break;
    case 0x68: cpu_ld_r8_r8(cpu, R8_L, R8_B); break;
    case 0x69: cpu_ld_r8_r8(cpu, R8_L, R8_C); break;
    case 0x6a: cpu_ld_r8_r8(cpu, R8_L, R8_D); break;
    case 0x6b: cpu_ld_r8_r8(cpu, R8_L, R8_E); break;
    case 0x6c: cpu_ld_r8_r8(cpu, R8_L, R8_H); break;
    case 0x6d: cpu_ld_r8_r8(cpu, R8_L, R8_L); break;
    case 0x6e: cpu_ld_r8_hl(cpu, R8_L); break;
    case 0x6f: cpu_ld_r8_r8(cpu, R8_L, R8_A); break;

    case 0x70: cpu_ld_hl_r8(cpu, R8_B); break;
    case 0x71: cpu_ld_hl_r8(cpu, R8_C); break;
    case 0x72: cpu_ld_hl_r8(cpu, R8_D); break;
    case 0x73: cpu_ld_hl_r8(cpu, R8_E); break;
    case 0x74: cpu_ld_hl_r8(cpu, R8_H); break;
    case 0x75: cpu_ld_hl_r8(cpu, R8_L); break;
    case 0x76: fprintf(stderr, "Unhandled opcode: 0x76\n"); break;
    case 0x77: cpu_ld_hl_r8(cpu, R8_A); break;
    case 0x78: cpu_ld_r8_r8(cpu, R8_A, R8_B); break;
    case 0x79: cpu_ld_r8_r8(cpu, R8_A, R8_C); break;
    case 0x7a: cpu_ld_r8_r8(cpu, R8_A, R8_D); break;
    case 0x7b: cpu_ld_r8_r8(cpu, R8_A, R8_E); break;
    case 0x7c: cpu_ld_r8_r8(cpu, R8_A, R8_H); break;
    case 0x7d: cpu_ld_r8_r8(cpu, R8_A, R8_L); break;
    case 0x7e: cpu_ld_r8_hl(cpu, R8_A); break;
    case 0x7f: cpu_ld_r8_r8(cpu, R8_A, R8_A); break;

    case 0x80: fprintf(stderr, "Unhandled opcode: 0x80\n"); break;
    case 0x81: fprintf(stderr, "Unhandled opcode: 0x81\n"); break;
    case 0x82: fprintf(stderr, "Unhandled opcode: 0x82\n"); break;
    case 0x83: fprintf(stderr, "Unhandled opcode: 0x83\n"); break;
    case 0x84: fprintf(stderr, "Unhandled opcode: 0x84\n"); break;
    case 0x85: fprintf(stderr, "Unhandled opcode: 0x85\n"); break;
    case 0x86: fprintf(stderr, "Unhandled opcode: 0x86\n"); break;
    case 0x87: fprintf(stderr, "Unhandled opcode: 0x87\n"); break;
    case 0x88: fprintf(stderr, "Unhandled opcode: 0x88\n"); break;
    case 0x89: fprintf(stderr, "Unhandled opcode: 0x89\n"); break;
    case 0x8a: fprintf(stderr, "Unhandled opcode: 0x8a\n"); break;
    case 0x8b: fprintf(stderr, "Unhandled opcode: 0x8b\n"); break;
    case 0x8c: fprintf(stderr, "Unhandled opcode: 0x8c\n"); break;
    case 0x8d: fprintf(stderr, "Unhandled opcode: 0x8d\n"); break;
    case 0x8e: fprintf(stderr, "Unhandled opcode: 0x8e\n"); break;
    case 0x8f: fprintf(stderr, "Unhandled opcode: 0x8f\n"); break;

    case 0x90: fprintf(stderr, "Unhandled opcode: 0x90\n"); break;
    case 0x91: fprintf(stderr, "Unhandled opcode: 0x91\n"); break;
    case 0x92: fprintf(stderr, "Unhandled opcode: 0x92\n"); break;
    case 0x93: fprintf(stderr, "Unhandled opcode: 0x93\n"); break;
    case 0x94: fprintf(stderr, "Unhandled opcode: 0x94\n"); break;
    case 0x95: fprintf(stderr, "Unhandled opcode: 0x95\n"); break;
    case 0x96: fprintf(stderr, "Unhandled opcode: 0x96\n"); break;
    case 0x97: fprintf(stderr, "Unhandled opcode: 0x97\n"); break;
    case 0x98: fprintf(stderr, "Unhandled opcode: 0x98\n"); break;
    case 0x99: fprintf(stderr, "Unhandled opcode: 0x99\n"); break;
    case 0x9a: fprintf(stderr, "Unhandled opcode: 0x9a\n"); break;
    case 0x9b: fprintf(stderr, "Unhandled opcode: 0x9b\n"); break;
    case 0x9c: fprintf(stderr, "Unhandled opcode: 0x9c\n"); break;
    case 0x9d: fprintf(stderr, "Unhandled opcode: 0x9d\n"); break;
    case 0x9e: fprintf(stderr, "Unhandled opcode: 0x9e\n"); break;
    case 0x9f: fprintf(stderr, "Unhandled opcode: 0x9f\n"); break;

    case 0xa0: fprintf(stderr, "Unhandled opcode: 0xa0\n"); break;
    case 0xa1: fprintf(stderr, "Unhandled opcode: 0xa1\n"); break;
    case 0xa2: fprintf(stderr, "Unhandled opcode: 0xa2\n"); break;
    case 0xa3: fprintf(stderr, "Unhandled opcode: 0xa3\n"); break;
    case 0xa4: fprintf(stderr, "Unhandled opcode: 0xa4\n"); break;
    case 0xa5: fprintf(stderr, "Unhandled opcode: 0xa5\n"); break;
    case 0xa6: fprintf(stderr, "Unhandled opcode: 0xa6\n"); break;
    case 0xa7: fprintf(stderr, "Unhandled opcode: 0xa7\n"); break;
    case 0xa8: fprintf(stderr, "Unhandled opcode: 0xa8\n"); break;
    case 0xa9: fprintf(stderr, "Unhandled opcode: 0xa9\n"); break;
    case 0xaa: fprintf(stderr, "Unhandled opcode: 0xaa\n"); break;
    case 0xab: fprintf(stderr, "Unhandled opcode: 0xab\n"); break;
    case 0xac: fprintf(stderr, "Unhandled opcode: 0xac\n"); break;
    case 0xad: fprintf(stderr, "Unhandled opcode: 0xad\n"); break;
    case 0xae: fprintf(stderr, "Unhandled opcode: 0xae\n"); break;
    case 0xaf: fprintf(stderr, "Unhandled opcode: 0xaf\n"); break;

    case 0xb0: fprintf(stderr, "Unhandled opcode: 0xb0\n"); break;
    case 0xb1: fprintf(stderr, "Unhandled opcode: 0xb1\n"); break;
    case 0xb2: fprintf(stderr, "Unhandled opcode: 0xb2\n"); break;
    case 0xb3: fprintf(stderr, "Unhandled opcode: 0xb3\n"); break;
    case 0xb4: fprintf(stderr, "Unhandled opcode: 0xb4\n"); break;
    case 0xb5: fprintf(stderr, "Unhandled opcode: 0xb5\n"); break;
    case 0xb6: fprintf(stderr, "Unhandled opcode: 0xb6\n"); break;
    case 0xb7: fprintf(stderr, "Unhandled opcode: 0xb7\n"); break;
    case 0xb8: fprintf(stderr, "Unhandled opcode: 0xb8\n"); break;
    case 0xb9: fprintf(stderr, "Unhandled opcode: 0xb9\n"); break;
    case 0xba: fprintf(stderr, "Unhandled opcode: 0xba\n"); break;
    case 0xbb: fprintf(stderr, "Unhandled opcode: 0xbb\n"); break;
    case 0xbc: fprintf(stderr, "Unhandled opcode: 0xbc\n"); break;
    case 0xbd: fprintf(stderr, "Unhandled opcode: 0xbd\n"); break;
    case 0xbe: fprintf(stderr, "Unhandled opcode: 0xbe\n"); break;
    case 0xbf: fprintf(stderr, "Unhandled opcode: 0xbf\n"); break;

    case 0xc0: fprintf(stderr, "Unhandled opcode: 0xc0\n"); break;
    case 0xc1: fprintf(stderr, "Unhandled opcode: 0xc1\n"); break;
    case 0xc2: fprintf(stderr, "Unhandled opcode: 0xc2\n"); break;
    case 0xc3: fprintf(stderr, "Unhandled opcode: 0xc3\n"); break;
    case 0xc4: fprintf(stderr, "Unhandled opcode: 0xc4\n"); break;
    case 0xc5: fprintf(stderr, "Unhandled opcode: 0xc5\n"); break;
    case 0xc6: fprintf(stderr, "Unhandled opcode: 0xc6\n"); break;
    case 0xc7: fprintf(stderr, "Unhandled opcode: 0xc7\n"); break;
    case 0xc8: fprintf(stderr, "Unhandled opcode: 0xc8\n"); break;
    case 0xc9: fprintf(stderr, "Unhandled opcode: 0xc9\n"); break;
    case 0xca: fprintf(stderr, "Unhandled opcode: 0xca\n"); break;
    case 0xcb: cpu_execute_cb_opcode(cpu); break;
    case 0xcc: fprintf(stderr, "Unhandled opcode: 0xcc\n"); break;
    case 0xcd: fprintf(stderr, "Unhandled opcode: 0xcd\n"); break;
    case 0xce: fprintf(stderr, "Unhandled opcode: 0xce\n"); break;
    case 0xcf: fprintf(stderr, "Unhandled opcode: 0xcf\n"); break;

    case 0xd0: fprintf(stderr, "Unhandled opcode: 0xd0\n"); break;
    case 0xd1: fprintf(stderr, "Unhandled opcode: 0xd1\n"); break;
    case 0xd2: fprintf(stderr, "Unhandled opcode: 0xd2\n"); break;
    case 0xd3: fprintf(stderr, "Unhandled opcode: 0xd3\n"); break;
    case 0xd4: fprintf(stderr, "Unhandled opcode: 0xd4\n"); break;
    case 0xd5: fprintf(stderr, "Unhandled opcode: 0xd5\n"); break;
    case 0xd6: fprintf(stderr, "Unhandled opcode: 0xd6\n"); break;
    case 0xd7: fprintf(stderr, "Unhandled opcode: 0xd7\n"); break;
    case 0xd8: fprintf(stderr, "Unhandled opcode: 0xd8\n"); break;
    case 0xd9: fprintf(stderr, "Unhandled opcode: 0xd9\n"); break;
    case 0xda: fprintf(stderr, "Unhandled opcode: 0xda\n"); break;
    case 0xdb: fprintf(stderr, "Unhandled opcode: 0xdb\n"); break;
    case 0xdc: fprintf(stderr, "Unhandled opcode: 0xdc\n"); break;
    case 0xdd: fprintf(stderr, "Unhandled opcode: 0xdd\n"); break;
    case 0xde: fprintf(stderr, "Unhandled opcode: 0xde\n"); break;
    case 0xdf: fprintf(stderr, "Unhandled opcode: 0xdf\n"); break;

    case 0xe0: cpu_ldh_n8_a(cpu); break;
    case 0xe1: fprintf(stderr, "Unhandled opcode: 0xe1\n"); break;
    case 0xe2: cpu_ldh_c_a(cpu); break;
    case 0xe3: fprintf(stderr, "Unhandled opcode: 0xe3\n"); break;
    case 0xe4: fprintf(stderr, "Unhandled opcode: 0xe4\n"); break;
    case 0xe5: fprintf(stderr, "Unhandled opcode: 0xe5\n"); break;
    case 0xe6: fprintf(stderr, "Unhandled opcode: 0xe6\n"); break;
    case 0xe7: fprintf(stderr, "Unhandled opcode: 0xe7\n"); break;
    case 0xe8: fprintf(stderr, "Unhandled opcode: 0xe8\n"); break;
    case 0xe9: fprintf(stderr, "Unhandled opcode: 0xe9\n"); break;
    case 0xea: cpu_ld_n16_a(cpu); break;
    case 0xeb: fprintf(stderr, "Unhandled opcode: 0xeb\n"); break;
    case 0xec: fprintf(stderr, "Unhandled opcode: 0xec\n"); break;
    case 0xed: fprintf(stderr, "Unhandled opcode: 0xed\n"); break;
    case 0xee: fprintf(stderr, "Unhandled opcode: 0xee\n"); break;
    case 0xef: fprintf(stderr, "Unhandled opcode: 0xef\n"); break;

    case 0xf0: cpu_ldh_a_n8(cpu); break;
    case 0xf1: fprintf(stderr, "Unhandled opcode: 0xf1\n"); break;
    case 0xf2: cpu_ldh_a_c(cpu); break;
    case 0xf3: fprintf(stderr, "Unhandled opcode: 0xf3\n"); break;
    case 0xf4: fprintf(stderr, "Unhandled opcode: 0xf4\n"); break;
    case 0xf5: fprintf(stderr, "Unhandled opcode: 0xf5\n"); break;
    case 0xf6: fprintf(stderr, "Unhandled opcode: 0xf6\n"); break;
    case 0xf7: fprintf(stderr, "Unhandled opcode: 0xf7\n"); break;
    case 0xf8: fprintf(stderr, "Unhandled opcode: 0xf8\n"); break;
    case 0xf9: fprintf(stderr, "Unhandled opcode: 0xf9\n"); break;
    case 0xfa: cpu_ld_a_n16(cpu); break;
    case 0xfb: fprintf(stderr, "Unhandled opcode: 0xfb\n"); break;
    case 0xfc: fprintf(stderr, "Unhandled opcode: 0xfc\n"); break;
    case 0xfd: fprintf(stderr, "Unhandled opcode: 0xfd\n"); break;
    case 0xfe: fprintf(stderr, "Unhandled opcode: 0xfe\n"); break;
    case 0xff: fprintf(stderr, "Unhandled opcode: 0xff\n"); break;

    default: break;
    }
}

void cpu_tick(struct Cpu *cpu) { cpu_execute_opcode(cpu); }
