/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/cpu.h"

#include <stdbool.h>
#include <stdlib.h>

#include "gb/cpu_instructions.h"
#include "gb/mmu.h"
#include "gb/utils/bits.h"
#include "gb/utils/log.h"

static uint8_t cpu_execute_opcode(struct GbCpu *cpu, uint8_t opcode);
static uint8_t cpu_execute_cb_opcode(struct GbCpu *cpu, uint8_t opcode);

struct GbCpu *gb_cpu_create()
{
    struct GbCpu *cpu = malloc(sizeof(struct GbCpu));
    cpu->mmu = NULL;

    cpu->registers.a = 0x01;
    cpu->registers.f = 0x00;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, true);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, true);
    gb_cpu_set_flag(cpu, GB_FLAG_C, true);

    cpu->registers.b = 0x00;
    cpu->registers.c = 0x13;
    cpu->registers.d = 0x00;
    cpu->registers.e = 0xd8;
    cpu->registers.h = 0x01;
    cpu->registers.l = 0x4d;
    cpu->registers.pc = 0x0100;
    cpu->registers.sp = 0xfffe;

    cpu->interrupt_master_enable = false;
    cpu->ime_delay = 0;

    cpu->interrupt_enable = 0;
    cpu->interrupt_flag = 0;

    return cpu;
}

void gb_cpu_destroy(struct GbCpu *cpu) { free(cpu); }

void gb_cpu_set_register8(struct GbCpu *cpu, const enum GbRegister8 reg, const uint8_t value)
{
    switch (reg)
    {
    case GB_R8_A: cpu->registers.a = value; break;
    case GB_R8_B: cpu->registers.b = value; break;
    case GB_R8_C: cpu->registers.c = value; break;
    case GB_R8_D: cpu->registers.d = value; break;
    case GB_R8_E: cpu->registers.e = value; break;
    case GB_R8_H: cpu->registers.h = value; break;
    case GB_R8_L: cpu->registers.l = value; break;
    default: break;
    }
}

uint8_t gb_cpu_get_register8(struct GbCpu *cpu, const enum GbRegister8 reg)
{
    switch (reg)
    {
    case GB_R8_A: return cpu->registers.a;
    case GB_R8_B: return cpu->registers.b;
    case GB_R8_C: return cpu->registers.c;
    case GB_R8_D: return cpu->registers.d;
    case GB_R8_E: return cpu->registers.e;
    case GB_R8_H: return cpu->registers.h;
    case GB_R8_L: return cpu->registers.l;
    default: return 0x00;
    }
}

void gb_cpu_set_register16(struct GbCpu *cpu, const enum GbRegister16 reg, const uint16_t value)
{
    switch (reg)
    {
    case GB_R16_AF: cpu->registers.af = value; break;
    case GB_R16_BC: cpu->registers.bc = value; break;
    case GB_R16_DE: cpu->registers.de = value; break;
    case GB_R16_HL: cpu->registers.hl = value; break;
    default: break;
    }
}

uint16_t gb_cpu_get_register16(struct GbCpu *cpu, const enum GbRegister16 reg)
{
    switch (reg)
    {
    case GB_R16_AF: return cpu->registers.af;
    case GB_R16_BC: return cpu->registers.bc;
    case GB_R16_DE: return cpu->registers.de;
    case GB_R16_HL: return cpu->registers.hl;
    default: return 0x0000;
    }
}

void gb_cpu_set_flag(struct GbCpu *cpu, const enum GbFlag flag, const bool value)
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

bool gb_cpu_is_flag(struct GbCpu *cpu, const enum GbFlag flag) { return (cpu->registers.f & flag) != 0; }

bool gb_cpu_is_condition(struct GbCpu *cpu, const enum GbConditionCode cc)
{
    switch (cc)
    {
    case GB_CC_Z: return gb_cpu_is_flag(cpu, GB_FLAG_Z);
    case GB_CC_NZ: return !gb_cpu_is_flag(cpu, GB_FLAG_Z);
    case GB_CC_C: return gb_cpu_is_flag(cpu, GB_FLAG_C);
    case GB_CC_NC: return !gb_cpu_is_flag(cpu, GB_FLAG_C);
    default: return false;
    }
}

void gb_cpu_push_stack(struct GbCpu *cpu, const uint16_t value)
{
    cpu->registers.sp -= 1;
    gb_mmu_write(cpu->mmu, cpu->registers.sp, (uint8_t) ((value & 0xff00) >> 8));
    cpu->registers.sp -= 1;
    gb_mmu_write(cpu->mmu, cpu->registers.sp, (uint8_t) ((value & 0x00ff) >> 0));
}

uint16_t gb_cpu_pop_stack(struct GbCpu *cpu)
{
    const uint8_t lower_byte = gb_mmu_read(cpu->mmu, cpu->registers.sp);
    cpu->registers.sp += 1;

    const uint8_t higher_byte = gb_mmu_read(cpu->mmu, cpu->registers.sp);
    cpu->registers.sp += 1;

    const uint16_t result = ((uint16_t) higher_byte << 8) | (uint16_t) lower_byte;
    return result;
}

int8_t gb_cpu_fetch_i8(struct GbCpu *cpu) { return (int8_t) gb_cpu_fetch_u8(cpu); }

uint8_t gb_cpu_fetch_u8(struct GbCpu *cpu)
{
    const uint8_t byte = gb_mmu_read(cpu->mmu, cpu->registers.pc);
    cpu->registers.pc += 1;

    return byte;
}

uint16_t gb_cpu_fetch_u16(struct GbCpu *cpu)
{
    const uint16_t lower_byte = gb_cpu_fetch_u8(cpu);
    const uint16_t higher_byte = gb_cpu_fetch_u8(cpu);

    return (higher_byte << 8) | lower_byte;
}

static void cpu_handle_interrupts(struct GbCpu *cpu)
{
    const uint8_t fired_interrupts = cpu->interrupt_flag & cpu->interrupt_enable;
    if (!fired_interrupts)
    {
        return;
    }

    if (!cpu->interrupt_master_enable)
    {
        return;
    }

    gb_cpu_push_stack(cpu, cpu->registers.pc);

    bool handled_interrupt = false;
    if (GB_BIT_CHECK(fired_interrupts, 0))
    {
        GB_BIT_CLEAR(cpu->interrupt_flag, 0);
        cpu->registers.pc = 0x40;
        cpu->interrupt_master_enable = false;
        handled_interrupt = true;
    }

    if (handled_interrupt)
    {
        return;
    }

    if (GB_BIT_CHECK(fired_interrupts, 1))
    {
        GB_BIT_CLEAR(cpu->interrupt_flag, 1);
        cpu->registers.pc = 0x48;
        cpu->interrupt_master_enable = false;
        handled_interrupt = true;
    }

    if (handled_interrupt)
    {
        return;
    }

    if (GB_BIT_CHECK(fired_interrupts, 2))
    {
        GB_BIT_CLEAR(cpu->interrupt_flag, 2);
        cpu->registers.pc = 0x50;
        cpu->interrupt_master_enable = false;
        handled_interrupt = true;
    }

    if (handled_interrupt)
    {
        return;
    }

    if (GB_BIT_CHECK(fired_interrupts, 3))
    {
        GB_BIT_CLEAR(cpu->interrupt_flag, 3);
        cpu->registers.pc = 0x58;
        cpu->interrupt_master_enable = false;
        handled_interrupt = true;
    }

    if (handled_interrupt)
    {
        return;
    }

    if (GB_BIT_CHECK(fired_interrupts, 4))
    {
        GB_BIT_CLEAR(cpu->interrupt_flag, 4);
        cpu->registers.pc = 0x60;
        cpu->interrupt_master_enable = false;
        handled_interrupt = true;
    }

    if (handled_interrupt)
    {
        return;
    }
}

uint8_t gb_cpu_tick(struct GbCpu *cpu)
{
    // Handle Interrupts
    cpu_handle_interrupts(cpu);

    if (cpu->ime_delay > 0)
    {
        cpu->ime_delay -= 1;
        if (cpu->ime_delay == 0)
        {
            cpu->interrupt_master_enable = true;
        }
    }

    const uint8_t opcode = gb_cpu_fetch_u8(cpu);
    if (opcode == 0xcb)
    {
        const uint8_t cb_opcode = gb_cpu_fetch_u8(cpu);
        return cpu_execute_cb_opcode(cpu, cb_opcode);
    }

    return cpu_execute_opcode(cpu, opcode);
}

static uint8_t cpu_execute_opcode(struct GbCpu *cpu, const uint8_t opcode)
{
    switch (opcode)
    {
    case 0x00: return cpu_nop(cpu);
    case 0x01: return cpu_ld_r16_n16(cpu, GB_R16_BC);
    case 0x02: return cpu_ld_r16_a(cpu, GB_R16_BC);
    case 0x03: return cpu_inc_r16(cpu, GB_R16_BC);
    case 0x04: return cpu_inc_r8(cpu, GB_R8_B);
    case 0x05: return cpu_dec_r8(cpu, GB_R8_B);
    case 0x06: return cpu_ld_r8_n8(cpu, GB_R8_B);
    case 0x07: return cpu_rlca(cpu);
    case 0x08: return cpu_ld_n16_sp(cpu);
    case 0x09: return cpu_add_hl_r16(cpu, GB_R16_BC);
    case 0x0a: return cpu_ld_a_r16(cpu, GB_R16_BC);
    case 0x0b: return cpu_dec_r16(cpu, GB_R16_BC);
    case 0x0c: return cpu_inc_r8(cpu, GB_R8_C);
    case 0x0d: return cpu_dec_r8(cpu, GB_R8_C);
    case 0x0e: return cpu_ld_r8_n8(cpu, GB_R8_C);
    case 0x0f: return cpu_rrca(cpu);
    case 0x10: return cpu_stop(cpu);
    case 0x11: return cpu_ld_r16_n16(cpu, GB_R16_DE);
    case 0x12: return cpu_ld_r16_a(cpu, GB_R16_DE);
    case 0x13: return cpu_inc_r16(cpu, GB_R16_DE);
    case 0x14: return cpu_inc_r8(cpu, GB_R8_D);
    case 0x15: return cpu_dec_r8(cpu, GB_R8_D);
    case 0x16: return cpu_ld_r8_n8(cpu, GB_R8_D);
    case 0x17: return cpu_rla(cpu);
    case 0x18: return cpu_jr_i8(cpu);
    case 0x19: return cpu_add_hl_r16(cpu, GB_R16_DE);
    case 0x1a: return cpu_ld_a_r16(cpu, GB_R16_DE);
    case 0x1b: return cpu_dec_r16(cpu, GB_R16_DE);
    case 0x1c: return cpu_inc_r8(cpu, GB_R8_E);
    case 0x1d: return cpu_dec_r8(cpu, GB_R8_E);
    case 0x1e: return cpu_ld_r8_n8(cpu, GB_R8_E);
    case 0x1f: return cpu_rra(cpu);
    case 0x20: return cpu_jr_cc_i8(cpu, GB_CC_NZ);
    case 0x21: return cpu_ld_r16_n16(cpu, GB_R16_HL);
    case 0x22: return cpu_ld_hli_a(cpu);
    case 0x23: return cpu_inc_r16(cpu, GB_R16_HL);
    case 0x24: return cpu_inc_r8(cpu, GB_R8_H);
    case 0x25: return cpu_dec_r8(cpu, GB_R8_H);
    case 0x26: return cpu_ld_r8_n8(cpu, GB_R8_H);
    case 0x27: return cpu_daa(cpu);
    case 0x28: return cpu_jr_cc_i8(cpu, GB_CC_Z);
    case 0x29: return cpu_add_hl_r16(cpu, GB_R16_HL);
    case 0x2a: return cpu_ld_a_hli(cpu);
    case 0x2b: return cpu_dec_r16(cpu, GB_R16_HL);
    case 0x2c: return cpu_inc_r8(cpu, GB_R8_L);
    case 0x2d: return cpu_dec_r8(cpu, GB_R8_L);
    case 0x2e: return cpu_ld_r8_n8(cpu, GB_R8_L);
    case 0x2f: return cpu_cpl(cpu);
    case 0x30: return cpu_jr_cc_i8(cpu, GB_CC_NC);
    case 0x31: return cpu_ld_sp_n16(cpu);
    case 0x32: return cpu_ld_hld_a(cpu);
    case 0x33: return cpu_inc_sp(cpu);
    case 0x34: return cpu_inc_hl(cpu);
    case 0x35: return cpu_dec_hl(cpu);
    case 0x36: return cpu_ld_hl_n8(cpu);
    case 0x37: return cpu_scf(cpu);
    case 0x38: return cpu_jr_cc_i8(cpu, GB_CC_C);
    case 0x39: return cpu_add_hl_sp(cpu);
    case 0x3a: return cpu_ld_a_hld(cpu);
    case 0x3b: return cpu_dec_sp(cpu);
    case 0x3c: return cpu_inc_r8(cpu, GB_R8_A);
    case 0x3d: return cpu_dec_r8(cpu, GB_R8_A);
    case 0x3e: return cpu_ld_r8_n8(cpu, GB_R8_A);
    case 0x3f: return cpu_ccf(cpu);
    case 0x40: return cpu_ld_r8_r8(cpu, GB_R8_B, GB_R8_B);
    case 0x41: return cpu_ld_r8_r8(cpu, GB_R8_B, GB_R8_C);
    case 0x42: return cpu_ld_r8_r8(cpu, GB_R8_B, GB_R8_D);
    case 0x43: return cpu_ld_r8_r8(cpu, GB_R8_B, GB_R8_E);
    case 0x44: return cpu_ld_r8_r8(cpu, GB_R8_B, GB_R8_H);
    case 0x45: return cpu_ld_r8_r8(cpu, GB_R8_B, GB_R8_L);
    case 0x46: return cpu_ld_r8_hl(cpu, GB_R8_B);
    case 0x47: return cpu_ld_r8_r8(cpu, GB_R8_B, GB_R8_A);
    case 0x48: return cpu_ld_r8_r8(cpu, GB_R8_C, GB_R8_B);
    case 0x49: return cpu_ld_r8_r8(cpu, GB_R8_C, GB_R8_C);
    case 0x4a: return cpu_ld_r8_r8(cpu, GB_R8_C, GB_R8_D);
    case 0x4b: return cpu_ld_r8_r8(cpu, GB_R8_C, GB_R8_E);
    case 0x4c: return cpu_ld_r8_r8(cpu, GB_R8_C, GB_R8_H);
    case 0x4d: return cpu_ld_r8_r8(cpu, GB_R8_C, GB_R8_L);
    case 0x4e: return cpu_ld_r8_hl(cpu, GB_R8_C);
    case 0x4f: return cpu_ld_r8_r8(cpu, GB_R8_C, GB_R8_A);
    case 0x50: return cpu_ld_r8_r8(cpu, GB_R8_D, GB_R8_B);
    case 0x51: return cpu_ld_r8_r8(cpu, GB_R8_D, GB_R8_C);
    case 0x52: return cpu_ld_r8_r8(cpu, GB_R8_D, GB_R8_D);
    case 0x53: return cpu_ld_r8_r8(cpu, GB_R8_D, GB_R8_E);
    case 0x54: return cpu_ld_r8_r8(cpu, GB_R8_D, GB_R8_H);
    case 0x55: return cpu_ld_r8_r8(cpu, GB_R8_D, GB_R8_L);
    case 0x56: return cpu_ld_r8_hl(cpu, GB_R8_D);
    case 0x57: return cpu_ld_r8_r8(cpu, GB_R8_D, GB_R8_A);
    case 0x58: return cpu_ld_r8_r8(cpu, GB_R8_E, GB_R8_B);
    case 0x59: return cpu_ld_r8_r8(cpu, GB_R8_E, GB_R8_C);
    case 0x5a: return cpu_ld_r8_r8(cpu, GB_R8_E, GB_R8_D);
    case 0x5b: return cpu_ld_r8_r8(cpu, GB_R8_E, GB_R8_E);
    case 0x5c: return cpu_ld_r8_r8(cpu, GB_R8_E, GB_R8_H);
    case 0x5d: return cpu_ld_r8_r8(cpu, GB_R8_E, GB_R8_L);
    case 0x5e: return cpu_ld_r8_hl(cpu, GB_R8_E);
    case 0x5f: return cpu_ld_r8_r8(cpu, GB_R8_E, GB_R8_A);
    case 0x60: return cpu_ld_r8_r8(cpu, GB_R8_H, GB_R8_B);
    case 0x61: return cpu_ld_r8_r8(cpu, GB_R8_H, GB_R8_C);
    case 0x62: return cpu_ld_r8_r8(cpu, GB_R8_H, GB_R8_D);
    case 0x63: return cpu_ld_r8_r8(cpu, GB_R8_H, GB_R8_E);
    case 0x64: return cpu_ld_r8_r8(cpu, GB_R8_H, GB_R8_H);
    case 0x65: return cpu_ld_r8_r8(cpu, GB_R8_H, GB_R8_L);
    case 0x66: return cpu_ld_r8_hl(cpu, GB_R8_H);
    case 0x67: return cpu_ld_r8_r8(cpu, GB_R8_H, GB_R8_A);
    case 0x68: return cpu_ld_r8_r8(cpu, GB_R8_L, GB_R8_B);
    case 0x69: return cpu_ld_r8_r8(cpu, GB_R8_L, GB_R8_C);
    case 0x6a: return cpu_ld_r8_r8(cpu, GB_R8_L, GB_R8_D);
    case 0x6b: return cpu_ld_r8_r8(cpu, GB_R8_L, GB_R8_E);
    case 0x6c: return cpu_ld_r8_r8(cpu, GB_R8_L, GB_R8_H);
    case 0x6d: return cpu_ld_r8_r8(cpu, GB_R8_L, GB_R8_L);
    case 0x6e: return cpu_ld_r8_hl(cpu, GB_R8_L);
    case 0x6f: return cpu_ld_r8_r8(cpu, GB_R8_L, GB_R8_A);
    case 0x70: return cpu_ld_hl_r8(cpu, GB_R8_B);
    case 0x71: return cpu_ld_hl_r8(cpu, GB_R8_C);
    case 0x72: return cpu_ld_hl_r8(cpu, GB_R8_D);
    case 0x73: return cpu_ld_hl_r8(cpu, GB_R8_E);
    case 0x74: return cpu_ld_hl_r8(cpu, GB_R8_H);
    case 0x75: return cpu_ld_hl_r8(cpu, GB_R8_L);
    case 0x76: return cpu_halt(cpu);
    case 0x77: return cpu_ld_hl_r8(cpu, GB_R8_A);
    case 0x78: return cpu_ld_r8_r8(cpu, GB_R8_A, GB_R8_B);
    case 0x79: return cpu_ld_r8_r8(cpu, GB_R8_A, GB_R8_C);
    case 0x7a: return cpu_ld_r8_r8(cpu, GB_R8_A, GB_R8_D);
    case 0x7b: return cpu_ld_r8_r8(cpu, GB_R8_A, GB_R8_E);
    case 0x7c: return cpu_ld_r8_r8(cpu, GB_R8_A, GB_R8_H);
    case 0x7d: return cpu_ld_r8_r8(cpu, GB_R8_A, GB_R8_L);
    case 0x7e: return cpu_ld_r8_hl(cpu, GB_R8_A);
    case 0x7f: return cpu_ld_r8_r8(cpu, GB_R8_A, GB_R8_A);
    case 0x80: return cpu_add_a_r8(cpu, GB_R8_B);
    case 0x81: return cpu_add_a_r8(cpu, GB_R8_C);
    case 0x82: return cpu_add_a_r8(cpu, GB_R8_D);
    case 0x83: return cpu_add_a_r8(cpu, GB_R8_E);
    case 0x84: return cpu_add_a_r8(cpu, GB_R8_H);
    case 0x85: return cpu_add_a_r8(cpu, GB_R8_L);
    case 0x86: return cpu_add_a_hl(cpu);
    case 0x87: return cpu_add_a_r8(cpu, GB_R8_A);
    case 0x88: return cpu_adc_a_r8(cpu, GB_R8_B);
    case 0x89: return cpu_adc_a_r8(cpu, GB_R8_C);
    case 0x8a: return cpu_adc_a_r8(cpu, GB_R8_D);
    case 0x8b: return cpu_adc_a_r8(cpu, GB_R8_E);
    case 0x8c: return cpu_adc_a_r8(cpu, GB_R8_H);
    case 0x8d: return cpu_adc_a_r8(cpu, GB_R8_L);
    case 0x8e: return cpu_adc_a_hl(cpu);
    case 0x8f: return cpu_adc_a_r8(cpu, GB_R8_A);
    case 0x90: return cpu_sub_a_r8(cpu, GB_R8_B);
    case 0x91: return cpu_sub_a_r8(cpu, GB_R8_C);
    case 0x92: return cpu_sub_a_r8(cpu, GB_R8_D);
    case 0x93: return cpu_sub_a_r8(cpu, GB_R8_E);
    case 0x94: return cpu_sub_a_r8(cpu, GB_R8_H);
    case 0x95: return cpu_sub_a_r8(cpu, GB_R8_L);
    case 0x96: return cpu_sub_a_hl(cpu);
    case 0x97: return cpu_sub_a_r8(cpu, GB_R8_A);
    case 0x98: return cpu_sbc_a_r8(cpu, GB_R8_B);
    case 0x99: return cpu_sbc_a_r8(cpu, GB_R8_C);
    case 0x9a: return cpu_sbc_a_r8(cpu, GB_R8_D);
    case 0x9b: return cpu_sbc_a_r8(cpu, GB_R8_E);
    case 0x9c: return cpu_sbc_a_r8(cpu, GB_R8_H);
    case 0x9d: return cpu_sbc_a_r8(cpu, GB_R8_L);
    case 0x9e: return cpu_sbc_a_hl(cpu);
    case 0x9f: return cpu_sbc_a_r8(cpu, GB_R8_A);
    case 0xa0: return cpu_and_a_r8(cpu, GB_R8_B);
    case 0xa1: return cpu_and_a_r8(cpu, GB_R8_C);
    case 0xa2: return cpu_and_a_r8(cpu, GB_R8_D);
    case 0xa3: return cpu_and_a_r8(cpu, GB_R8_E);
    case 0xa4: return cpu_and_a_r8(cpu, GB_R8_H);
    case 0xa5: return cpu_and_a_r8(cpu, GB_R8_L);
    case 0xa6: return cpu_and_a_hl(cpu);
    case 0xa7: return cpu_and_a_r8(cpu, GB_R8_A);
    case 0xa8: return cpu_xor_a_r8(cpu, GB_R8_B);
    case 0xa9: return cpu_xor_a_r8(cpu, GB_R8_C);
    case 0xaa: return cpu_xor_a_r8(cpu, GB_R8_D);
    case 0xab: return cpu_xor_a_r8(cpu, GB_R8_E);
    case 0xac: return cpu_xor_a_r8(cpu, GB_R8_H);
    case 0xad: return cpu_xor_a_r8(cpu, GB_R8_L);
    case 0xae: return cpu_xor_a_hl(cpu);
    case 0xaf: return cpu_xor_a_r8(cpu, GB_R8_A);
    case 0xb0: return cpu_or_a_r8(cpu, GB_R8_B);
    case 0xb1: return cpu_or_a_r8(cpu, GB_R8_C);
    case 0xb2: return cpu_or_a_r8(cpu, GB_R8_D);
    case 0xb3: return cpu_or_a_r8(cpu, GB_R8_E);
    case 0xb4: return cpu_or_a_r8(cpu, GB_R8_H);
    case 0xb5: return cpu_or_a_r8(cpu, GB_R8_L);
    case 0xb6: return cpu_or_a_hl(cpu);
    case 0xb7: return cpu_or_a_r8(cpu, GB_R8_A);
    case 0xb8: return cpu_cp_a_r8(cpu, GB_R8_B);
    case 0xb9: return cpu_cp_a_r8(cpu, GB_R8_C);
    case 0xba: return cpu_cp_a_r8(cpu, GB_R8_D);
    case 0xbb: return cpu_cp_a_r8(cpu, GB_R8_E);
    case 0xbc: return cpu_cp_a_r8(cpu, GB_R8_H);
    case 0xbd: return cpu_cp_a_r8(cpu, GB_R8_L);
    case 0xbe: return cpu_cp_a_hl(cpu);
    case 0xbf: return cpu_cp_a_r8(cpu, GB_R8_A);
    case 0xc0: return cpu_ret_cc(cpu, GB_CC_NZ);
    case 0xc1: return cpu_pop_r16(cpu, GB_R16_BC);
    case 0xc2: return cpu_jp_cc_n16(cpu, GB_CC_NZ);
    case 0xc3: return cpu_jp_n16(cpu);
    case 0xc4: return cpu_call_cc_n16(cpu, GB_CC_NZ);
    case 0xc5: return cpu_push_r16(cpu, GB_R16_BC);
    case 0xc6: return cpu_add_a_n8(cpu);
    case 0xc7: return cpu_rst_vec(cpu, GB_RST_00);
    case 0xc8: return cpu_ret_cc(cpu, GB_CC_Z);
    case 0xc9: return cpu_ret(cpu);
    case 0xca: return cpu_jp_cc_n16(cpu, GB_CC_Z);
    case 0xcc: return cpu_call_cc_n16(cpu, GB_CC_Z);
    case 0xcd: return cpu_call_n16(cpu);
    case 0xce: return cpu_adc_a_n8(cpu);
    case 0xcf: return cpu_rst_vec(cpu, GB_RST_08);
    case 0xd0: return cpu_ret_cc(cpu, GB_CC_NC);
    case 0xd1: return cpu_pop_r16(cpu, GB_R16_DE);
    case 0xd2: return cpu_jp_cc_n16(cpu, GB_CC_NC);
    case 0xd4: return cpu_call_cc_n16(cpu, GB_CC_NC);
    case 0xd5: return cpu_push_r16(cpu, GB_R16_DE);
    case 0xd6: return cpu_sub_a_n8(cpu);
    case 0xd7: return cpu_rst_vec(cpu, GB_RST_10);
    case 0xd8: return cpu_ret_cc(cpu, GB_CC_C);
    case 0xd9: return cpu_reti(cpu);
    case 0xda: return cpu_jp_cc_n16(cpu, GB_CC_C);
    case 0xdc: return cpu_call_cc_n16(cpu, GB_CC_C);
    case 0xde: return cpu_sbc_a_n8(cpu);
    case 0xdf: return cpu_rst_vec(cpu, GB_RST_18);
    case 0xe0: return cpu_ldh_n8_a(cpu);
    case 0xe1: return cpu_pop_r16(cpu, GB_R16_HL);
    case 0xe2: return cpu_ldh_c_a(cpu);
    case 0xe5: return cpu_push_r16(cpu, GB_R16_HL);
    case 0xe6: return cpu_and_a_n8(cpu);
    case 0xe7: return cpu_rst_vec(cpu, GB_RST_20);
    case 0xe8: return cpu_add_sp_i8(cpu);
    case 0xe9: return cpu_jp_hl(cpu);
    case 0xea: return cpu_ld_n16_a(cpu);
    case 0xee: return cpu_xor_a_n8(cpu);
    case 0xef: return cpu_rst_vec(cpu, GB_RST_28);
    case 0xf0: return cpu_ldh_a_n8(cpu);
    case 0xf1: return cpu_pop_af(cpu);
    case 0xf2: return cpu_ldh_a_c(cpu);
    case 0xf3: return cpu_di(cpu);
    case 0xf5: return cpu_push_af(cpu);
    case 0xf6: return cpu_or_a_n8(cpu);
    case 0xf7: return cpu_rst_vec(cpu, GB_RST_30);
    case 0xf8: return cpu_ld_hl_sp_i8(cpu);
    case 0xf9: return cpu_ld_sp_hl(cpu);
    case 0xfa: return cpu_ld_a_n16(cpu);
    case 0xfb: return cpu_ei(cpu);
    case 0xfe: return cpu_cp_a_n8(cpu);
    case 0xff: return cpu_rst_vec(cpu, GB_RST_38);
    default: gb_log(GB_LOG_ERROR, "Invalid instruction encountered: 0x%02x\n", opcode); return 0;
    }
}

static uint8_t cpu_execute_cb_opcode(struct GbCpu *cpu, const uint8_t opcode)
{
    switch (opcode)
    {
    case 0x00: return cpu_rlc_r8(cpu, GB_R8_B);
    case 0x01: return cpu_rlc_r8(cpu, GB_R8_C);
    case 0x02: return cpu_rlc_r8(cpu, GB_R8_D);
    case 0x03: return cpu_rlc_r8(cpu, GB_R8_E);
    case 0x04: return cpu_rlc_r8(cpu, GB_R8_H);
    case 0x05: return cpu_rlc_r8(cpu, GB_R8_L);
    case 0x06: return cpu_rlc_hl(cpu);
    case 0x07: return cpu_rlc_r8(cpu, GB_R8_A);
    case 0x08: return cpu_rrc_r8(cpu, GB_R8_B);
    case 0x09: return cpu_rrc_r8(cpu, GB_R8_C);
    case 0x0a: return cpu_rrc_r8(cpu, GB_R8_D);
    case 0x0b: return cpu_rrc_r8(cpu, GB_R8_E);
    case 0x0c: return cpu_rrc_r8(cpu, GB_R8_H);
    case 0x0d: return cpu_rrc_r8(cpu, GB_R8_L);
    case 0x0e: return cpu_rrc_hl(cpu);
    case 0x0f: return cpu_rrc_r8(cpu, GB_R8_A);
    case 0x10: return cpu_rl_r8(cpu, GB_R8_B);
    case 0x11: return cpu_rl_r8(cpu, GB_R8_C);
    case 0x12: return cpu_rl_r8(cpu, GB_R8_D);
    case 0x13: return cpu_rl_r8(cpu, GB_R8_E);
    case 0x14: return cpu_rl_r8(cpu, GB_R8_H);
    case 0x15: return cpu_rl_r8(cpu, GB_R8_L);
    case 0x16: return cpu_rl_hl(cpu);
    case 0x17: return cpu_rl_r8(cpu, GB_R8_A);
    case 0x18: return cpu_rr_r8(cpu, GB_R8_B);
    case 0x19: return cpu_rr_r8(cpu, GB_R8_C);
    case 0x1a: return cpu_rr_r8(cpu, GB_R8_D);
    case 0x1b: return cpu_rr_r8(cpu, GB_R8_E);
    case 0x1c: return cpu_rr_r8(cpu, GB_R8_H);
    case 0x1d: return cpu_rr_r8(cpu, GB_R8_L);
    case 0x1e: return cpu_rr_hl(cpu);
    case 0x1f: return cpu_rr_r8(cpu, GB_R8_A);
    case 0x20: return cpu_sla_r8(cpu, GB_R8_B);
    case 0x21: return cpu_sla_r8(cpu, GB_R8_C);
    case 0x22: return cpu_sla_r8(cpu, GB_R8_D);
    case 0x23: return cpu_sla_r8(cpu, GB_R8_E);
    case 0x24: return cpu_sla_r8(cpu, GB_R8_H);
    case 0x25: return cpu_sla_r8(cpu, GB_R8_L);
    case 0x26: return cpu_sla_hl(cpu);
    case 0x27: return cpu_sla_r8(cpu, GB_R8_A);
    case 0x28: return cpu_sra_r8(cpu, GB_R8_B);
    case 0x29: return cpu_sra_r8(cpu, GB_R8_C);
    case 0x2a: return cpu_sra_r8(cpu, GB_R8_D);
    case 0x2b: return cpu_sra_r8(cpu, GB_R8_E);
    case 0x2c: return cpu_sra_r8(cpu, GB_R8_H);
    case 0x2d: return cpu_sra_r8(cpu, GB_R8_L);
    case 0x2e: return cpu_sra_hl(cpu);
    case 0x2f: return cpu_sra_r8(cpu, GB_R8_A);
    case 0x30: return cpu_swap_r8(cpu, GB_R8_B);
    case 0x31: return cpu_swap_r8(cpu, GB_R8_C);
    case 0x32: return cpu_swap_r8(cpu, GB_R8_D);
    case 0x33: return cpu_swap_r8(cpu, GB_R8_E);
    case 0x34: return cpu_swap_r8(cpu, GB_R8_H);
    case 0x35: return cpu_swap_r8(cpu, GB_R8_L);
    case 0x36: return cpu_swap_hl(cpu);
    case 0x37: return cpu_swap_r8(cpu, GB_R8_A);
    case 0x38: return cpu_srl_r8(cpu, GB_R8_B);
    case 0x39: return cpu_srl_r8(cpu, GB_R8_C);
    case 0x3a: return cpu_srl_r8(cpu, GB_R8_D);
    case 0x3b: return cpu_srl_r8(cpu, GB_R8_E);
    case 0x3c: return cpu_srl_r8(cpu, GB_R8_H);
    case 0x3d: return cpu_srl_r8(cpu, GB_R8_L);
    case 0x3e: return cpu_srl_hl(cpu);
    case 0x3f: return cpu_srl_r8(cpu, GB_R8_A);
    case 0x40: return cpu_bit_u3_r8(cpu, 0, GB_R8_B);
    case 0x41: return cpu_bit_u3_r8(cpu, 0, GB_R8_C);
    case 0x42: return cpu_bit_u3_r8(cpu, 0, GB_R8_D);
    case 0x43: return cpu_bit_u3_r8(cpu, 0, GB_R8_E);
    case 0x44: return cpu_bit_u3_r8(cpu, 0, GB_R8_H);
    case 0x45: return cpu_bit_u3_r8(cpu, 0, GB_R8_L);
    case 0x46: return cpu_bit_u3_hl(cpu, 0);
    case 0x47: return cpu_bit_u3_r8(cpu, 0, GB_R8_A);
    case 0x48: return cpu_bit_u3_r8(cpu, 1, GB_R8_B);
    case 0x49: return cpu_bit_u3_r8(cpu, 1, GB_R8_C);
    case 0x4a: return cpu_bit_u3_r8(cpu, 1, GB_R8_D);
    case 0x4b: return cpu_bit_u3_r8(cpu, 1, GB_R8_E);
    case 0x4c: return cpu_bit_u3_r8(cpu, 1, GB_R8_H);
    case 0x4d: return cpu_bit_u3_r8(cpu, 1, GB_R8_L);
    case 0x4e: return cpu_bit_u3_hl(cpu, 1);
    case 0x4f: return cpu_bit_u3_r8(cpu, 1, GB_R8_A);
    case 0x50: return cpu_bit_u3_r8(cpu, 2, GB_R8_B);
    case 0x51: return cpu_bit_u3_r8(cpu, 2, GB_R8_C);
    case 0x52: return cpu_bit_u3_r8(cpu, 2, GB_R8_D);
    case 0x53: return cpu_bit_u3_r8(cpu, 2, GB_R8_E);
    case 0x54: return cpu_bit_u3_r8(cpu, 2, GB_R8_H);
    case 0x55: return cpu_bit_u3_r8(cpu, 2, GB_R8_L);
    case 0x56: return cpu_bit_u3_hl(cpu, 2);
    case 0x57: return cpu_bit_u3_r8(cpu, 2, GB_R8_A);
    case 0x58: return cpu_bit_u3_r8(cpu, 3, GB_R8_B);
    case 0x59: return cpu_bit_u3_r8(cpu, 3, GB_R8_C);
    case 0x5a: return cpu_bit_u3_r8(cpu, 3, GB_R8_D);
    case 0x5b: return cpu_bit_u3_r8(cpu, 3, GB_R8_E);
    case 0x5c: return cpu_bit_u3_r8(cpu, 3, GB_R8_H);
    case 0x5d: return cpu_bit_u3_r8(cpu, 3, GB_R8_L);
    case 0x5e: return cpu_bit_u3_hl(cpu, 3);
    case 0x5f: return cpu_bit_u3_r8(cpu, 3, GB_R8_A);
    case 0x60: return cpu_bit_u3_r8(cpu, 4, GB_R8_B);
    case 0x61: return cpu_bit_u3_r8(cpu, 4, GB_R8_C);
    case 0x62: return cpu_bit_u3_r8(cpu, 4, GB_R8_D);
    case 0x63: return cpu_bit_u3_r8(cpu, 4, GB_R8_E);
    case 0x64: return cpu_bit_u3_r8(cpu, 4, GB_R8_H);
    case 0x65: return cpu_bit_u3_r8(cpu, 4, GB_R8_L);
    case 0x66: return cpu_bit_u3_hl(cpu, 4);
    case 0x67: return cpu_bit_u3_r8(cpu, 4, GB_R8_A);
    case 0x68: return cpu_bit_u3_r8(cpu, 5, GB_R8_B);
    case 0x69: return cpu_bit_u3_r8(cpu, 5, GB_R8_C);
    case 0x6a: return cpu_bit_u3_r8(cpu, 5, GB_R8_D);
    case 0x6b: return cpu_bit_u3_r8(cpu, 5, GB_R8_E);
    case 0x6c: return cpu_bit_u3_r8(cpu, 5, GB_R8_H);
    case 0x6d: return cpu_bit_u3_r8(cpu, 5, GB_R8_L);
    case 0x6e: return cpu_bit_u3_hl(cpu, 5);
    case 0x6f: return cpu_bit_u3_r8(cpu, 5, GB_R8_A);
    case 0x70: return cpu_bit_u3_r8(cpu, 6, GB_R8_B);
    case 0x71: return cpu_bit_u3_r8(cpu, 6, GB_R8_C);
    case 0x72: return cpu_bit_u3_r8(cpu, 6, GB_R8_D);
    case 0x73: return cpu_bit_u3_r8(cpu, 6, GB_R8_E);
    case 0x74: return cpu_bit_u3_r8(cpu, 6, GB_R8_H);
    case 0x75: return cpu_bit_u3_r8(cpu, 6, GB_R8_L);
    case 0x76: return cpu_bit_u3_hl(cpu, 6);
    case 0x77: return cpu_bit_u3_r8(cpu, 6, GB_R8_A);
    case 0x78: return cpu_bit_u3_r8(cpu, 7, GB_R8_B);
    case 0x79: return cpu_bit_u3_r8(cpu, 7, GB_R8_C);
    case 0x7a: return cpu_bit_u3_r8(cpu, 7, GB_R8_D);
    case 0x7b: return cpu_bit_u3_r8(cpu, 7, GB_R8_E);
    case 0x7c: return cpu_bit_u3_r8(cpu, 7, GB_R8_H);
    case 0x7d: return cpu_bit_u3_r8(cpu, 7, GB_R8_L);
    case 0x7e: return cpu_bit_u3_hl(cpu, 7);
    case 0x7f: return cpu_bit_u3_r8(cpu, 7, GB_R8_A);
    case 0x80: return cpu_res_u3_r8(cpu, 0, GB_R8_B);
    case 0x81: return cpu_res_u3_r8(cpu, 0, GB_R8_C);
    case 0x82: return cpu_res_u3_r8(cpu, 0, GB_R8_D);
    case 0x83: return cpu_res_u3_r8(cpu, 0, GB_R8_E);
    case 0x84: return cpu_res_u3_r8(cpu, 0, GB_R8_H);
    case 0x85: return cpu_res_u3_r8(cpu, 0, GB_R8_L);
    case 0x86: return cpu_res_u3_hl(cpu, 0);
    case 0x87: return cpu_res_u3_r8(cpu, 0, GB_R8_A);
    case 0x88: return cpu_res_u3_r8(cpu, 1, GB_R8_B);
    case 0x89: return cpu_res_u3_r8(cpu, 1, GB_R8_C);
    case 0x8a: return cpu_res_u3_r8(cpu, 1, GB_R8_D);
    case 0x8b: return cpu_res_u3_r8(cpu, 1, GB_R8_E);
    case 0x8c: return cpu_res_u3_r8(cpu, 1, GB_R8_H);
    case 0x8d: return cpu_res_u3_r8(cpu, 1, GB_R8_L);
    case 0x8e: return cpu_res_u3_hl(cpu, 1);
    case 0x8f: return cpu_res_u3_r8(cpu, 1, GB_R8_A);
    case 0x90: return cpu_res_u3_r8(cpu, 2, GB_R8_B);
    case 0x91: return cpu_res_u3_r8(cpu, 2, GB_R8_C);
    case 0x92: return cpu_res_u3_r8(cpu, 2, GB_R8_D);
    case 0x93: return cpu_res_u3_r8(cpu, 2, GB_R8_E);
    case 0x94: return cpu_res_u3_r8(cpu, 2, GB_R8_H);
    case 0x95: return cpu_res_u3_r8(cpu, 2, GB_R8_L);
    case 0x96: return cpu_res_u3_hl(cpu, 2);
    case 0x97: return cpu_res_u3_r8(cpu, 2, GB_R8_A);
    case 0x98: return cpu_res_u3_r8(cpu, 3, GB_R8_B);
    case 0x99: return cpu_res_u3_r8(cpu, 3, GB_R8_C);
    case 0x9a: return cpu_res_u3_r8(cpu, 3, GB_R8_D);
    case 0x9b: return cpu_res_u3_r8(cpu, 3, GB_R8_E);
    case 0x9c: return cpu_res_u3_r8(cpu, 3, GB_R8_H);
    case 0x9d: return cpu_res_u3_r8(cpu, 3, GB_R8_L);
    case 0x9e: return cpu_res_u3_hl(cpu, 3);
    case 0x9f: return cpu_res_u3_r8(cpu, 3, GB_R8_A);
    case 0xa0: return cpu_res_u3_r8(cpu, 4, GB_R8_B);
    case 0xa1: return cpu_res_u3_r8(cpu, 4, GB_R8_C);
    case 0xa2: return cpu_res_u3_r8(cpu, 4, GB_R8_D);
    case 0xa3: return cpu_res_u3_r8(cpu, 4, GB_R8_E);
    case 0xa4: return cpu_res_u3_r8(cpu, 4, GB_R8_H);
    case 0xa5: return cpu_res_u3_r8(cpu, 4, GB_R8_L);
    case 0xa6: return cpu_res_u3_hl(cpu, 4);
    case 0xa7: return cpu_res_u3_r8(cpu, 4, GB_R8_A);
    case 0xa8: return cpu_res_u3_r8(cpu, 5, GB_R8_B);
    case 0xa9: return cpu_res_u3_r8(cpu, 5, GB_R8_C);
    case 0xaa: return cpu_res_u3_r8(cpu, 5, GB_R8_D);
    case 0xab: return cpu_res_u3_r8(cpu, 5, GB_R8_E);
    case 0xac: return cpu_res_u3_r8(cpu, 5, GB_R8_H);
    case 0xad: return cpu_res_u3_r8(cpu, 5, GB_R8_L);
    case 0xae: return cpu_res_u3_hl(cpu, 5);
    case 0xaf: return cpu_res_u3_r8(cpu, 5, GB_R8_A);
    case 0xb0: return cpu_res_u3_r8(cpu, 6, GB_R8_B);
    case 0xb1: return cpu_res_u3_r8(cpu, 6, GB_R8_C);
    case 0xb2: return cpu_res_u3_r8(cpu, 6, GB_R8_D);
    case 0xb3: return cpu_res_u3_r8(cpu, 6, GB_R8_E);
    case 0xb4: return cpu_res_u3_r8(cpu, 6, GB_R8_H);
    case 0xb5: return cpu_res_u3_r8(cpu, 6, GB_R8_L);
    case 0xb6: return cpu_res_u3_hl(cpu, 6);
    case 0xb7: return cpu_res_u3_r8(cpu, 6, GB_R8_A);
    case 0xb8: return cpu_res_u3_r8(cpu, 7, GB_R8_B);
    case 0xb9: return cpu_res_u3_r8(cpu, 7, GB_R8_C);
    case 0xba: return cpu_res_u3_r8(cpu, 7, GB_R8_D);
    case 0xbb: return cpu_res_u3_r8(cpu, 7, GB_R8_E);
    case 0xbc: return cpu_res_u3_r8(cpu, 7, GB_R8_H);
    case 0xbd: return cpu_res_u3_r8(cpu, 7, GB_R8_L);
    case 0xbe: return cpu_res_u3_hl(cpu, 7);
    case 0xbf: return cpu_res_u3_r8(cpu, 7, GB_R8_A);
    case 0xc0: return cpu_set_u3_r8(cpu, 0, GB_R8_B);
    case 0xc1: return cpu_set_u3_r8(cpu, 0, GB_R8_C);
    case 0xc2: return cpu_set_u3_r8(cpu, 0, GB_R8_D);
    case 0xc3: return cpu_set_u3_r8(cpu, 0, GB_R8_E);
    case 0xc4: return cpu_set_u3_r8(cpu, 0, GB_R8_H);
    case 0xc5: return cpu_set_u3_r8(cpu, 0, GB_R8_L);
    case 0xc6: return cpu_set_u3_hl(cpu, 0);
    case 0xc7: return cpu_set_u3_r8(cpu, 0, GB_R8_A);
    case 0xc8: return cpu_set_u3_r8(cpu, 1, GB_R8_B);
    case 0xc9: return cpu_set_u3_r8(cpu, 1, GB_R8_C);
    case 0xca: return cpu_set_u3_r8(cpu, 1, GB_R8_D);
    case 0xcb: return cpu_set_u3_r8(cpu, 1, GB_R8_E);
    case 0xcc: return cpu_set_u3_r8(cpu, 1, GB_R8_H);
    case 0xcd: return cpu_set_u3_r8(cpu, 1, GB_R8_L);
    case 0xce: return cpu_set_u3_hl(cpu, 1);
    case 0xcf: return cpu_set_u3_r8(cpu, 1, GB_R8_A);
    case 0xd0: return cpu_set_u3_r8(cpu, 2, GB_R8_B);
    case 0xd1: return cpu_set_u3_r8(cpu, 2, GB_R8_C);
    case 0xd2: return cpu_set_u3_r8(cpu, 2, GB_R8_D);
    case 0xd3: return cpu_set_u3_r8(cpu, 2, GB_R8_E);
    case 0xd4: return cpu_set_u3_r8(cpu, 2, GB_R8_H);
    case 0xd5: return cpu_set_u3_r8(cpu, 2, GB_R8_L);
    case 0xd6: return cpu_set_u3_hl(cpu, 2);
    case 0xd7: return cpu_set_u3_r8(cpu, 2, GB_R8_A);
    case 0xd8: return cpu_set_u3_r8(cpu, 3, GB_R8_B);
    case 0xd9: return cpu_set_u3_r8(cpu, 3, GB_R8_C);
    case 0xda: return cpu_set_u3_r8(cpu, 3, GB_R8_D);
    case 0xdb: return cpu_set_u3_r8(cpu, 3, GB_R8_E);
    case 0xdc: return cpu_set_u3_r8(cpu, 3, GB_R8_H);
    case 0xdd: return cpu_set_u3_r8(cpu, 3, GB_R8_L);
    case 0xde: return cpu_set_u3_hl(cpu, 3);
    case 0xdf: return cpu_set_u3_r8(cpu, 3, GB_R8_A);
    case 0xe0: return cpu_set_u3_r8(cpu, 4, GB_R8_B);
    case 0xe1: return cpu_set_u3_r8(cpu, 4, GB_R8_C);
    case 0xe2: return cpu_set_u3_r8(cpu, 4, GB_R8_D);
    case 0xe3: return cpu_set_u3_r8(cpu, 4, GB_R8_E);
    case 0xe4: return cpu_set_u3_r8(cpu, 4, GB_R8_H);
    case 0xe5: return cpu_set_u3_r8(cpu, 4, GB_R8_L);
    case 0xe6: return cpu_set_u3_hl(cpu, 4);
    case 0xe7: return cpu_set_u3_r8(cpu, 4, GB_R8_A);
    case 0xe8: return cpu_set_u3_r8(cpu, 5, GB_R8_B);
    case 0xe9: return cpu_set_u3_r8(cpu, 5, GB_R8_C);
    case 0xea: return cpu_set_u3_r8(cpu, 5, GB_R8_D);
    case 0xeb: return cpu_set_u3_r8(cpu, 5, GB_R8_E);
    case 0xec: return cpu_set_u3_r8(cpu, 5, GB_R8_H);
    case 0xed: return cpu_set_u3_r8(cpu, 5, GB_R8_L);
    case 0xee: return cpu_set_u3_hl(cpu, 5);
    case 0xef: return cpu_set_u3_r8(cpu, 5, GB_R8_A);
    case 0xf0: return cpu_set_u3_r8(cpu, 6, GB_R8_B);
    case 0xf1: return cpu_set_u3_r8(cpu, 6, GB_R8_C);
    case 0xf2: return cpu_set_u3_r8(cpu, 6, GB_R8_D);
    case 0xf3: return cpu_set_u3_r8(cpu, 6, GB_R8_E);
    case 0xf4: return cpu_set_u3_r8(cpu, 6, GB_R8_H);
    case 0xf5: return cpu_set_u3_r8(cpu, 6, GB_R8_L);
    case 0xf6: return cpu_set_u3_hl(cpu, 6);
    case 0xf7: return cpu_set_u3_r8(cpu, 6, GB_R8_A);
    case 0xf8: return cpu_set_u3_r8(cpu, 7, GB_R8_B);
    case 0xf9: return cpu_set_u3_r8(cpu, 7, GB_R8_C);
    case 0xfa: return cpu_set_u3_r8(cpu, 7, GB_R8_D);
    case 0xfb: return cpu_set_u3_r8(cpu, 7, GB_R8_E);
    case 0xfc: return cpu_set_u3_r8(cpu, 7, GB_R8_H);
    case 0xfd: return cpu_set_u3_r8(cpu, 7, GB_R8_L);
    case 0xfe: return cpu_set_u3_hl(cpu, 7);
    case 0xff: return cpu_set_u3_r8(cpu, 7, GB_R8_A);
    default: gb_log(GB_LOG_ERROR, "Invalid instruction encountered: 0xcb 0x%02x\n", opcode); return 0;
    }
}
