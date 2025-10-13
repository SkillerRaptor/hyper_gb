/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "cpu_instructions.h"

#include "gameboy.h"
#include "mmu.h"
#include "utils/bits.h"

// Load instructions
u8 cpu_ld_r8_r8(struct Cpu *cpu, const enum Register8 dst, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_set_register8(cpu, dst, value);
    return 1;
}

u8 cpu_ld_r8_n8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 src = cpu_fetch_u8(cpu);
    cpu_set_register8(cpu, dst, src);
    return 2;
}

u8 cpu_ld_r16_n16(struct Cpu *cpu, const enum Register16 dst)
{
    const u16 src = cpu_fetch_u16(cpu);
    cpu_set_register16(cpu, dst, src);
    return 3;
}

u8 cpu_ld_hl_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, value);
    return 2;
}

u8 cpu_ld_hl_n8(struct Cpu *cpu)
{
    const u8 src = cpu_fetch_u8(cpu);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, src);
    return 3;
}

u8 cpu_ld_r8_hl(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    cpu_set_register8(cpu, dst, value);
    return 2;
}

u8 cpu_ld_r16_a(struct Cpu *cpu, const enum Register16 dst)
{
    const u16 address = cpu_get_register16(cpu, dst);
    mmu_write(cpu->gb->mmu, address, cpu->registers.a);
    return 2;
}

u8 cpu_ld_n16_a(struct Cpu *cpu)
{
    const u16 dst = cpu_fetch_u16(cpu);
    mmu_write(cpu->gb->mmu, dst, cpu->registers.a);
    return 4;
}

u8 cpu_ldh_n8_a(struct Cpu *cpu)
{
    const u8 dst = cpu_fetch_u8(cpu);
    const u16 address = 0xff00 + dst;
    mmu_write(cpu->gb->mmu, address, cpu->registers.a);
    return 3;
}

u8 cpu_ldh_c_a(struct Cpu *cpu)
{
    const u16 address = 0xff00 + cpu->registers.c;
    mmu_write(cpu->gb->mmu, address, cpu->registers.a);
    return 2;
}

u8 cpu_ld_a_r16(struct Cpu *cpu, const enum Register16 src)
{
    const u16 address = cpu_get_register16(cpu, src);
    const u8 value = mmu_read(cpu->gb->mmu, address);
    cpu->registers.a = value;
    return 2;
}

u8 cpu_ld_a_n16(struct Cpu *cpu)
{
    const u16 src = cpu_fetch_u16(cpu);
    const u8 value = mmu_read(cpu->gb->mmu, src);
    cpu->registers.a = value;
    return 4;
}

u8 cpu_ldh_a_n8(struct Cpu *cpu)
{
    const u16 src = cpu_fetch_u8(cpu);
    const u16 address = 0xff00 + src;
    const u8 value = mmu_read(cpu->gb->mmu, address);
    cpu->registers.a = value;
    return 3;
}

u8 cpu_ldh_a_c(struct Cpu *cpu)
{
    const u16 address = 0xff00 + cpu->registers.c;
    const u8 value = mmu_read(cpu->gb->mmu, address);
    cpu->registers.a = value;
    return 2;
}

u8 cpu_ld_hli_a(struct Cpu *cpu)
{
    mmu_write(cpu->gb->mmu, cpu->registers.hl, cpu->registers.a);
    cpu->registers.hl += 1;
    return 2;
}

u8 cpu_ld_hld_a(struct Cpu *cpu)
{
    mmu_write(cpu->gb->mmu, cpu->registers.hl, cpu->registers.a);
    cpu->registers.hl -= 1;
    return 2;
}

u8 cpu_ld_a_hli(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    cpu->registers.a = value;
    cpu->registers.hl += 1;
    return 2;
}

u8 cpu_ld_a_hld(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    cpu->registers.a = value;
    cpu->registers.hl -= 1;
    return 2;
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

u8 cpu_adc_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_adc_a(cpu, value);
    return 1;
}

u8 cpu_adc_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    cpu_adc_a(cpu, value);
    return 2;
}

u8 cpu_adc_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_adc_a(cpu, value);
    return 2;
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

u8 cpu_add_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_add_a(cpu, value);
    return 1;
}

u8 cpu_add_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    cpu_add_a(cpu, value);
    return 2;
}

u8 cpu_add_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_add_a(cpu, value);
    return 2;
}

static void cpu_cp_a(struct Cpu *cpu, const u8 value)
{
    const u8 result = cpu->registers.a - value;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, true);
    cpu_set_flag(cpu, FLAG_H, ((cpu->registers.a & 0x0f) - (value & 0x0f)) < 0x00);
    cpu_set_flag(cpu, FLAG_C, cpu->registers.a < value);
}

u8 cpu_cp_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_cp_a(cpu, value);
    return 1;
}

u8 cpu_cp_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    cpu_cp_a(cpu, value);
    return 2;
}

u8 cpu_cp_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_cp_a(cpu, value);
    return 2;
}

static u8 cpu_dec(struct Cpu *cpu, const u8 value)
{
    const u8 result = value - 1;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, true);
    cpu_set_flag(cpu, FLAG_H, (result & 0x0f) == 0x0f);

    return result;
}

u8 cpu_dec_r8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_dec(cpu, value);
    cpu_set_register8(cpu, dst, result);
    return 1;
}

u8 cpu_dec_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    const u8 result = cpu_dec(cpu, value);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, result);
    return 3;
}

static u8 cpu_inc(struct Cpu *cpu, const u8 value)
{
    const u8 result = value + 1;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, (result & 0x0f) == 0x00);

    return result;
}

u8 cpu_inc_r8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_inc(cpu, value);
    cpu_set_register8(cpu, dst, result);
    return 1;
}

u8 cpu_inc_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    const u8 result = cpu_inc(cpu, value);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, result);
    return 3;
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

u8 cpu_sbc_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_sbc_a(cpu, value);
    return 1;
}

u8 cpu_sbc_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    cpu_sbc_a(cpu, value);
    return 2;
}

u8 cpu_sbc_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_sbc_a(cpu, value);
    return 2;
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

u8 cpu_sub_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_sub_a(cpu, value);
    return 1;
}

u8 cpu_sub_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    cpu_sub_a(cpu, value);
    return 2;
}

u8 cpu_sub_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_sub_a(cpu, value);
    return 2;
}

// 16-bit arithmetic instructions
u8 cpu_add_hl_r16(struct Cpu *cpu, const enum Register16 src)
{
    const u16 value = cpu_get_register16(cpu, src);
    const u32 result_full = cpu->registers.hl + value;
    const u16 result = (u16) result_full;

    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, ((cpu->registers.hl & 0x0fff) + (value & 0x0fff)) > 0x0fff);
    cpu_set_flag(cpu, FLAG_C, (result_full & 0x10000) != 0);

    cpu->registers.hl = result;
    return 2;
}

u8 cpu_dec_r16(struct Cpu *cpu, const enum Register16 dst)
{
    const u16 value = cpu_get_register16(cpu, dst);
    const u16 result = value - 1;

    cpu_set_register16(cpu, dst, result);
    return 2;
}

u8 cpu_inc_r16(struct Cpu *cpu, const enum Register16 dst)
{
    const u16 value = cpu_get_register16(cpu, dst);
    const u16 result = value + 1;

    cpu_set_register16(cpu, dst, result);
    return 2;
}

// Bitwise logic instructions
static void cpu_and_a(struct Cpu *cpu, const u8 value)
{
    const u8 result = cpu->registers.a & value;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, true);
    cpu_set_flag(cpu, FLAG_C, false);

    cpu->registers.a = result;
}

u8 cpu_and_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_and_a(cpu, value);
    return 1;
}

u8 cpu_and_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    cpu_and_a(cpu, value);
    return 2;
}

u8 cpu_and_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_and_a(cpu, value);
    return 2;
}

u8 cpu_cpl(struct Cpu *cpu)
{
    const u8 result = ~cpu->registers.a;

    cpu_set_flag(cpu, FLAG_N, true);
    cpu_set_flag(cpu, FLAG_H, true);

    cpu->registers.a = result;
    return 1;
}

static void cpu_or_a(struct Cpu *cpu, const u8 value)
{
    const u8 result = cpu->registers.a | value;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, false);
    cpu_set_flag(cpu, FLAG_C, false);

    cpu->registers.a = result;
}

u8 cpu_or_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_or_a(cpu, value);
    return 1;
}

u8 cpu_or_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    cpu_or_a(cpu, value);
    return 2;
}

u8 cpu_or_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_or_a(cpu, value);
    return 2;
}

static void cpu_xor_a(struct Cpu *cpu, const u8 value)
{
    const u8 result = cpu->registers.a ^ value;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, false);
    cpu_set_flag(cpu, FLAG_C, false);

    cpu->registers.a = result;
}

u8 cpu_xor_a_r8(struct Cpu *cpu, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_xor_a(cpu, value);
    return 1;
}

u8 cpu_xor_a_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    cpu_xor_a(cpu, value);
    return 2;
}

u8 cpu_xor_a_n8(struct Cpu *cpu)
{
    const u8 value = cpu_fetch_u8(cpu);
    cpu_xor_a(cpu, value);
    return 2;
}

// Bit flag instructions
static void cpu_bit_u3(struct Cpu *cpu, const u8 bit, const u8 value)
{
    const bool is_zero = !GB_BIT_CHECK(value, bit);

    cpu_set_flag(cpu, FLAG_Z, is_zero);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, true);
}

u8 cpu_bit_u3_r8(struct Cpu *cpu, const u8 bit, const enum Register8 src)
{
    const u8 value = cpu_get_register8(cpu, src);
    cpu_bit_u3(cpu, bit, value);
    return 2;
}

u8 cpu_bit_u3_hl(struct Cpu *cpu, const u8 bit)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    cpu_bit_u3(cpu, bit, value);
    return 3;
}

static u8 cpu_res_u3(const u8 bit, u8 value)
{
    GB_BIT_CLEAR(value, bit);
    return value;
}

u8 cpu_res_u3_r8(struct Cpu *cpu, const u8 bit, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_res_u3(bit, value);
    cpu_set_register8(cpu, dst, result);
    return 2;
}

u8 cpu_res_u3_hl(struct Cpu *cpu, const u8 bit)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    const u8 result = cpu_res_u3(bit, value);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, result);
    return 4;
}

static u8 cpu_set_u3(const u8 bit, u8 value)
{
    GB_BIT_SET(value, bit);
    return value;
}

u8 cpu_set_u3_r8(struct Cpu *cpu, const u8 bit, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_set_u3(bit, value);
    cpu_set_register8(cpu, dst, result);
    return 2;
}

u8 cpu_set_u3_hl(struct Cpu *cpu, const u8 bit)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    const u8 result = cpu_set_u3(bit, value);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, result);
    return 4;
}

// Bit shift instructions
static u8 cpu_rl(struct Cpu *cpu, const u8 value)
{
    const u8 carry = cpu_is_flag(cpu, FLAG_C);
    const bool will_carry = GB_BIT_CHECK(value, 7);
    const u8 result = (value << 1) | carry;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, false);
    cpu_set_flag(cpu, FLAG_C, will_carry);

    return result;
}

u8 cpu_rl_r8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_rl(cpu, value);
    cpu_set_register8(cpu, dst, result);
    return 2;
}

u8 cpu_rl_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    const u8 result = cpu_rl(cpu, value);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, result);
    return 4;
}

u8 cpu_rla(struct Cpu *cpu)
{
    cpu_rl_r8(cpu, R8_A);
    cpu_set_flag(cpu, FLAG_Z, false);
    return 1;
}

static u8 cpu_rlc(struct Cpu *cpu, const u8 value)
{
    const u8 carry = GB_BIT_CHECK(value, 7);
    const u8 result = (value << 1) | carry;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, false);
    cpu_set_flag(cpu, FLAG_C, carry);

    return result;
}

u8 cpu_rlc_r8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_rlc(cpu, value);
    cpu_set_register8(cpu, dst, result);
    return 2;
}

u8 cpu_rlc_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    const u8 result = cpu_rlc(cpu, value);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, result);
    return 4;
}

u8 cpu_rlca(struct Cpu *cpu)
{
    cpu_rlc_r8(cpu, R8_A);
    cpu_set_flag(cpu, FLAG_Z, false);
    return 1;
}

static u8 cpu_rr(struct Cpu *cpu, const u8 value)
{
    const u8 carry = cpu_is_flag(cpu, FLAG_C);
    const bool will_carry = GB_BIT_CHECK(value, 0);

    const u8 result = (value >> 1) | (carry << 7);

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, false);
    cpu_set_flag(cpu, FLAG_C, will_carry);

    return result;
}

u8 cpu_rr_r8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_rr(cpu, value);
    cpu_set_register8(cpu, dst, result);
    return 2;
}

u8 cpu_rr_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    const u8 result = cpu_rr(cpu, value);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, result);
    return 4;
}

u8 cpu_rra(struct Cpu *cpu)
{
    cpu_rr_r8(cpu, R8_A);
    cpu_set_flag(cpu, FLAG_Z, false);
    return 1;
}

static u8 cpu_rrc(struct Cpu *cpu, const u8 value)
{
    const u8 carry = GB_BIT_CHECK(value, 0);
    const u8 result = (value >> 1) | (carry << 7);

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, false);
    cpu_set_flag(cpu, FLAG_C, carry);

    return result;
}

u8 cpu_rrc_r8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_rrc(cpu, value);
    cpu_set_register8(cpu, dst, result);
    return 2;
}

u8 cpu_rrc_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    const u8 result = cpu_rrc(cpu, value);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, result);
    return 4;
}

u8 cpu_rrca(struct Cpu *cpu)
{
    cpu_rrc_r8(cpu, R8_A);
    cpu_set_flag(cpu, FLAG_Z, false);
    return 1;
}

static u8 cpu_sla(struct Cpu *cpu, const u8 value)
{
    const u8 carry = GB_BIT_CHECK(value, 7);
    const u8 result = value << 1;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, false);
    cpu_set_flag(cpu, FLAG_C, carry);

    return result;
}

u8 cpu_sla_r8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_sla(cpu, value);
    cpu_set_register8(cpu, dst, result);
    return 2;
}

u8 cpu_sla_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    const u8 result = cpu_sla(cpu, value);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, result);
    return 4;
}

static u8 cpu_sra(struct Cpu *cpu, const u8 value)
{
    const u8 carry = GB_BIT_CHECK(value, 0);
    const u8 top_bit = GB_BIT_CHECK(value, 7);

    u8 result = value >> 1;
    if (top_bit)
    {
        GB_BIT_SET(result, 7);
    }
    else
    {
        GB_BIT_CLEAR(result, 7);
    }

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, false);
    cpu_set_flag(cpu, FLAG_C, carry);

    return result;
}

u8 cpu_sra_r8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_sra(cpu, value);
    cpu_set_register8(cpu, dst, result);
    return 2;
}

u8 cpu_sra_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    const u8 result = cpu_sra(cpu, value);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, result);
    return 4;
}

static u8 cpu_srl(struct Cpu *cpu, const u8 value)
{
    const u8 carry = GB_BIT_CHECK(value, 0);
    const u8 result = value >> 1;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, false);
    cpu_set_flag(cpu, FLAG_C, carry);

    return result;
}

u8 cpu_srl_r8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_srl(cpu, value);
    cpu_set_register8(cpu, dst, result);
    return 2;
}

u8 cpu_srl_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    const u8 result = cpu_srl(cpu, value);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, result);
    return 4;
}

static u8 cpu_swap(struct Cpu *cpu, const u8 value)
{
    const u8 lower_nibble = value & 0x0f;
    const u8 upper_nibble = (value & 0xf0) >> 4;
    const u8 result = (lower_nibble << 4) | upper_nibble;

    cpu_set_flag(cpu, FLAG_Z, result == 0);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, false);
    cpu_set_flag(cpu, FLAG_C, false);

    return result;
}

u8 cpu_swap_r8(struct Cpu *cpu, const enum Register8 dst)
{
    const u8 value = cpu_get_register8(cpu, dst);
    const u8 result = cpu_swap(cpu, value);
    cpu_set_register8(cpu, dst, result);
    return 2;
}

u8 cpu_swap_hl(struct Cpu *cpu)
{
    const u8 value = mmu_read(cpu->gb->mmu, cpu->registers.hl);
    const u8 result = cpu_swap(cpu, value);
    mmu_write(cpu->gb->mmu, cpu->registers.hl, result);
    return 4;
}

// Jumps and subroutine instructions

u8 cpu_call_n16(struct Cpu *cpu)
{
    const u16 address = cpu_fetch_u16(cpu);
    cpu_push_stack(cpu, cpu->registers.pc);
    cpu->registers.pc = address;
    return 6;
}

u8 cpu_call_cc_n16(struct Cpu *cpu, const enum ConditionCode cc)
{
    const u16 address = cpu_fetch_u16(cpu);

    if (!cpu_is_condition(cpu, cc))
    {
        return 3;
    }

    cpu_push_stack(cpu, cpu->registers.pc);
    cpu->registers.pc = address;
    return 6;
}

u8 cpu_jp_hl(struct Cpu *cpu)
{
    cpu->registers.pc = cpu->registers.hl;
    return 1;
}

u8 cpu_jp_n16(struct Cpu *cpu)
{
    const u16 address = cpu_fetch_u16(cpu);
    cpu->registers.pc = address;
    return 4;
}

u8 cpu_jp_cc_n16(struct Cpu *cpu, const enum ConditionCode cc)
{
    const u16 address = cpu_fetch_u16(cpu);
    if (!cpu_is_condition(cpu, cc))
    {
        return 3;
    }

    cpu->registers.pc = address;
    return 4;
}

u8 cpu_jr_i8(struct Cpu *cpu)
{
    const i8 offset = cpu_fetch_i8(cpu);
    const u16 address = (u16) ((i32) cpu->registers.pc + offset);
    cpu->registers.pc = address;
    return 3;
}

u8 cpu_jr_cc_i8(struct Cpu *cpu, const enum ConditionCode cc)
{
    const i8 offset = cpu_fetch_i8(cpu);
    if (!cpu_is_condition(cpu, cc))
    {
        return 2;
    }

    const u16 address = (u16) ((i32) cpu->registers.pc + offset);
    cpu->registers.pc = address;
    return 3;
}

u8 cpu_ret_cc(struct Cpu *cpu, const enum ConditionCode cc)
{
    if (!cpu_is_condition(cpu, cc))
    {
        return 2;
    }

    const u16 address = cpu_pop_stack(cpu);
    cpu->registers.pc = address;
    return 5;
}

u8 cpu_ret(struct Cpu *cpu)
{
    const u16 address = cpu_pop_stack(cpu);
    cpu->registers.pc = address;
    return 4;
}

u8 cpu_reti(struct Cpu *cpu)
{
    cpu->ime_delay = 1;
    cpu_ret(cpu);
    return 4;
}

u8 cpu_rst_vec(struct Cpu *cpu, const enum Rst vec)
{
    cpu_push_stack(cpu, cpu->registers.pc);
    cpu->registers.pc = (u16) vec;
    return 4;
}

// Carry flag instructions
u8 cpu_ccf(struct Cpu *cpu)
{
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, false);
    cpu_set_flag(cpu, FLAG_C, !cpu_is_flag(cpu, FLAG_C));
    return 1;
}

u8 cpu_scf(struct Cpu *cpu)
{
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, false);
    cpu_set_flag(cpu, FLAG_C, true);
    return 1;
}

// Stack manipulation instructions
u8 cpu_add_hl_sp(struct Cpu *cpu)
{
    const u16 value = cpu->registers.sp;
    const u32 result_full = cpu->registers.hl + value;
    const u16 result = (u16) result_full;

    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, ((cpu->registers.hl & 0x0fff) + (value & 0x0fff)) > 0x0fff);
    cpu_set_flag(cpu, FLAG_C, (result_full & 0x10000) != 0);

    cpu->registers.hl = result;
    return 2;
}

u8 cpu_add_sp_i8(struct Cpu *cpu)
{
    const i8 value = cpu_fetch_i8(cpu);
    const i32 result = cpu->registers.sp + value;

    cpu_set_flag(cpu, FLAG_Z, false);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, ((cpu->registers.sp ^ value ^ (result & 0xffff)) & 0x0010) == 0x0010);
    cpu_set_flag(cpu, FLAG_C, ((cpu->registers.sp ^ value ^ (result & 0xffff)) & 0x0100) == 0x0100);

    cpu->registers.sp = (u16) result;
    return 4;
}

u8 cpu_dec_sp(struct Cpu *cpu)
{
    cpu->registers.sp -= 1;
    return 2;
}

u8 cpu_inc_sp(struct Cpu *cpu)
{
    cpu->registers.sp += 1;
    return 2;
}

u8 cpu_ld_sp_n16(struct Cpu *cpu)
{
    const u16 value = cpu_fetch_u16(cpu);
    cpu->registers.sp = value;
    return 3;
}

u8 cpu_ld_n16_sp(struct Cpu *cpu)
{
    const u16 address = cpu_fetch_u16(cpu);

    mmu_write(cpu->gb->mmu, address, cpu->registers.sp & 0xff);
    mmu_write(cpu->gb->mmu, address + 1, cpu->registers.sp >> 8);
    return 5;
}

u8 cpu_ld_hl_sp_i8(struct Cpu *cpu)
{
    const i8 value = cpu_fetch_i8(cpu);
    const i32 result = cpu->registers.sp + value;

    cpu_set_flag(cpu, FLAG_Z, false);
    cpu_set_flag(cpu, FLAG_N, false);
    cpu_set_flag(cpu, FLAG_H, ((cpu->registers.sp ^ value ^ (result & 0xffff)) & 0x0010) == 0x0010);
    cpu_set_flag(cpu, FLAG_C, ((cpu->registers.sp ^ value ^ (result & 0xffff)) & 0x0100) == 0x0100);

    cpu->registers.hl = (u16) result;
    return 3;
}

u8 cpu_ld_sp_hl(struct Cpu *cpu)
{
    cpu->registers.sp = cpu->registers.hl;
    return 2;
}

u8 cpu_pop_af(struct Cpu *cpu)
{
    cpu->registers.af = cpu_pop_stack(cpu) & 0xfff0;
    return 3;
}

u8 cpu_pop_r16(struct Cpu *cpu, const enum Register16 dst)
{
    cpu_set_register16(cpu, dst, cpu_pop_stack(cpu));
    return 3;
}

u8 cpu_push_af(struct Cpu *cpu)
{
    cpu_push_stack(cpu, cpu->registers.af & 0xfff0);
    return 4;
}

u8 cpu_push_r16(struct Cpu *cpu, const enum Register16 src)
{
    cpu_push_stack(cpu, cpu_get_register16(cpu, src));
    return 4;
}

// Interrupt-related instructions

u8 cpu_di(struct Cpu *cpu)
{
    cpu->interrupt_master_enable = false;
    return 1;
}

u8 cpu_ei(struct Cpu *cpu)
{
    cpu->ime_delay = 2;
    return 1;
}

u8 cpu_halt(struct Cpu *cpu)
{
    (void) cpu;
    // FIXME: Implement me
    return 0;
}

// Miscellaneous instructions

/// DAA
u8 cpu_daa(struct Cpu *cpu)
{
    u8 value = cpu->registers.a;
    u16 correction = cpu_is_flag(cpu, FLAG_C) ? 0x60 : 0x00;
    if (cpu_is_flag(cpu, FLAG_H) || (!cpu_is_flag(cpu, FLAG_N) && ((value & 0x0f) > 9)))
    {
        correction |= 0x06;
    }

    if (cpu_is_flag(cpu, FLAG_C) || (!cpu_is_flag(cpu, FLAG_N) && (value > 0x99)))
    {
        correction |= 0x60;
    }

    if (cpu_is_flag(cpu, FLAG_N))
    {
        value = (u8) (value - correction);
    }
    else
    {
        value = (u8) (value + correction);
    }

    cpu_set_flag(cpu, FLAG_Z, value == 0);
    cpu_set_flag(cpu, FLAG_H, false);

    if (((correction << 2) & 0x0100) != 0)
    {
        cpu_set_flag(cpu, FLAG_C, true);
    }

    cpu->registers.a = value;
    return 1;
}

/// NOP
u8 cpu_nop(struct Cpu *cpu)
{
    (void) cpu;
    return 1;
}

/// STOP
u8 cpu_stop(struct Cpu *cpu)
{
    (void) cpu;
    // FIXME: Implement me
    return 0;
}
