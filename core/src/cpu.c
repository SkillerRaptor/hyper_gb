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
    case 0x00: break;
    case 0x01: break;
    case 0x02: break;
    case 0x03: break;
    case 0x04: break;
    case 0x05: break;
    case 0x06: break;
    case 0x07: break;
    case 0x08: break;
    case 0x09: break;
    case 0x0a: break;
    case 0x0b: break;
    case 0x0c: break;
    case 0x0d: break;
    case 0x0e: break;
    case 0x0f: break;

    case 0x10: break;
    case 0x11: break;
    case 0x12: break;
    case 0x13: break;
    case 0x14: break;
    case 0x15: break;
    case 0x16: break;
    case 0x17: break;
    case 0x18: break;
    case 0x19: break;
    case 0x1a: break;
    case 0x1b: break;
    case 0x1c: break;
    case 0x1d: break;
    case 0x1e: break;
    case 0x1f: break;

    case 0x20: break;
    case 0x21: break;
    case 0x22: break;
    case 0x23: break;
    case 0x24: break;
    case 0x25: break;
    case 0x26: break;
    case 0x27: break;
    case 0x28: break;
    case 0x29: break;
    case 0x2a: break;
    case 0x2b: break;
    case 0x2c: break;
    case 0x2d: break;
    case 0x2e: break;
    case 0x2f: break;

    case 0x30: break;
    case 0x31: break;
    case 0x32: break;
    case 0x33: break;
    case 0x34: break;
    case 0x35: break;
    case 0x36: break;
    case 0x37: break;
    case 0x38: break;
    case 0x39: break;
    case 0x3a: break;
    case 0x3b: break;
    case 0x3c: break;
    case 0x3d: break;
    case 0x3e: break;
    case 0x3f: break;

    case 0x40: break;
    case 0x41: break;
    case 0x42: break;
    case 0x43: break;
    case 0x44: break;
    case 0x45: break;
    case 0x46: break;
    case 0x47: break;
    case 0x48: break;
    case 0x49: break;
    case 0x4a: break;
    case 0x4b: break;
    case 0x4c: break;
    case 0x4d: break;
    case 0x4e: break;
    case 0x4f: break;

    case 0x50: break;
    case 0x51: break;
    case 0x52: break;
    case 0x53: break;
    case 0x54: break;
    case 0x55: break;
    case 0x56: break;
    case 0x57: break;
    case 0x58: break;
    case 0x59: break;
    case 0x5a: break;
    case 0x5b: break;
    case 0x5c: break;
    case 0x5d: break;
    case 0x5e: break;
    case 0x5f: break;

    case 0x60: break;
    case 0x61: break;
    case 0x62: break;
    case 0x63: break;
    case 0x64: break;
    case 0x65: break;
    case 0x66: break;
    case 0x67: break;
    case 0x68: break;
    case 0x69: break;
    case 0x6a: break;
    case 0x6b: break;
    case 0x6c: break;
    case 0x6d: break;
    case 0x6e: break;
    case 0x6f: break;

    case 0x70: break;
    case 0x71: break;
    case 0x72: break;
    case 0x73: break;
    case 0x74: break;
    case 0x75: break;
    case 0x76: break;
    case 0x77: break;
    case 0x78: break;
    case 0x79: break;
    case 0x7a: break;
    case 0x7b: break;
    case 0x7c: break;
    case 0x7d: break;
    case 0x7e: break;
    case 0x7f: break;

    case 0x80: break;
    case 0x81: break;
    case 0x82: break;
    case 0x83: break;
    case 0x84: break;
    case 0x85: break;
    case 0x86: break;
    case 0x87: break;
    case 0x88: break;
    case 0x89: break;
    case 0x8a: break;
    case 0x8b: break;
    case 0x8c: break;
    case 0x8d: break;
    case 0x8e: break;
    case 0x8f: break;

    case 0x90: break;
    case 0x91: break;
    case 0x92: break;
    case 0x93: break;
    case 0x94: break;
    case 0x95: break;
    case 0x96: break;
    case 0x97: break;
    case 0x98: break;
    case 0x99: break;
    case 0x9a: break;
    case 0x9b: break;
    case 0x9c: break;
    case 0x9d: break;
    case 0x9e: break;
    case 0x9f: break;

    case 0xa0: break;
    case 0xa1: break;
    case 0xa2: break;
    case 0xa3: break;
    case 0xa4: break;
    case 0xa5: break;
    case 0xa6: break;
    case 0xa7: break;
    case 0xa8: break;
    case 0xa9: break;
    case 0xaa: break;
    case 0xab: break;
    case 0xac: break;
    case 0xad: break;
    case 0xae: break;
    case 0xaf: break;

    case 0xb0: break;
    case 0xb1: break;
    case 0xb2: break;
    case 0xb3: break;
    case 0xb4: break;
    case 0xb5: break;
    case 0xb6: break;
    case 0xb7: break;
    case 0xb8: break;
    case 0xb9: break;
    case 0xba: break;
    case 0xbb: break;
    case 0xbc: break;
    case 0xbd: break;
    case 0xbe: break;
    case 0xbf: break;

    case 0xc0: break;
    case 0xc1: break;
    case 0xc2: break;
    case 0xc3: break;
    case 0xc4: break;
    case 0xc5: break;
    case 0xc6: break;
    case 0xc7: break;
    case 0xc8: break;
    case 0xc9: break;
    case 0xca: break;
    case 0xcb: break;
    case 0xcc: break;
    case 0xcd: break;
    case 0xce: break;
    case 0xcf: break;

    case 0xd0: break;
    case 0xd1: break;
    case 0xd2: break;
    case 0xd3: break;
    case 0xd4: break;
    case 0xd5: break;
    case 0xd6: break;
    case 0xd7: break;
    case 0xd8: break;
    case 0xd9: break;
    case 0xda: break;
    case 0xdb: break;
    case 0xdc: break;
    case 0xdd: break;
    case 0xde: break;
    case 0xdf: break;

    case 0xe0: break;
    case 0xe1: break;
    case 0xe2: break;
    case 0xe3: break;
    case 0xe4: break;
    case 0xe5: break;
    case 0xe6: break;
    case 0xe7: break;
    case 0xe8: break;
    case 0xe9: break;
    case 0xea: break;
    case 0xeb: break;
    case 0xec: break;
    case 0xed: break;
    case 0xee: break;
    case 0xef: break;

    case 0xf0: break;
    case 0xf1: break;
    case 0xf2: break;
    case 0xf3: break;
    case 0xf4: break;
    case 0xf5: break;
    case 0xf6: break;
    case 0xf7: break;
    case 0xf8: break;
    case 0xf9: break;
    case 0xfa: break;
    case 0xfb: break;
    case 0xfc: break;
    case 0xfd: break;
    case 0xfe: break;
    case 0xff: break;

    default: break;
    }
}

static void cpu_execute_opcode(struct Cpu *cpu)
{
    const u8 opcode = cpu_fetch_u8(cpu);

    switch (opcode)
    {
    case 0x00: break;
    case 0x01: cpu_ld_r16_n16(cpu, R16_BC); break;
    case 0x02: cpu_ld_r16_a(cpu, R16_BC); break;
    case 0x03: cpu_inc_r16(cpu, R16_BC); break;
    case 0x04: cpu_inc_r8(cpu, R8_B); break;
    case 0x05: cpu_dec_r8(cpu, R8_B); break;
    case 0x06: cpu_ld_r8_n8(cpu, R8_B); break;
    case 0x07: break;
    case 0x08: break;
    case 0x09: cpu_add_hl_r16(cpu, R16_BC); break;
    case 0x0a: cpu_ld_a_r16(cpu, R16_BC); break;
    case 0x0b: cpu_dec_r16(cpu, R16_BC); break;
    case 0x0c: cpu_inc_r8(cpu, R8_C); break;
    case 0x0d: cpu_dec_r8(cpu, R8_C); break;
    case 0x0e: cpu_ld_r8_n8(cpu, R8_C); break;
    case 0x0f: break;

    case 0x10: break;
    case 0x11: cpu_ld_r16_n16(cpu, R16_DE); break;
    case 0x12: cpu_ld_r16_a(cpu, R16_DE); break;
    case 0x13: cpu_inc_r16(cpu, R16_DE); break;
    case 0x14: cpu_inc_r8(cpu, R8_D); break;
    case 0x15: cpu_dec_r8(cpu, R8_D); break;
    case 0x16: cpu_ld_r8_n8(cpu, R8_D); break;
    case 0x17: break;
    case 0x18: break;
    case 0x19: cpu_add_hl_r16(cpu, R16_DE); break;
    case 0x1a: cpu_ld_a_r16(cpu, R16_DE); break;
    case 0x1b: cpu_dec_r16(cpu, R16_DE); break;
    case 0x1c: cpu_inc_r8(cpu, R8_E); break;
    case 0x1d: cpu_dec_r8(cpu, R8_E); break;
    case 0x1e: cpu_ld_r8_n8(cpu, R8_E); break;
    case 0x1f: break;

    case 0x20: break;
    case 0x21: cpu_ld_r16_n16(cpu, R16_HL); break;
    case 0x22: cpu_ld_hli_a(cpu); break;
    case 0x23: cpu_inc_r16(cpu, R16_HL); break;
    case 0x24: cpu_inc_r8(cpu, R8_H); break;
    case 0x25: cpu_dec_r8(cpu, R8_H); break;
    case 0x26: cpu_ld_r8_n8(cpu, R8_H); break;
    case 0x27: break;
    case 0x28: break;
    case 0x29: cpu_add_hl_r16(cpu, R16_HL); break;
    case 0x2a: cpu_ld_a_hli(cpu); break;
    case 0x2b: cpu_dec_r16(cpu, R16_HL); break;
    case 0x2c: cpu_inc_r8(cpu, R8_L); break;
    case 0x2d: cpu_dec_r8(cpu, R8_L); break;
    case 0x2e: cpu_ld_r8_n8(cpu, R8_L); break;
    case 0x2f: cpu_cpl(cpu); break;

    case 0x30: break;
    case 0x31: break;
    case 0x32: cpu_ld_hld_a(cpu); break;
    case 0x33: break;
    case 0x34: cpu_inc_hl(cpu); break;
    case 0x35: cpu_dec_hl(cpu); break;
    case 0x36: cpu_ld_hl_n8(cpu); break;
    case 0x37: break;
    case 0x38: break;
    case 0x39: break;
    case 0x3a: cpu_ld_a_hld(cpu); break;
    case 0x3b: break;
    case 0x3c: cpu_inc_r8(cpu, R8_A); break;
    case 0x3d: cpu_dec_r8(cpu, R8_A); break;
    case 0x3e: cpu_ld_r8_n8(cpu, R8_A); break;
    case 0x3f: break;

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
    case 0x76: break;
    case 0x77: cpu_ld_hl_r8(cpu, R8_A); break;
    case 0x78: cpu_ld_r8_r8(cpu, R8_A, R8_B); break;
    case 0x79: cpu_ld_r8_r8(cpu, R8_A, R8_C); break;
    case 0x7a: cpu_ld_r8_r8(cpu, R8_A, R8_D); break;
    case 0x7b: cpu_ld_r8_r8(cpu, R8_A, R8_E); break;
    case 0x7c: cpu_ld_r8_r8(cpu, R8_A, R8_H); break;
    case 0x7d: cpu_ld_r8_r8(cpu, R8_A, R8_L); break;
    case 0x7e: cpu_ld_r8_hl(cpu, R8_A); break;
    case 0x7f: cpu_ld_r8_r8(cpu, R8_A, R8_A); break;

    case 0x80: cpu_add_a_r8(cpu, R8_B); break;
    case 0x81: cpu_add_a_r8(cpu, R8_C); break;
    case 0x82: cpu_add_a_r8(cpu, R8_D); break;
    case 0x83: cpu_add_a_r8(cpu, R8_E); break;
    case 0x84: cpu_add_a_r8(cpu, R8_H); break;
    case 0x85: cpu_add_a_r8(cpu, R8_L); break;
    case 0x86: cpu_add_a_hl(cpu); break;
    case 0x87: cpu_add_a_r8(cpu, R8_A); break;
    case 0x88: cpu_adc_a_r8(cpu, R8_B); break;
    case 0x89: cpu_adc_a_r8(cpu, R8_C); break;
    case 0x8a: cpu_adc_a_r8(cpu, R8_D); break;
    case 0x8b: cpu_adc_a_r8(cpu, R8_E); break;
    case 0x8c: cpu_adc_a_r8(cpu, R8_H); break;
    case 0x8d: cpu_adc_a_r8(cpu, R8_L); break;
    case 0x8e: cpu_adc_a_hl(cpu); break;
    case 0x8f: cpu_adc_a_r8(cpu, R8_A); break;

    case 0x90: cpu_sub_a_r8(cpu, R8_B); break;
    case 0x91: cpu_sub_a_r8(cpu, R8_C); break;
    case 0x92: cpu_sub_a_r8(cpu, R8_D); break;
    case 0x93: cpu_sub_a_r8(cpu, R8_E); break;
    case 0x94: cpu_sub_a_r8(cpu, R8_H); break;
    case 0x95: cpu_sub_a_r8(cpu, R8_L); break;
    case 0x96: cpu_sub_a_hl(cpu); break;
    case 0x97: cpu_sub_a_r8(cpu, R8_A); break;
    case 0x98: cpu_sbc_a_r8(cpu, R8_B); break;
    case 0x99: cpu_sbc_a_r8(cpu, R8_C); break;
    case 0x9a: cpu_sbc_a_r8(cpu, R8_D); break;
    case 0x9b: cpu_sbc_a_r8(cpu, R8_E); break;
    case 0x9c: cpu_sbc_a_r8(cpu, R8_H); break;
    case 0x9d: cpu_sbc_a_r8(cpu, R8_L); break;
    case 0x9e: cpu_sbc_a_hl(cpu); break;
    case 0x9f: cpu_sbc_a_r8(cpu, R8_A); break;

    case 0xa0: cpu_and_a_r8(cpu, R8_B); break;
    case 0xa1: cpu_and_a_r8(cpu, R8_C); break;
    case 0xa2: cpu_and_a_r8(cpu, R8_D); break;
    case 0xa3: cpu_and_a_r8(cpu, R8_E); break;
    case 0xa4: cpu_and_a_r8(cpu, R8_H); break;
    case 0xa5: cpu_and_a_r8(cpu, R8_L); break;
    case 0xa6: cpu_and_a_hl(cpu); break;
    case 0xa7: cpu_and_a_r8(cpu, R8_A); break;
    case 0xa8: cpu_xor_a_r8(cpu, R8_B); break;
    case 0xa9: cpu_xor_a_r8(cpu, R8_C); break;
    case 0xaa: cpu_xor_a_r8(cpu, R8_D); break;
    case 0xab: cpu_xor_a_r8(cpu, R8_E); break;
    case 0xac: cpu_xor_a_r8(cpu, R8_H); break;
    case 0xad: cpu_xor_a_r8(cpu, R8_L); break;
    case 0xae: cpu_xor_a_hl(cpu); break;
    case 0xaf: cpu_xor_a_r8(cpu, R8_A); break;

    case 0xb0: cpu_or_a_r8(cpu, R8_B); break;
    case 0xb1: cpu_or_a_r8(cpu, R8_C); break;
    case 0xb2: cpu_or_a_r8(cpu, R8_D); break;
    case 0xb3: cpu_or_a_r8(cpu, R8_E); break;
    case 0xb4: cpu_or_a_r8(cpu, R8_H); break;
    case 0xb5: cpu_or_a_r8(cpu, R8_L); break;
    case 0xb6: cpu_or_a_hl(cpu); break;
    case 0xb7: cpu_or_a_r8(cpu, R8_A); break;
    case 0xb8: cpu_cp_a_r8(cpu, R8_B); break;
    case 0xb9: cpu_cp_a_r8(cpu, R8_C); break;
    case 0xba: cpu_cp_a_r8(cpu, R8_D); break;
    case 0xbb: cpu_cp_a_r8(cpu, R8_E); break;
    case 0xbc: cpu_cp_a_r8(cpu, R8_H); break;
    case 0xbd: cpu_cp_a_r8(cpu, R8_L); break;
    case 0xbe: cpu_cp_a_hl(cpu); break;
    case 0xbf: cpu_cp_a_r8(cpu, R8_A); break;

    case 0xc0: break;
    case 0xc1: break;
    case 0xc2: break;
    case 0xc3: break;
    case 0xc4: break;
    case 0xc5: break;
    case 0xc6: cpu_add_a_n8(cpu); break;
    case 0xc7: break;
    case 0xc8: break;
    case 0xc9: break;
    case 0xca: break;
    case 0xcb: cpu_execute_cb_opcode(cpu); break;
    case 0xcc: break;
    case 0xcd: break;
    case 0xce: cpu_adc_a_n8(cpu); break;
    case 0xcf: break;

    case 0xd0: break;
    case 0xd1: break;
    case 0xd2: break;
    case 0xd3: break;
    case 0xd4: break;
    case 0xd5: break;
    case 0xd6: cpu_sub_a_n8(cpu); break;
    case 0xd7: break;
    case 0xd8: break;
    case 0xd9: break;
    case 0xda: break;
    case 0xdb: break;
    case 0xdc: break;
    case 0xdd: break;
    case 0xde: cpu_sbc_a_n8(cpu); break;
    case 0xdf: break;

    case 0xe0: cpu_ldh_n8_a(cpu); break;
    case 0xe1: break;
    case 0xe2: cpu_ldh_c_a(cpu); break;
    case 0xe3: break;
    case 0xe4: break;
    case 0xe5: break;
    case 0xe6: break;
    case 0xe7: break;
    case 0xe8: break;
    case 0xe9: break;
    case 0xea: cpu_ld_n16_a(cpu); break;
    case 0xeb: break;
    case 0xec: break;
    case 0xed: break;
    case 0xee: break;
    case 0xef: break;

    case 0xf0: cpu_ldh_a_n8(cpu); break;
    case 0xf1: break;
    case 0xf2: cpu_ldh_a_c(cpu); break;
    case 0xf3: break;
    case 0xf4: break;
    case 0xf5: break;
    case 0xf6: break;
    case 0xf7: break;
    case 0xf8: break;
    case 0xf9: break;
    case 0xfa: cpu_ld_a_n16(cpu); break;
    case 0xfb: break;
    case 0xfc: break;
    case 0xfd: break;
    case 0xfe: cpu_cp_a_n8(cpu); break;
    case 0xff: break;

    default: break;
    }
}

void cpu_tick(struct Cpu *cpu) { cpu_execute_opcode(cpu); }
