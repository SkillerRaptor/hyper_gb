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

bool cpu_is_condition(struct Cpu *cpu, const enum ConditionCode cc)
{
    switch (cc)
    {
    case CC_Z: return cpu_is_flag(cpu, FLAG_Z);
    case CC_NZ: return !cpu_is_flag(cpu, FLAG_Z);
    case CC_C: return cpu_is_flag(cpu, FLAG_C);
    case CC_NC: return !cpu_is_flag(cpu, FLAG_C);
    default: return false;
    }
}

void cpu_push_stack(struct Cpu *cpu, const u16 value)
{
    cpu->registers.sp -= 1;
    mmu_write(cpu->mmu, cpu->registers.sp, (u8) ((value & 0xff00) >> 8));
    cpu->registers.sp -= 1;
    mmu_write(cpu->mmu, cpu->registers.sp, (u8) ((value & 0x00ff) >> 0));
}

u16 cpu_pop_stack(struct Cpu *cpu)
{
    const u8 lower_byte = mmu_read(cpu->mmu, cpu->registers.sp);
    cpu->registers.sp += 1;

    const u8 higher_byte = mmu_read(cpu->mmu, cpu->registers.sp);
    cpu->registers.sp += 1;

    const u16 result = ((u16) higher_byte << 8) | (u16) lower_byte;
    return result;
}

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
    case 0x00: cpu_rlc_r8(cpu, R8_B); break;
    case 0x01: cpu_rlc_r8(cpu, R8_C); break;
    case 0x02: cpu_rlc_r8(cpu, R8_D); break;
    case 0x03: cpu_rlc_r8(cpu, R8_E); break;
    case 0x04: cpu_rlc_r8(cpu, R8_H); break;
    case 0x05: cpu_rlc_r8(cpu, R8_L); break;
    case 0x06: cpu_rlc_hl(cpu); break;
    case 0x07: cpu_rlc_r8(cpu, R8_A); break;
    case 0x08: cpu_rrc_r8(cpu, R8_B); break;
    case 0x09: cpu_rrc_r8(cpu, R8_C); break;
    case 0x0a: cpu_rrc_r8(cpu, R8_D); break;
    case 0x0b: cpu_rrc_r8(cpu, R8_E); break;
    case 0x0c: cpu_rrc_r8(cpu, R8_H); break;
    case 0x0d: cpu_rrc_r8(cpu, R8_L); break;
    case 0x0e: cpu_rrc_hl(cpu); break;
    case 0x0f: cpu_rrc_r8(cpu, R8_A); break;

    case 0x10: cpu_rl_r8(cpu, R8_B); break;
    case 0x11: cpu_rl_r8(cpu, R8_C); break;
    case 0x12: cpu_rl_r8(cpu, R8_D); break;
    case 0x13: cpu_rl_r8(cpu, R8_E); break;
    case 0x14: cpu_rl_r8(cpu, R8_H); break;
    case 0x15: cpu_rl_r8(cpu, R8_L); break;
    case 0x16: cpu_rl_hl(cpu); break;
    case 0x17: cpu_rl_r8(cpu, R8_A); break;
    case 0x18: cpu_rr_r8(cpu, R8_B); break;
    case 0x19: cpu_rr_r8(cpu, R8_C); break;
    case 0x1a: cpu_rr_r8(cpu, R8_D); break;
    case 0x1b: cpu_rr_r8(cpu, R8_E); break;
    case 0x1c: cpu_rr_r8(cpu, R8_H); break;
    case 0x1d: cpu_rr_r8(cpu, R8_L); break;
    case 0x1e: cpu_rr_hl(cpu); break;
    case 0x1f: cpu_rr_r8(cpu, R8_A); break;

    case 0x20: cpu_sla_r8(cpu, R8_B); break;
    case 0x21: cpu_sla_r8(cpu, R8_C); break;
    case 0x22: cpu_sla_r8(cpu, R8_D); break;
    case 0x23: cpu_sla_r8(cpu, R8_E); break;
    case 0x24: cpu_sla_r8(cpu, R8_H); break;
    case 0x25: cpu_sla_r8(cpu, R8_L); break;
    case 0x26: cpu_sla_hl(cpu); break;
    case 0x27: cpu_sla_r8(cpu, R8_A); break;
    case 0x28: cpu_sra_r8(cpu, R8_B); break;
    case 0x29: cpu_sra_r8(cpu, R8_C); break;
    case 0x2a: cpu_sra_r8(cpu, R8_D); break;
    case 0x2b: cpu_sra_r8(cpu, R8_E); break;
    case 0x2c: cpu_sra_r8(cpu, R8_H); break;
    case 0x2d: cpu_sra_r8(cpu, R8_L); break;
    case 0x2e: cpu_sra_hl(cpu); break;
    case 0x2f: cpu_sra_r8(cpu, R8_A); break;

    case 0x30: cpu_swap_r8(cpu, R8_B); break;
    case 0x31: cpu_swap_r8(cpu, R8_C); break;
    case 0x32: cpu_swap_r8(cpu, R8_D); break;
    case 0x33: cpu_swap_r8(cpu, R8_E); break;
    case 0x34: cpu_swap_r8(cpu, R8_H); break;
    case 0x35: cpu_swap_r8(cpu, R8_L); break;
    case 0x36: cpu_swap_hl(cpu); break;
    case 0x37: cpu_swap_r8(cpu, R8_A); break;
    case 0x38: cpu_srl_r8(cpu, R8_B); break;
    case 0x39: cpu_srl_r8(cpu, R8_C); break;
    case 0x3a: cpu_srl_r8(cpu, R8_D); break;
    case 0x3b: cpu_srl_r8(cpu, R8_E); break;
    case 0x3c: cpu_srl_r8(cpu, R8_H); break;
    case 0x3d: cpu_srl_r8(cpu, R8_L); break;
    case 0x3e: cpu_srl_hl(cpu); break;
    case 0x3f: cpu_srl_r8(cpu, R8_A); break;

    case 0x40: cpu_bit_u3_r8(cpu, 0, R8_B); break;
    case 0x41: cpu_bit_u3_r8(cpu, 0, R8_C); break;
    case 0x42: cpu_bit_u3_r8(cpu, 0, R8_D); break;
    case 0x43: cpu_bit_u3_r8(cpu, 0, R8_E); break;
    case 0x44: cpu_bit_u3_r8(cpu, 0, R8_H); break;
    case 0x45: cpu_bit_u3_r8(cpu, 0, R8_L); break;
    case 0x46: cpu_bit_u3_hl(cpu, 0); break;
    case 0x47: cpu_bit_u3_r8(cpu, 0, R8_A); break;
    case 0x48: cpu_bit_u3_r8(cpu, 1, R8_B); break;
    case 0x49: cpu_bit_u3_r8(cpu, 1, R8_C); break;
    case 0x4a: cpu_bit_u3_r8(cpu, 1, R8_D); break;
    case 0x4b: cpu_bit_u3_r8(cpu, 1, R8_E); break;
    case 0x4c: cpu_bit_u3_r8(cpu, 1, R8_H); break;
    case 0x4d: cpu_bit_u3_r8(cpu, 1, R8_L); break;
    case 0x4e: cpu_bit_u3_hl(cpu, 1); break;
    case 0x4f: cpu_bit_u3_r8(cpu, 1, R8_A); break;

    case 0x50: cpu_bit_u3_r8(cpu, 2, R8_B); break;
    case 0x51: cpu_bit_u3_r8(cpu, 2, R8_C); break;
    case 0x52: cpu_bit_u3_r8(cpu, 2, R8_D); break;
    case 0x53: cpu_bit_u3_r8(cpu, 2, R8_E); break;
    case 0x54: cpu_bit_u3_r8(cpu, 2, R8_H); break;
    case 0x55: cpu_bit_u3_r8(cpu, 2, R8_L); break;
    case 0x56: cpu_bit_u3_hl(cpu, 2); break;
    case 0x57: cpu_bit_u3_r8(cpu, 2, R8_A); break;
    case 0x58: cpu_bit_u3_r8(cpu, 3, R8_B); break;
    case 0x59: cpu_bit_u3_r8(cpu, 3, R8_C); break;
    case 0x5a: cpu_bit_u3_r8(cpu, 3, R8_D); break;
    case 0x5b: cpu_bit_u3_r8(cpu, 3, R8_E); break;
    case 0x5c: cpu_bit_u3_r8(cpu, 3, R8_H); break;
    case 0x5d: cpu_bit_u3_r8(cpu, 3, R8_L); break;
    case 0x5e: cpu_bit_u3_hl(cpu, 3); break;
    case 0x5f: cpu_bit_u3_r8(cpu, 3, R8_A); break;

    case 0x60: cpu_bit_u3_r8(cpu, 4, R8_B); break;
    case 0x61: cpu_bit_u3_r8(cpu, 4, R8_C); break;
    case 0x62: cpu_bit_u3_r8(cpu, 4, R8_D); break;
    case 0x63: cpu_bit_u3_r8(cpu, 4, R8_E); break;
    case 0x64: cpu_bit_u3_r8(cpu, 4, R8_H); break;
    case 0x65: cpu_bit_u3_r8(cpu, 4, R8_L); break;
    case 0x66: cpu_bit_u3_hl(cpu, 4); break;
    case 0x67: cpu_bit_u3_r8(cpu, 4, R8_A); break;
    case 0x68: cpu_bit_u3_r8(cpu, 5, R8_B); break;
    case 0x69: cpu_bit_u3_r8(cpu, 5, R8_C); break;
    case 0x6a: cpu_bit_u3_r8(cpu, 5, R8_D); break;
    case 0x6b: cpu_bit_u3_r8(cpu, 5, R8_E); break;
    case 0x6c: cpu_bit_u3_r8(cpu, 5, R8_H); break;
    case 0x6d: cpu_bit_u3_r8(cpu, 5, R8_L); break;
    case 0x6e: cpu_bit_u3_hl(cpu, 5); break;
    case 0x6f: cpu_bit_u3_r8(cpu, 5, R8_A); break;

    case 0x70: cpu_bit_u3_r8(cpu, 6, R8_B); break;
    case 0x71: cpu_bit_u3_r8(cpu, 6, R8_C); break;
    case 0x72: cpu_bit_u3_r8(cpu, 6, R8_D); break;
    case 0x73: cpu_bit_u3_r8(cpu, 6, R8_E); break;
    case 0x74: cpu_bit_u3_r8(cpu, 6, R8_H); break;
    case 0x75: cpu_bit_u3_r8(cpu, 6, R8_L); break;
    case 0x76: cpu_bit_u3_hl(cpu, 6); break;
    case 0x77: cpu_bit_u3_r8(cpu, 6, R8_A); break;
    case 0x78: cpu_bit_u3_r8(cpu, 7, R8_B); break;
    case 0x79: cpu_bit_u3_r8(cpu, 7, R8_C); break;
    case 0x7a: cpu_bit_u3_r8(cpu, 7, R8_D); break;
    case 0x7b: cpu_bit_u3_r8(cpu, 7, R8_E); break;
    case 0x7c: cpu_bit_u3_r8(cpu, 7, R8_H); break;
    case 0x7d: cpu_bit_u3_r8(cpu, 7, R8_L); break;
    case 0x7e: cpu_bit_u3_hl(cpu, 7); break;
    case 0x7f: cpu_bit_u3_r8(cpu, 7, R8_A); break;

    case 0x80: cpu_res_u3_r8(cpu, 0, R8_B); break;
    case 0x81: cpu_res_u3_r8(cpu, 0, R8_C); break;
    case 0x82: cpu_res_u3_r8(cpu, 0, R8_D); break;
    case 0x83: cpu_res_u3_r8(cpu, 0, R8_E); break;
    case 0x84: cpu_res_u3_r8(cpu, 0, R8_H); break;
    case 0x85: cpu_res_u3_r8(cpu, 0, R8_L); break;
    case 0x86: cpu_res_u3_hl(cpu, 0); break;
    case 0x87: cpu_res_u3_r8(cpu, 0, R8_A); break;
    case 0x88: cpu_res_u3_r8(cpu, 1, R8_B); break;
    case 0x89: cpu_res_u3_r8(cpu, 1, R8_C); break;
    case 0x8a: cpu_res_u3_r8(cpu, 1, R8_D); break;
    case 0x8b: cpu_res_u3_r8(cpu, 1, R8_E); break;
    case 0x8c: cpu_res_u3_r8(cpu, 1, R8_H); break;
    case 0x8d: cpu_res_u3_r8(cpu, 1, R8_L); break;
    case 0x8e: cpu_res_u3_hl(cpu, 1); break;
    case 0x8f: cpu_res_u3_r8(cpu, 1, R8_A); break;

    case 0x90: cpu_res_u3_r8(cpu, 2, R8_B); break;
    case 0x91: cpu_res_u3_r8(cpu, 2, R8_C); break;
    case 0x92: cpu_res_u3_r8(cpu, 2, R8_D); break;
    case 0x93: cpu_res_u3_r8(cpu, 2, R8_E); break;
    case 0x94: cpu_res_u3_r8(cpu, 2, R8_H); break;
    case 0x95: cpu_res_u3_r8(cpu, 2, R8_L); break;
    case 0x96: cpu_res_u3_hl(cpu, 2); break;
    case 0x97: cpu_res_u3_r8(cpu, 2, R8_A); break;
    case 0x98: cpu_res_u3_r8(cpu, 3, R8_B); break;
    case 0x99: cpu_res_u3_r8(cpu, 3, R8_C); break;
    case 0x9a: cpu_res_u3_r8(cpu, 3, R8_D); break;
    case 0x9b: cpu_res_u3_r8(cpu, 3, R8_E); break;
    case 0x9c: cpu_res_u3_r8(cpu, 3, R8_H); break;
    case 0x9d: cpu_res_u3_r8(cpu, 3, R8_L); break;
    case 0x9e: cpu_res_u3_hl(cpu, 3); break;
    case 0x9f: cpu_res_u3_r8(cpu, 3, R8_A); break;

    case 0xa0: cpu_res_u3_r8(cpu, 4, R8_B); break;
    case 0xa1: cpu_res_u3_r8(cpu, 4, R8_C); break;
    case 0xa2: cpu_res_u3_r8(cpu, 4, R8_D); break;
    case 0xa3: cpu_res_u3_r8(cpu, 4, R8_E); break;
    case 0xa4: cpu_res_u3_r8(cpu, 4, R8_H); break;
    case 0xa5: cpu_res_u3_r8(cpu, 4, R8_L); break;
    case 0xa6: cpu_res_u3_hl(cpu, 4); break;
    case 0xa7: cpu_res_u3_r8(cpu, 4, R8_A); break;
    case 0xa8: cpu_res_u3_r8(cpu, 5, R8_B); break;
    case 0xa9: cpu_res_u3_r8(cpu, 5, R8_C); break;
    case 0xaa: cpu_res_u3_r8(cpu, 5, R8_D); break;
    case 0xab: cpu_res_u3_r8(cpu, 5, R8_E); break;
    case 0xac: cpu_res_u3_r8(cpu, 5, R8_H); break;
    case 0xad: cpu_res_u3_r8(cpu, 5, R8_L); break;
    case 0xae: cpu_res_u3_hl(cpu, 5); break;
    case 0xaf: cpu_res_u3_r8(cpu, 5, R8_A); break;

    case 0xb0: cpu_res_u3_r8(cpu, 6, R8_B); break;
    case 0xb1: cpu_res_u3_r8(cpu, 6, R8_C); break;
    case 0xb2: cpu_res_u3_r8(cpu, 6, R8_D); break;
    case 0xb3: cpu_res_u3_r8(cpu, 6, R8_E); break;
    case 0xb4: cpu_res_u3_r8(cpu, 6, R8_H); break;
    case 0xb5: cpu_res_u3_r8(cpu, 6, R8_L); break;
    case 0xb6: cpu_res_u3_hl(cpu, 6); break;
    case 0xb7: cpu_res_u3_r8(cpu, 6, R8_A); break;
    case 0xb8: cpu_res_u3_r8(cpu, 7, R8_B); break;
    case 0xb9: cpu_res_u3_r8(cpu, 7, R8_C); break;
    case 0xba: cpu_res_u3_r8(cpu, 7, R8_D); break;
    case 0xbb: cpu_res_u3_r8(cpu, 7, R8_E); break;
    case 0xbc: cpu_res_u3_r8(cpu, 7, R8_H); break;
    case 0xbd: cpu_res_u3_r8(cpu, 7, R8_L); break;
    case 0xbe: cpu_res_u3_hl(cpu, 7); break;
    case 0xbf: cpu_res_u3_r8(cpu, 7, R8_A); break;

    case 0xc0: cpu_set_u3_r8(cpu, 0, R8_B); break;
    case 0xc1: cpu_set_u3_r8(cpu, 0, R8_C); break;
    case 0xc2: cpu_set_u3_r8(cpu, 0, R8_D); break;
    case 0xc3: cpu_set_u3_r8(cpu, 0, R8_E); break;
    case 0xc4: cpu_set_u3_r8(cpu, 0, R8_H); break;
    case 0xc5: cpu_set_u3_r8(cpu, 0, R8_L); break;
    case 0xc6: cpu_set_u3_hl(cpu, 0); break;
    case 0xc7: cpu_set_u3_r8(cpu, 0, R8_A); break;
    case 0xc8: cpu_set_u3_r8(cpu, 1, R8_B); break;
    case 0xc9: cpu_set_u3_r8(cpu, 1, R8_C); break;
    case 0xca: cpu_set_u3_r8(cpu, 1, R8_D); break;
    case 0xcb: cpu_set_u3_r8(cpu, 1, R8_E); break;
    case 0xcc: cpu_set_u3_r8(cpu, 1, R8_H); break;
    case 0xcd: cpu_set_u3_r8(cpu, 1, R8_L); break;
    case 0xce: cpu_set_u3_hl(cpu, 1); break;
    case 0xcf: cpu_set_u3_r8(cpu, 1, R8_A); break;

    case 0xd0: cpu_set_u3_r8(cpu, 2, R8_B); break;
    case 0xd1: cpu_set_u3_r8(cpu, 2, R8_C); break;
    case 0xd2: cpu_set_u3_r8(cpu, 2, R8_D); break;
    case 0xd3: cpu_set_u3_r8(cpu, 2, R8_E); break;
    case 0xd4: cpu_set_u3_r8(cpu, 2, R8_H); break;
    case 0xd5: cpu_set_u3_r8(cpu, 2, R8_L); break;
    case 0xd6: cpu_set_u3_hl(cpu, 2); break;
    case 0xd7: cpu_set_u3_r8(cpu, 2, R8_A); break;
    case 0xd8: cpu_set_u3_r8(cpu, 3, R8_B); break;
    case 0xd9: cpu_set_u3_r8(cpu, 3, R8_C); break;
    case 0xda: cpu_set_u3_r8(cpu, 3, R8_D); break;
    case 0xdb: cpu_set_u3_r8(cpu, 3, R8_E); break;
    case 0xdc: cpu_set_u3_r8(cpu, 3, R8_H); break;
    case 0xdd: cpu_set_u3_r8(cpu, 3, R8_L); break;
    case 0xde: cpu_set_u3_hl(cpu, 3); break;
    case 0xdf: cpu_set_u3_r8(cpu, 3, R8_A); break;

    case 0xe0: cpu_set_u3_r8(cpu, 4, R8_B); break;
    case 0xe1: cpu_set_u3_r8(cpu, 4, R8_C); break;
    case 0xe2: cpu_set_u3_r8(cpu, 4, R8_D); break;
    case 0xe3: cpu_set_u3_r8(cpu, 4, R8_E); break;
    case 0xe4: cpu_set_u3_r8(cpu, 4, R8_H); break;
    case 0xe5: cpu_set_u3_r8(cpu, 4, R8_L); break;
    case 0xe6: cpu_set_u3_hl(cpu, 4); break;
    case 0xe7: cpu_set_u3_r8(cpu, 4, R8_A); break;
    case 0xe8: cpu_set_u3_r8(cpu, 5, R8_B); break;
    case 0xe9: cpu_set_u3_r8(cpu, 5, R8_C); break;
    case 0xea: cpu_set_u3_r8(cpu, 5, R8_D); break;
    case 0xeb: cpu_set_u3_r8(cpu, 5, R8_E); break;
    case 0xec: cpu_set_u3_r8(cpu, 5, R8_H); break;
    case 0xed: cpu_set_u3_r8(cpu, 5, R8_L); break;
    case 0xee: cpu_set_u3_hl(cpu, 5); break;
    case 0xef: cpu_set_u3_r8(cpu, 5, R8_A); break;

    case 0xf0: cpu_set_u3_r8(cpu, 6, R8_B); break;
    case 0xf1: cpu_set_u3_r8(cpu, 6, R8_C); break;
    case 0xf2: cpu_set_u3_r8(cpu, 6, R8_D); break;
    case 0xf3: cpu_set_u3_r8(cpu, 6, R8_E); break;
    case 0xf4: cpu_set_u3_r8(cpu, 6, R8_H); break;
    case 0xf5: cpu_set_u3_r8(cpu, 6, R8_L); break;
    case 0xf6: cpu_set_u3_hl(cpu, 6); break;
    case 0xf7: cpu_set_u3_r8(cpu, 6, R8_A); break;
    case 0xf8: cpu_set_u3_r8(cpu, 7, R8_B); break;
    case 0xf9: cpu_set_u3_r8(cpu, 7, R8_C); break;
    case 0xfa: cpu_set_u3_r8(cpu, 7, R8_D); break;
    case 0xfb: cpu_set_u3_r8(cpu, 7, R8_E); break;
    case 0xfc: cpu_set_u3_r8(cpu, 7, R8_H); break;
    case 0xfd: cpu_set_u3_r8(cpu, 7, R8_L); break;
    case 0xfe: cpu_set_u3_hl(cpu, 7); break;
    case 0xff: cpu_set_u3_r8(cpu, 7, R8_A); break;

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
    case 0x07: cpu_rlca(cpu); break;
    case 0x08: cpu_ld_n16_sp(cpu); break;
    case 0x09: cpu_add_hl_r16(cpu, R16_BC); break;
    case 0x0a: cpu_ld_a_r16(cpu, R16_BC); break;
    case 0x0b: cpu_dec_r16(cpu, R16_BC); break;
    case 0x0c: cpu_inc_r8(cpu, R8_C); break;
    case 0x0d: cpu_dec_r8(cpu, R8_C); break;
    case 0x0e: cpu_ld_r8_n8(cpu, R8_C); break;
    case 0x0f: cpu_rrca(cpu); break;

    case 0x10: break;
    case 0x11: cpu_ld_r16_n16(cpu, R16_DE); break;
    case 0x12: cpu_ld_r16_a(cpu, R16_DE); break;
    case 0x13: cpu_inc_r16(cpu, R16_DE); break;
    case 0x14: cpu_inc_r8(cpu, R8_D); break;
    case 0x15: cpu_dec_r8(cpu, R8_D); break;
    case 0x16: cpu_ld_r8_n8(cpu, R8_D); break;
    case 0x17: cpu_rla(cpu); break;
    case 0x18: cpu_jr_i8(cpu); break;
    case 0x19: cpu_add_hl_r16(cpu, R16_DE); break;
    case 0x1a: cpu_ld_a_r16(cpu, R16_DE); break;
    case 0x1b: cpu_dec_r16(cpu, R16_DE); break;
    case 0x1c: cpu_inc_r8(cpu, R8_E); break;
    case 0x1d: cpu_dec_r8(cpu, R8_E); break;
    case 0x1e: cpu_ld_r8_n8(cpu, R8_E); break;
    case 0x1f: cpu_rra(cpu); break;

    case 0x20: cpu_jr_cc_i8(cpu, CC_NZ); break;
    case 0x21: cpu_ld_r16_n16(cpu, R16_HL); break;
    case 0x22: cpu_ld_hli_a(cpu); break;
    case 0x23: cpu_inc_r16(cpu, R16_HL); break;
    case 0x24: cpu_inc_r8(cpu, R8_H); break;
    case 0x25: cpu_dec_r8(cpu, R8_H); break;
    case 0x26: cpu_ld_r8_n8(cpu, R8_H); break;
    case 0x27: break;
    case 0x28: cpu_jr_cc_i8(cpu, CC_Z); break;
    case 0x29: cpu_add_hl_r16(cpu, R16_HL); break;
    case 0x2a: cpu_ld_a_hli(cpu); break;
    case 0x2b: cpu_dec_r16(cpu, R16_HL); break;
    case 0x2c: cpu_inc_r8(cpu, R8_L); break;
    case 0x2d: cpu_dec_r8(cpu, R8_L); break;
    case 0x2e: cpu_ld_r8_n8(cpu, R8_L); break;
    case 0x2f: cpu_cpl(cpu); break;

    case 0x30: cpu_jr_cc_i8(cpu, CC_NC); break;
    case 0x31: cpu_ld_sp_n16(cpu); break;
    case 0x32: cpu_ld_hld_a(cpu); break;
    case 0x33: cpu_inc_sp(cpu); break;
    case 0x34: cpu_inc_hl(cpu); break;
    case 0x35: cpu_dec_hl(cpu); break;
    case 0x36: cpu_ld_hl_n8(cpu); break;
    case 0x37: cpu_scf(cpu); break;
    case 0x38: cpu_jr_cc_i8(cpu, CC_C); break;
    case 0x39: cpu_add_hl_sp(cpu); break;
    case 0x3a: cpu_ld_a_hld(cpu); break;
    case 0x3b: cpu_dec_sp(cpu); break;
    case 0x3c: cpu_inc_r8(cpu, R8_A); break;
    case 0x3d: cpu_dec_r8(cpu, R8_A); break;
    case 0x3e: cpu_ld_r8_n8(cpu, R8_A); break;
    case 0x3f: cpu_ccf(cpu); break;

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

    case 0xc0: cpu_ret_cc(cpu, CC_NZ); break;
    case 0xc1: cpu_pop_r16(cpu, R16_BC); break;
    case 0xc2: cpu_jp_cc_n16(cpu, CC_NZ); break;
    case 0xc3: cpu_jp_n16(cpu); break;
    case 0xc4: cpu_call_cc_n16(cpu, CC_NZ); break;
    case 0xc5: cpu_push_r16(cpu, R16_BC); break;
    case 0xc6: cpu_add_a_n8(cpu); break;
    case 0xc7: cpu_rst_vec(cpu, RST_00); break;
    case 0xc8: cpu_ret_cc(cpu, CC_Z); break;
    case 0xc9: cpu_ret(cpu); break;
    case 0xca: cpu_jp_cc_n16(cpu, CC_Z); break;
    case 0xcb: cpu_execute_cb_opcode(cpu); break;
    case 0xcc: cpu_call_cc_n16(cpu, CC_Z); break;
    case 0xcd: cpu_call_n16(cpu); break;
    case 0xce: cpu_adc_a_n8(cpu); break;
    case 0xcf: cpu_rst_vec(cpu, RST_08); break;

    case 0xd0: cpu_ret_cc(cpu, CC_NC); break;
    case 0xd1: cpu_pop_r16(cpu, R16_DE); break;
    case 0xd2: cpu_jp_cc_n16(cpu, CC_NC); break;
    // case 0xd3:
    case 0xd4: cpu_call_cc_n16(cpu, CC_NC); break;
    case 0xd5: cpu_push_r16(cpu, R16_DE); break;
    case 0xd6: cpu_sub_a_n8(cpu); break;
    case 0xd7: cpu_rst_vec(cpu, RST_10); break;
    case 0xd8: cpu_ret_cc(cpu, CC_C); break;
    case 0xd9: cpu_reti(cpu); break;
    case 0xda: cpu_jp_cc_n16(cpu, CC_C); break;
    // case 0xdb:
    case 0xdc: cpu_call_cc_n16(cpu, CC_C); break;
    // case 0xdd:
    case 0xde: cpu_sbc_a_n8(cpu); break;
    case 0xdf: cpu_rst_vec(cpu, RST_18); break;

    case 0xe0: cpu_ldh_n8_a(cpu); break;
    case 0xe1: cpu_pop_r16(cpu, R16_HL); break;
    case 0xe2: cpu_ldh_c_a(cpu); break;
    // case 0xe3:
    // case 0xe4:
    case 0xe5: cpu_push_r16(cpu, R16_HL); break;
    case 0xe6: cpu_and_a_n8(cpu); break;
    case 0xe7: cpu_rst_vec(cpu, RST_20); break;
    case 0xe8: cpu_add_sp_i8(cpu); break;
    case 0xe9: cpu_jp_hl(cpu); break;
    case 0xea: cpu_ld_n16_a(cpu); break;
    // case 0xeb:
    // case 0xec:
    // case 0xed:
    case 0xee: cpu_xor_a_n8(cpu); break;
    case 0xef: cpu_rst_vec(cpu, RST_28); break;

    case 0xf0: cpu_ldh_a_n8(cpu); break;
    case 0xf1: cpu_pop_af(cpu); break;
    case 0xf2: cpu_ldh_a_c(cpu); break;
    case 0xf3: break;
    // case 0xf4:
    case 0xf5: cpu_push_af(cpu); break;
    case 0xf6: cpu_or_a_n8(cpu); break;
    case 0xf7: cpu_rst_vec(cpu, RST_30); break;
    case 0xf8: cpu_ld_hl_sp_i8(cpu); break;
    case 0xf9: cpu_ld_sp_hl(cpu); break;
    case 0xfa: cpu_ld_a_n16(cpu); break;
    case 0xfb: break;
    // case 0xfc:
    // case 0xfd:
    case 0xfe: cpu_cp_a_n8(cpu); break;
    case 0xff: cpu_rst_vec(cpu, RST_38); break;

    default: break;
    }
}

void cpu_tick(struct Cpu *cpu) { cpu_execute_opcode(cpu); }
