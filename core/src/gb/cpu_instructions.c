/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/cpu_instructions.h"

#include "gb/gb.h"
#include "gb/mmu.h"
#include "gb/utils/bits.h"

// Load instructions
uint8_t cpu_ld_r8_r8(struct GbCpu *cpu, const enum GbRegister8 dst, const enum GbRegister8 src)
{
    const uint8_t value = gb_cpu_get_register8(cpu, src);
    gb_cpu_set_register8(cpu, dst, value);
    return 1;
}

uint8_t cpu_ld_r8_n8(struct GbCpu *cpu, const enum GbRegister8 dst)
{
    const uint8_t src = gb_cpu_fetch_u8(cpu);
    gb_cpu_set_register8(cpu, dst, src);
    return 2;
}

uint8_t cpu_ld_r16_n16(struct GbCpu *cpu, const enum GbRegister16 dst)
{
    const uint16_t src = gb_cpu_fetch_u16(cpu);
    gb_cpu_set_register16(cpu, dst, src);
    return 3;
}

uint8_t cpu_ld_hl_r8(struct GbCpu *cpu, const enum GbRegister8 src)
{
    const uint8_t value = gb_cpu_get_register8(cpu, src);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, value);
    return 2;
}

uint8_t cpu_ld_hl_n8(struct GbCpu *cpu)
{
    const uint8_t src = gb_cpu_fetch_u8(cpu);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, src);
    return 3;
}

uint8_t cpu_ld_r8_hl(struct GbCpu *cpu, const enum GbRegister8 dst)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    gb_cpu_set_register8(cpu, dst, value);
    return 2;
}

uint8_t cpu_ld_r16_a(struct GbCpu *cpu, const enum GbRegister16 dst)
{
    const uint16_t address = gb_cpu_get_register16(cpu, dst);
    gb_mmu_write(cpu->mmu, address, cpu->registers.a);
    return 2;
}

uint8_t cpu_ld_n16_a(struct GbCpu *cpu)
{
    const uint16_t dst = gb_cpu_fetch_u16(cpu);
    gb_mmu_write(cpu->mmu, dst, cpu->registers.a);
    return 4;
}

uint8_t cpu_ldh_n8_a(struct GbCpu *cpu)
{
    const uint8_t dst = gb_cpu_fetch_u8(cpu);
    const uint16_t address = 0xff00 + dst;
    gb_mmu_write(cpu->mmu, address, cpu->registers.a);
    return 3;
}

uint8_t cpu_ldh_c_a(struct GbCpu *cpu)
{
    const uint16_t address = 0xff00 + cpu->registers.c;
    gb_mmu_write(cpu->mmu, address, cpu->registers.a);
    return 2;
}

uint8_t cpu_ld_a_r16(struct GbCpu *cpu, const enum GbRegister16 src)
{
    const uint16_t address = gb_cpu_get_register16(cpu, src);
    const uint8_t value = gb_mmu_read(cpu->mmu, address);
    cpu->registers.a = value;
    return 2;
}

uint8_t cpu_ld_a_n16(struct GbCpu *cpu)
{
    const uint16_t src = gb_cpu_fetch_u16(cpu);
    const uint8_t value = gb_mmu_read(cpu->mmu, src);
    cpu->registers.a = value;
    return 4;
}

uint8_t cpu_ldh_a_n8(struct GbCpu *cpu)
{
    const uint16_t src = gb_cpu_fetch_u8(cpu);
    const uint16_t address = 0xff00 + src;
    const uint8_t value = gb_mmu_read(cpu->mmu, address);
    cpu->registers.a = value;
    return 3;
}

uint8_t cpu_ldh_a_c(struct GbCpu *cpu)
{
    const uint16_t address = 0xff00 + cpu->registers.c;
    const uint8_t value = gb_mmu_read(cpu->mmu, address);
    cpu->registers.a = value;
    return 2;
}

uint8_t cpu_ld_hli_a(struct GbCpu *cpu)
{
    gb_mmu_write(cpu->mmu, cpu->registers.hl, cpu->registers.a);
    cpu->registers.hl += 1;
    return 2;
}

uint8_t cpu_ld_hld_a(struct GbCpu *cpu)
{
    gb_mmu_write(cpu->mmu, cpu->registers.hl, cpu->registers.a);
    cpu->registers.hl -= 1;
    return 2;
}

uint8_t cpu_ld_a_hli(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    cpu->registers.a = value;
    cpu->registers.hl += 1;
    return 2;
}

uint8_t cpu_ld_a_hld(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    cpu->registers.a = value;
    cpu->registers.hl -= 1;
    return 2;
}

// 8-bit arithmetic instructions
static void cpu_adc_a(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t carry = gb_cpu_is_flag(cpu, GB_FLAG_C);
    const uint32_t result_full = cpu->registers.a + value + carry;
    const uint8_t result = (uint8_t) result_full;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, ((cpu->registers.a & 0x0f) + (value & 0x0f) + carry) > 0x0f);
    gb_cpu_set_flag(cpu, GB_FLAG_C, result_full > 0xff);

    cpu->registers.a = result;
}

uint8_t cpu_adc_a_r8(struct GbCpu *cpu, const enum GbRegister8 src)
{
    const uint8_t value = gb_cpu_get_register8(cpu, src);
    cpu_adc_a(cpu, value);
    return 1;
}

uint8_t cpu_adc_a_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_adc_a(cpu, value);
    return 2;
}

uint8_t cpu_adc_a_n8(struct GbCpu *cpu)
{
    const uint8_t value = gb_cpu_fetch_u8(cpu);
    cpu_adc_a(cpu, value);
    return 2;
}

static void cpu_add_a(struct GbCpu *cpu, const uint8_t value)
{
    const uint32_t result_full = cpu->registers.a + value;
    const uint8_t result = (uint8_t) result_full;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, ((cpu->registers.a & 0x0f) + (value & 0x0f)) > 0x0f);
    gb_cpu_set_flag(cpu, GB_FLAG_C, result_full > 0xff);

    cpu->registers.a = result;
}

uint8_t cpu_add_a_r8(struct GbCpu *cpu, const enum GbRegister8 src)
{
    const uint8_t value = gb_cpu_get_register8(cpu, src);
    cpu_add_a(cpu, value);
    return 1;
}

uint8_t cpu_add_a_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_add_a(cpu, value);
    return 2;
}

uint8_t cpu_add_a_n8(struct GbCpu *cpu)
{
    const uint8_t value = gb_cpu_fetch_u8(cpu);
    cpu_add_a(cpu, value);
    return 2;
}

static void cpu_cp_a(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t result = cpu->registers.a - value;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, true);
    gb_cpu_set_flag(cpu, GB_FLAG_H, ((cpu->registers.a & 0x0f) - (value & 0x0f)) < 0x00);
    gb_cpu_set_flag(cpu, GB_FLAG_C, cpu->registers.a < value);
}

uint8_t cpu_cp_a_r8(struct GbCpu *cpu, const enum GbRegister8 src)
{
    const uint8_t value = gb_cpu_get_register8(cpu, src);
    cpu_cp_a(cpu, value);
    return 1;
}

uint8_t cpu_cp_a_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_cp_a(cpu, value);
    return 2;
}

uint8_t cpu_cp_a_n8(struct GbCpu *cpu)
{
    const uint8_t value = gb_cpu_fetch_u8(cpu);
    cpu_cp_a(cpu, value);
    return 2;
}

static uint8_t cpu_dec(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t result = value - 1;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, true);
    gb_cpu_set_flag(cpu, GB_FLAG_H, (result & 0x0f) == 0x0f);

    return result;
}

uint8_t cpu_dec_r8(struct GbCpu *cpu, const enum GbRegister8 dst)
{
    const uint8_t value = gb_cpu_get_register8(cpu, dst);
    const uint8_t result = cpu_dec(cpu, value);
    gb_cpu_set_register8(cpu, dst, result);
    return 1;
}

uint8_t cpu_dec_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    const uint8_t result = cpu_dec(cpu, value);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, result);
    return 3;
}

static uint8_t cpu_inc(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t result = value + 1;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, (result & 0x0f) == 0x00);

    return result;
}

uint8_t cpu_inc_r8(struct GbCpu *cpu, const enum GbRegister8 dst)
{
    const uint8_t value = gb_cpu_get_register8(cpu, dst);
    const uint8_t result = cpu_inc(cpu, value);
    gb_cpu_set_register8(cpu, dst, result);
    return 1;
}

uint8_t cpu_inc_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    const uint8_t result = cpu_inc(cpu, value);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, result);
    return 3;
}

static void cpu_sbc_a(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t carry = gb_cpu_is_flag(cpu, GB_FLAG_C);
    const int32_t result_full = cpu->registers.a - value - carry;
    const uint8_t result = (uint8_t) result_full;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, true);
    gb_cpu_set_flag(cpu, GB_FLAG_H, ((cpu->registers.a & 0x0f) - (value & 0x0f) - carry) < 0x00);
    gb_cpu_set_flag(cpu, GB_FLAG_C, result_full < 0x00);

    cpu->registers.a = result;
}

uint8_t cpu_sbc_a_r8(struct GbCpu *cpu, const enum GbRegister8 src)
{
    const uint8_t value = gb_cpu_get_register8(cpu, src);
    cpu_sbc_a(cpu, value);
    return 1;
}

uint8_t cpu_sbc_a_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_sbc_a(cpu, value);
    return 2;
}

uint8_t cpu_sbc_a_n8(struct GbCpu *cpu)
{
    const uint8_t value = gb_cpu_fetch_u8(cpu);
    cpu_sbc_a(cpu, value);
    return 2;
}

static void cpu_sub_a(struct GbCpu *cpu, const uint8_t value)
{
    const int32_t result_full = cpu->registers.a - value;
    const uint8_t result = (uint8_t) result_full;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, true);
    gb_cpu_set_flag(cpu, GB_FLAG_H, ((cpu->registers.a & 0x0f) - (value & 0x0f)) < 0x00);
    gb_cpu_set_flag(cpu, GB_FLAG_C, result_full < 0x00);

    cpu->registers.a = result;
}

uint8_t cpu_sub_a_r8(struct GbCpu *cpu, const enum GbRegister8 src)
{
    const uint8_t value = gb_cpu_get_register8(cpu, src);
    cpu_sub_a(cpu, value);
    return 1;
}

uint8_t cpu_sub_a_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_sub_a(cpu, value);
    return 2;
}

uint8_t cpu_sub_a_n8(struct GbCpu *cpu)
{
    const uint8_t value = gb_cpu_fetch_u8(cpu);
    cpu_sub_a(cpu, value);
    return 2;
}

// 16-bit arithmetic instructions
uint8_t cpu_add_hl_r16(struct GbCpu *cpu, const enum GbRegister16 src)
{
    const uint16_t value = gb_cpu_get_register16(cpu, src);
    const uint32_t result_full = cpu->registers.hl + value;
    const uint16_t result = (uint16_t) result_full;

    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, ((cpu->registers.hl & 0x0fff) + (value & 0x0fff)) > 0x0fff);
    gb_cpu_set_flag(cpu, GB_FLAG_C, (result_full & 0x10000) != 0);

    cpu->registers.hl = result;
    return 2;
}

uint8_t cpu_dec_r16(struct GbCpu *cpu, const enum GbRegister16 dst)
{
    const uint16_t value = gb_cpu_get_register16(cpu, dst);
    const uint16_t result = value - 1;

    gb_cpu_set_register16(cpu, dst, result);
    return 2;
}

uint8_t cpu_inc_r16(struct GbCpu *cpu, const enum GbRegister16 dst)
{
    const uint16_t value = gb_cpu_get_register16(cpu, dst);
    const uint16_t result = value + 1;

    gb_cpu_set_register16(cpu, dst, result);
    return 2;
}

// Bitwise logic instructions
static void cpu_and_a(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t result = cpu->registers.a & value;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, true);
    gb_cpu_set_flag(cpu, GB_FLAG_C, false);

    cpu->registers.a = result;
}

uint8_t cpu_and_a_r8(struct GbCpu *cpu, const enum GbRegister8 src)
{
    const uint8_t value = gb_cpu_get_register8(cpu, src);
    cpu_and_a(cpu, value);
    return 1;
}

uint8_t cpu_and_a_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_and_a(cpu, value);
    return 2;
}

uint8_t cpu_and_a_n8(struct GbCpu *cpu)
{
    const uint8_t value = gb_cpu_fetch_u8(cpu);
    cpu_and_a(cpu, value);
    return 2;
}

uint8_t cpu_cpl(struct GbCpu *cpu)
{
    const uint8_t result = ~cpu->registers.a;

    gb_cpu_set_flag(cpu, GB_FLAG_N, true);
    gb_cpu_set_flag(cpu, GB_FLAG_H, true);

    cpu->registers.a = result;
    return 1;
}

static void cpu_or_a(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t result = cpu->registers.a | value;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);
    gb_cpu_set_flag(cpu, GB_FLAG_C, false);

    cpu->registers.a = result;
}

uint8_t cpu_or_a_r8(struct GbCpu *cpu, const enum GbRegister8 src)
{
    const uint8_t value = gb_cpu_get_register8(cpu, src);
    cpu_or_a(cpu, value);
    return 1;
}

uint8_t cpu_or_a_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_or_a(cpu, value);
    return 2;
}

uint8_t cpu_or_a_n8(struct GbCpu *cpu)
{
    const uint8_t value = gb_cpu_fetch_u8(cpu);
    cpu_or_a(cpu, value);
    return 2;
}

static void cpu_xor_a(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t result = cpu->registers.a ^ value;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);
    gb_cpu_set_flag(cpu, GB_FLAG_C, false);

    cpu->registers.a = result;
}

uint8_t cpu_xor_a_r8(struct GbCpu *cpu, const enum GbRegister8 src)
{
    const uint8_t value = gb_cpu_get_register8(cpu, src);
    cpu_xor_a(cpu, value);
    return 1;
}

uint8_t cpu_xor_a_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_xor_a(cpu, value);
    return 2;
}

uint8_t cpu_xor_a_n8(struct GbCpu *cpu)
{
    const uint8_t value = gb_cpu_fetch_u8(cpu);
    cpu_xor_a(cpu, value);
    return 2;
}

// Bit flag instructions
static void cpu_bit_u3(struct GbCpu *cpu, const uint8_t bit, const uint8_t value)
{
    const bool is_zero = !GB_BIT_CHECK(value, bit);

    gb_cpu_set_flag(cpu, GB_FLAG_Z, is_zero);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, true);
}

uint8_t cpu_bit_u3_r8(struct GbCpu *cpu, const uint8_t bit, const enum GbRegister8 src)
{
    const uint8_t value = gb_cpu_get_register8(cpu, src);
    cpu_bit_u3(cpu, bit, value);
    return 2;
}

uint8_t cpu_bit_u3_hl(struct GbCpu *cpu, const uint8_t bit)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    cpu_bit_u3(cpu, bit, value);
    return 3;
}

static uint8_t cpu_res_u3(const uint8_t bit, uint8_t value)
{
    GB_BIT_CLEAR(value, bit);
    return value;
}

uint8_t cpu_res_u3_r8(struct GbCpu *cpu, const uint8_t bit, const enum GbRegister8 dst)
{
    const uint8_t value = gb_cpu_get_register8(cpu, dst);
    const uint8_t result = cpu_res_u3(bit, value);
    gb_cpu_set_register8(cpu, dst, result);
    return 2;
}

uint8_t cpu_res_u3_hl(struct GbCpu *cpu, const uint8_t bit)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    const uint8_t result = cpu_res_u3(bit, value);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, result);
    return 4;
}

static uint8_t cpu_set_u3(const uint8_t bit, uint8_t value)
{
    GB_BIT_SET(value, bit);
    return value;
}

uint8_t cpu_set_u3_r8(struct GbCpu *cpu, const uint8_t bit, const enum GbRegister8 dst)
{
    const uint8_t value = gb_cpu_get_register8(cpu, dst);
    const uint8_t result = cpu_set_u3(bit, value);
    gb_cpu_set_register8(cpu, dst, result);
    return 2;
}

uint8_t cpu_set_u3_hl(struct GbCpu *cpu, const uint8_t bit)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    const uint8_t result = cpu_set_u3(bit, value);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, result);
    return 4;
}

// Bit shift instructions
static uint8_t cpu_rl(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t carry = gb_cpu_is_flag(cpu, GB_FLAG_C);
    const bool will_carry = GB_BIT_CHECK(value, 7);
    const uint8_t result = (value << 1) | carry;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);
    gb_cpu_set_flag(cpu, GB_FLAG_C, will_carry);

    return result;
}

uint8_t cpu_rl_r8(struct GbCpu *cpu, const enum GbRegister8 dst)
{
    const uint8_t value = gb_cpu_get_register8(cpu, dst);
    const uint8_t result = cpu_rl(cpu, value);
    gb_cpu_set_register8(cpu, dst, result);
    return 2;
}

uint8_t cpu_rl_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    const uint8_t result = cpu_rl(cpu, value);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, result);
    return 4;
}

uint8_t cpu_rla(struct GbCpu *cpu)
{
    cpu_rl_r8(cpu, GB_R8_A);
    gb_cpu_set_flag(cpu, GB_FLAG_Z, false);
    return 1;
}

static uint8_t cpu_rlc(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t carry = GB_BIT_CHECK(value, 7);
    const uint8_t result = (value << 1) | carry;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);
    gb_cpu_set_flag(cpu, GB_FLAG_C, carry);

    return result;
}

uint8_t cpu_rlc_r8(struct GbCpu *cpu, const enum GbRegister8 dst)
{
    const uint8_t value = gb_cpu_get_register8(cpu, dst);
    const uint8_t result = cpu_rlc(cpu, value);
    gb_cpu_set_register8(cpu, dst, result);
    return 2;
}

uint8_t cpu_rlc_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    const uint8_t result = cpu_rlc(cpu, value);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, result);
    return 4;
}

uint8_t cpu_rlca(struct GbCpu *cpu)
{
    cpu_rlc_r8(cpu, GB_R8_A);
    gb_cpu_set_flag(cpu, GB_FLAG_Z, false);
    return 1;
}

static uint8_t cpu_rr(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t carry = gb_cpu_is_flag(cpu, GB_FLAG_C);
    const bool will_carry = GB_BIT_CHECK(value, 0);

    const uint8_t result = (value >> 1) | (carry << 7);

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);
    gb_cpu_set_flag(cpu, GB_FLAG_C, will_carry);

    return result;
}

uint8_t cpu_rr_r8(struct GbCpu *cpu, const enum GbRegister8 dst)
{
    const uint8_t value = gb_cpu_get_register8(cpu, dst);
    const uint8_t result = cpu_rr(cpu, value);
    gb_cpu_set_register8(cpu, dst, result);
    return 2;
}

uint8_t cpu_rr_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    const uint8_t result = cpu_rr(cpu, value);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, result);
    return 4;
}

uint8_t cpu_rra(struct GbCpu *cpu)
{
    cpu_rr_r8(cpu, GB_R8_A);
    gb_cpu_set_flag(cpu, GB_FLAG_Z, false);
    return 1;
}

static uint8_t cpu_rrc(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t carry = GB_BIT_CHECK(value, 0);
    const uint8_t result = (value >> 1) | (carry << 7);

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);
    gb_cpu_set_flag(cpu, GB_FLAG_C, carry);

    return result;
}

uint8_t cpu_rrc_r8(struct GbCpu *cpu, const enum GbRegister8 dst)
{
    const uint8_t value = gb_cpu_get_register8(cpu, dst);
    const uint8_t result = cpu_rrc(cpu, value);
    gb_cpu_set_register8(cpu, dst, result);
    return 2;
}

uint8_t cpu_rrc_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    const uint8_t result = cpu_rrc(cpu, value);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, result);
    return 4;
}

uint8_t cpu_rrca(struct GbCpu *cpu)
{
    cpu_rrc_r8(cpu, GB_R8_A);
    gb_cpu_set_flag(cpu, GB_FLAG_Z, false);
    return 1;
}

static uint8_t cpu_sla(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t carry = GB_BIT_CHECK(value, 7);
    const uint8_t result = value << 1;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);
    gb_cpu_set_flag(cpu, GB_FLAG_C, carry);

    return result;
}

uint8_t cpu_sla_r8(struct GbCpu *cpu, const enum GbRegister8 dst)
{
    const uint8_t value = gb_cpu_get_register8(cpu, dst);
    const uint8_t result = cpu_sla(cpu, value);
    gb_cpu_set_register8(cpu, dst, result);
    return 2;
}

uint8_t cpu_sla_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    const uint8_t result = cpu_sla(cpu, value);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, result);
    return 4;
}

static uint8_t cpu_sra(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t carry = GB_BIT_CHECK(value, 0);
    const uint8_t top_bit = GB_BIT_CHECK(value, 7);

    uint8_t result = value >> 1;
    if (top_bit)
    {
        GB_BIT_SET(result, 7);
    }
    else
    {
        GB_BIT_CLEAR(result, 7);
    }

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);
    gb_cpu_set_flag(cpu, GB_FLAG_C, carry);

    return result;
}

uint8_t cpu_sra_r8(struct GbCpu *cpu, const enum GbRegister8 dst)
{
    const uint8_t value = gb_cpu_get_register8(cpu, dst);
    const uint8_t result = cpu_sra(cpu, value);
    gb_cpu_set_register8(cpu, dst, result);
    return 2;
}

uint8_t cpu_sra_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    const uint8_t result = cpu_sra(cpu, value);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, result);
    return 4;
}

static uint8_t cpu_srl(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t carry = GB_BIT_CHECK(value, 0);
    const uint8_t result = value >> 1;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);
    gb_cpu_set_flag(cpu, GB_FLAG_C, carry);

    return result;
}

uint8_t cpu_srl_r8(struct GbCpu *cpu, const enum GbRegister8 dst)
{
    const uint8_t value = gb_cpu_get_register8(cpu, dst);
    const uint8_t result = cpu_srl(cpu, value);
    gb_cpu_set_register8(cpu, dst, result);
    return 2;
}

uint8_t cpu_srl_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    const uint8_t result = cpu_srl(cpu, value);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, result);
    return 4;
}

static uint8_t cpu_swap(struct GbCpu *cpu, const uint8_t value)
{
    const uint8_t lower_nibble = value & 0x0f;
    const uint8_t upper_nibble = (value & 0xf0) >> 4;
    const uint8_t result = (lower_nibble << 4) | upper_nibble;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, result == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);
    gb_cpu_set_flag(cpu, GB_FLAG_C, false);

    return result;
}

uint8_t cpu_swap_r8(struct GbCpu *cpu, const enum GbRegister8 dst)
{
    const uint8_t value = gb_cpu_get_register8(cpu, dst);
    const uint8_t result = cpu_swap(cpu, value);
    gb_cpu_set_register8(cpu, dst, result);
    return 2;
}

uint8_t cpu_swap_hl(struct GbCpu *cpu)
{
    const uint8_t value = gb_mmu_read(cpu->mmu, cpu->registers.hl);
    const uint8_t result = cpu_swap(cpu, value);
    gb_mmu_write(cpu->mmu, cpu->registers.hl, result);
    return 4;
}

// Jumps and subroutine instructions

uint8_t cpu_call_n16(struct GbCpu *cpu)
{
    const uint16_t address = gb_cpu_fetch_u16(cpu);
    gb_cpu_push_stack(cpu, cpu->registers.pc);
    cpu->registers.pc = address;
    return 6;
}

uint8_t cpu_call_cc_n16(struct GbCpu *cpu, const enum GbConditionCode cc)
{
    const uint16_t address = gb_cpu_fetch_u16(cpu);

    if (!gb_cpu_is_condition(cpu, cc))
    {
        return 3;
    }

    gb_cpu_push_stack(cpu, cpu->registers.pc);
    cpu->registers.pc = address;
    return 6;
}

uint8_t cpu_jp_hl(struct GbCpu *cpu)
{
    cpu->registers.pc = cpu->registers.hl;
    return 1;
}

uint8_t cpu_jp_n16(struct GbCpu *cpu)
{
    const uint16_t address = gb_cpu_fetch_u16(cpu);
    cpu->registers.pc = address;
    return 4;
}

uint8_t cpu_jp_cc_n16(struct GbCpu *cpu, const enum GbConditionCode cc)
{
    const uint16_t address = gb_cpu_fetch_u16(cpu);
    if (!gb_cpu_is_condition(cpu, cc))
    {
        return 3;
    }

    cpu->registers.pc = address;
    return 4;
}

uint8_t cpu_jr_i8(struct GbCpu *cpu)
{
    const int8_t offset = gb_cpu_fetch_i8(cpu);
    const uint16_t address = (uint16_t) ((int32_t) cpu->registers.pc + offset);
    cpu->registers.pc = address;
    return 3;
}

uint8_t cpu_jr_cc_i8(struct GbCpu *cpu, const enum GbConditionCode cc)
{
    const int8_t offset = gb_cpu_fetch_i8(cpu);
    if (!gb_cpu_is_condition(cpu, cc))
    {
        return 2;
    }

    const uint16_t address = (uint16_t) ((int32_t) cpu->registers.pc + offset);
    cpu->registers.pc = address;
    return 3;
}

uint8_t cpu_ret_cc(struct GbCpu *cpu, const enum GbConditionCode cc)
{
    if (!gb_cpu_is_condition(cpu, cc))
    {
        return 2;
    }

    const uint16_t address = gb_cpu_pop_stack(cpu);
    cpu->registers.pc = address;
    return 5;
}

uint8_t cpu_ret(struct GbCpu *cpu)
{
    const uint16_t address = gb_cpu_pop_stack(cpu);
    cpu->registers.pc = address;
    return 4;
}

uint8_t cpu_reti(struct GbCpu *cpu)
{
    cpu->ime_delay = 1;
    cpu_ret(cpu);
    return 4;
}

uint8_t cpu_rst_vec(struct GbCpu *cpu, const enum GbRst vec)
{
    gb_cpu_push_stack(cpu, cpu->registers.pc);
    cpu->registers.pc = (uint16_t) vec;
    return 4;
}

// Carry flag instructions
uint8_t cpu_ccf(struct GbCpu *cpu)
{
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);
    gb_cpu_set_flag(cpu, GB_FLAG_C, !gb_cpu_is_flag(cpu, GB_FLAG_C));
    return 1;
}

uint8_t cpu_scf(struct GbCpu *cpu)
{
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);
    gb_cpu_set_flag(cpu, GB_FLAG_C, true);
    return 1;
}

// Stack manipulation instructions
uint8_t cpu_add_hl_sp(struct GbCpu *cpu)
{
    const uint16_t value = cpu->registers.sp;
    const uint32_t result_full = cpu->registers.hl + value;
    const uint16_t result = (uint16_t) result_full;

    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, ((cpu->registers.hl & 0x0fff) + (value & 0x0fff)) > 0x0fff);
    gb_cpu_set_flag(cpu, GB_FLAG_C, (result_full & 0x10000) != 0);

    cpu->registers.hl = result;
    return 2;
}

uint8_t cpu_add_sp_i8(struct GbCpu *cpu)
{
    const int8_t value = gb_cpu_fetch_i8(cpu);
    const int32_t result = cpu->registers.sp + value;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, false);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, ((cpu->registers.sp ^ value ^ (result & 0xffff)) & 0x0010) == 0x0010);
    gb_cpu_set_flag(cpu, GB_FLAG_C, ((cpu->registers.sp ^ value ^ (result & 0xffff)) & 0x0100) == 0x0100);

    cpu->registers.sp = (uint16_t) result;
    return 4;
}

uint8_t cpu_dec_sp(struct GbCpu *cpu)
{
    cpu->registers.sp -= 1;
    return 2;
}

uint8_t cpu_inc_sp(struct GbCpu *cpu)
{
    cpu->registers.sp += 1;
    return 2;
}

uint8_t cpu_ld_sp_n16(struct GbCpu *cpu)
{
    const uint16_t value = gb_cpu_fetch_u16(cpu);
    cpu->registers.sp = value;
    return 3;
}

uint8_t cpu_ld_n16_sp(struct GbCpu *cpu)
{
    const uint16_t address = gb_cpu_fetch_u16(cpu);

    gb_mmu_write(cpu->mmu, address, cpu->registers.sp & 0xff);
    gb_mmu_write(cpu->mmu, address + 1, cpu->registers.sp >> 8);
    return 5;
}

uint8_t cpu_ld_hl_sp_i8(struct GbCpu *cpu)
{
    const int8_t value = gb_cpu_fetch_i8(cpu);
    const int32_t result = cpu->registers.sp + value;

    gb_cpu_set_flag(cpu, GB_FLAG_Z, false);
    gb_cpu_set_flag(cpu, GB_FLAG_N, false);
    gb_cpu_set_flag(cpu, GB_FLAG_H, ((cpu->registers.sp ^ value ^ (result & 0xffff)) & 0x0010) == 0x0010);
    gb_cpu_set_flag(cpu, GB_FLAG_C, ((cpu->registers.sp ^ value ^ (result & 0xffff)) & 0x0100) == 0x0100);

    cpu->registers.hl = (uint16_t) result;
    return 3;
}

uint8_t cpu_ld_sp_hl(struct GbCpu *cpu)
{
    cpu->registers.sp = cpu->registers.hl;
    return 2;
}

uint8_t cpu_pop_af(struct GbCpu *cpu)
{
    cpu->registers.af = gb_cpu_pop_stack(cpu) & 0xfff0;
    return 3;
}

uint8_t cpu_pop_r16(struct GbCpu *cpu, const enum GbRegister16 dst)
{
    gb_cpu_set_register16(cpu, dst, gb_cpu_pop_stack(cpu));
    return 3;
}

uint8_t cpu_push_af(struct GbCpu *cpu)
{
    gb_cpu_push_stack(cpu, cpu->registers.af & 0xfff0);
    return 4;
}

uint8_t cpu_push_r16(struct GbCpu *cpu, const enum GbRegister16 src)
{
    gb_cpu_push_stack(cpu, gb_cpu_get_register16(cpu, src));
    return 4;
}

// Interrupt-related instructions

uint8_t cpu_di(struct GbCpu *cpu)
{
    cpu->interrupt_master_enable = false;
    return 1;
}

uint8_t cpu_ei(struct GbCpu *cpu)
{
    cpu->ime_delay = 2;
    return 1;
}

uint8_t cpu_halt(struct GbCpu *cpu)
{
    (void) cpu;
    // FIXME: Implement me
    return 0;
}

// Miscellaneous instructions

/// DAA
uint8_t cpu_daa(struct GbCpu *cpu)
{
    uint8_t value = cpu->registers.a;
    uint16_t correction = gb_cpu_is_flag(cpu, GB_FLAG_C) ? 0x60 : 0x00;
    if (gb_cpu_is_flag(cpu, GB_FLAG_H) || (!gb_cpu_is_flag(cpu, GB_FLAG_N) && ((value & 0x0f) > 9)))
    {
        correction |= 0x06;
    }

    if (gb_cpu_is_flag(cpu, GB_FLAG_C) || (!gb_cpu_is_flag(cpu, GB_FLAG_N) && (value > 0x99)))
    {
        correction |= 0x60;
    }

    if (gb_cpu_is_flag(cpu, GB_FLAG_N))
    {
        value = (uint8_t) (value - correction);
    }
    else
    {
        value = (uint8_t) (value + correction);
    }

    gb_cpu_set_flag(cpu, GB_FLAG_Z, value == 0);
    gb_cpu_set_flag(cpu, GB_FLAG_H, false);

    if (((correction << 2) & 0x0100) != 0)
    {
        gb_cpu_set_flag(cpu, GB_FLAG_C, true);
    }

    cpu->registers.a = value;
    return 1;
}

/// NOP
uint8_t cpu_nop(struct GbCpu *cpu)
{
    (void) cpu;
    return 1;
}

/// STOP
uint8_t cpu_stop(struct GbCpu *cpu)
{
    (void) cpu;
    // FIXME: Implement me
    return 0;
}
