/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "cpu_instructions.h"

#include "mmu.h"

// Load instructions
void cpu_ld_r8_r8(struct Cpu *cpu, const enum Register8 dst, const enum Register8 src)
{
    if (dst == src)
    {
        return;
    }

    const u8 value = cpu_get_register8(cpu, src);
    cpu_set_register8(cpu, dst, value);
}

void cpu_ld_r8_n8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 src = cpu_fetch_u8(cpu);
    cpu_set_register8(cpu, dst, src);
}

void cpu_ld_r16_n16(struct Cpu *cpu, const enum Register16 dst)
{
    const u16 src = cpu_fetch_u16(cpu);
    cpu_set_register16(cpu, dst, src);
}

void cpu_ld_hl_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    mmu_write(cpu->mmu, cpu->registers.hl, value);
}

void cpu_ld_hl_n8(struct Cpu *cpu)
{
    const u8 src = cpu_fetch_u8(cpu);
    mmu_write(cpu->mmu, cpu->registers.hl, src);
}

void cpu_ld_r8_hl(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_set_register8(cpu, dst, value);
}

void cpu_ld_r16_a(struct Cpu *cpu, const enum Register16 dst)
{
    const u16 address = cpu_get_register16(cpu, dst);
    mmu_write(cpu->mmu, address, cpu->registers.a);
}

void cpu_ld_n16_a(struct Cpu *cpu)
{
    const u16 dst = cpu_fetch_u16(cpu);
    mmu_write(cpu->mmu, dst, cpu->registers.a);
}

void cpu_ldh_n8_a(struct Cpu *cpu)
{
    const u8 dst = cpu_fetch_u8(cpu);
    const u16 address = 0xff00 + dst;
    mmu_write(cpu->mmu, address, cpu->registers.a);
}

void cpu_ldh_c_a(struct Cpu *cpu)
{
    const u16 address = 0xff00 + cpu->registers.c;
    mmu_write(cpu->mmu, address, cpu->registers.a);
}

void cpu_ld_a_r16(struct Cpu *cpu, const enum Register16 src)
{
    const u16 address = cpu_get_register16(cpu, src);
    const u8 value = mmu_read(cpu->mmu, address);
    cpu->registers.a = value;
}

void cpu_ld_a_n16(struct Cpu *cpu)
{
    const u16 src = cpu_fetch_u16(cpu);
    const u8 value = mmu_read(cpu->mmu, src);
    cpu->registers.a = value;
}

void cpu_ldh_a_n8(struct Cpu *cpu)
{
    const u16 src = cpu_fetch_u8(cpu);
    const u16 address = 0xff00 + src;
    const u8 value = mmu_read(cpu->mmu, address);
    cpu->registers.a = value;
}

void cpu_ldh_a_c(struct Cpu *cpu)
{
    const u16 address = 0xff00 + cpu->registers.c;
    const u8 value = mmu_read(cpu->mmu, address);
    cpu->registers.a = value;
}

void cpu_ld_hli_a(struct Cpu *cpu)
{
    mmu_write(cpu->mmu, cpu->registers.hl, cpu->registers.a);
    cpu->registers.hl += 1;
}

void cpu_ld_hld_a(struct Cpu *cpu)
{
    mmu_write(cpu->mmu, cpu->registers.hl, cpu->registers.a);
    cpu->registers.hl -= 1;
}

void cpu_ld_a_hli(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->mmu, cpu->registers.hl);
    cpu->registers.a = value;
    cpu->registers.hl += 1;
}

void cpu_ld_a_hld(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->mmu, cpu->registers.hl);
    cpu->registers.a = value;
    cpu->registers.hl -= 1;
}

// 8-bit arithmetic instructions
static void cpu_adc_a(struct Cpu *cpu, const u8 value)
{
    const u8 carry = cpu_is_flag(cpu, FLAG_C);
    const u32 result_full = cpu->registers.a + value + carry;
    const u8 result = (u8) result_full;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, ((cpu->registers.a & 0x0f) + (value & 0x0f) + carry) > 0x0f);
    cpu_set_flag(cpu, FLAG_C, result_full > 0xff);

    cpu->registers.a = result;
}

void cpu_adc_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_adc_a(cpu, value);
}

void cpu_adc_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_adc_a(cpu, value);
}

void cpu_adc_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_adc_a(cpu, value);
}

static void cpu_add_a(struct Cpu *cpu, const u8 value)
{
    const u32 result_full = cpu->registers.a + value;
    const u8 result = (u8) result_full;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, ((cpu->registers.a & 0x0f) + (value & 0x0f)) > 0x0f);
    cpu_set_flag(cpu, FLAG_C, result_full > 0xff);

    cpu->registers.a = result;
}

void cpu_add_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_add_a(cpu, value);
}

void cpu_add_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_add_a(cpu, value);
}

void cpu_add_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_add_a(cpu, value);
}

static void cpu_cp_a(struct Cpu *cpu, const u8 value)
{
    const u8 result = cpu->registers.a - value;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, true);
    cpu_set_flag(cpu, FLAG_H, ((cpu->registers.a & 0x0f) - (value & 0x0f)) < 0x00);
    cpu_set_flag(cpu, FLAG_C, cpu->registers.a < value);
}

void cpu_cp_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_cp_a(cpu, value);
}

void cpu_cp_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_cp_a(cpu, value);
}

void cpu_cp_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_cp_a(cpu, value);
}

static uint8_t cpu_dec(struct Cpu *cpu, const u8 value)
{
    const u8 result = value - 1;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, true);
    cpu_set_flag(cpu, FLAG_H, (result & 0x0f) == 0x0f);

    return result;
}

void cpu_dec_r8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_dec(cpu, value);
    cpu_set_register8(cpu, dst, result);
}

void cpu_dec_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->mmu, cpu->registers.hl);
    const u8 result = cpu_dec(cpu, value);
    mmu_write(cpu->mmu, cpu->registers.hl, result);
}

static uint8_t cpu_inc(struct Cpu *cpu, const u8 value)
{
    const u8 result = value + 1;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, (result & 0x0f) == 0x00);

    return result;
}

void cpu_inc_r8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_inc(cpu, value);
    cpu_set_register8(cpu, dst, result);
}

void cpu_inc_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->mmu, cpu->registers.hl);
    const u8 result = cpu_inc(cpu, value);
    mmu_write(cpu->mmu, cpu->registers.hl, result);
}

static void cpu_sbc_a(struct Cpu *cpu, const u8 value)
{
    const u8 carry = cpu_is_flag(cpu, FLAG_C);
    const i32 result_full = cpu->registers.a - value - carry;
    const u8 result = (u8) result_full;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, true);
    cpu_set_flag(cpu, FLAG_H, ((cpu->registers.a & 0x0f) - (value & 0x0f) - carry) < 0x00);
    cpu_set_flag(cpu, FLAG_C, result_full < 0x00);

    cpu->registers.a = result;
}

void cpu_sbc_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_sbc_a(cpu, value);
}

void cpu_sbc_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_sbc_a(cpu, value);
}

void cpu_sbc_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_sbc_a(cpu, value);
}

static void cpu_sub_a(struct Cpu *cpu, const u8 value)
{
    const i32 result_full = cpu->registers.a - value;
    const u8 result = (u8) result_full;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, true);
    cpu_set_flag(cpu, FLAG_H, ((cpu->registers.a & 0x0f) - (value & 0x0f)) < 0x00);
    cpu_set_flag(cpu, FLAG_C, result_full < 0x00);

    cpu->registers.a = result;
}

void cpu_sub_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_sub_a(cpu, value);
}

void cpu_sub_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_sub_a(cpu, value);
}

void cpu_sub_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_sub_a(cpu, value);
}

// 16-bit arithmetic instructions
void cpu_add_hl_r16(struct Cpu *cpu, const enum Register16 src)
{
    const u16 value = cpu_get_register16(cpu, src);
    const u32 result_full = cpu->registers.hl + value;
    const u16 result = (u16) result_full;

    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, ((cpu->registers.hl & 0x0fff) + (value & 0x0fff)) > 0x0fff);
    cpu_set_flag(cpu, FLAG_C, (result_full & 0x10000) != 0);

    cpu->registers.hl = result;
}

void cpu_dec_r16(struct Cpu *cpu, const enum Register16 dst)
{
    const u16 value = cpu_get_register16(cpu, dst);
    const u32 result = value - 1;

    cpu_set_register16(cpu, dst, result);
}

void cpu_inc_r16(struct Cpu *cpu, const enum Register16 dst)
{
    const u16 value = cpu_get_register16(cpu, dst);
    const u32 result = value + 1;

    cpu_set_register16(cpu, dst, result);
}
