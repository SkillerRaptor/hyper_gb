/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "gb/cpu.h"

#ifdef __cplusplus
extern "C"
{
#endif

// Load instructions
uint8_t cpu_ld_r8_r8(struct GbCpu *, enum GbRegister8 dst, enum GbRegister8 src);
uint8_t cpu_ld_r8_n8(struct GbCpu *, enum GbRegister8 dst);
uint8_t cpu_ld_r16_n16(struct GbCpu *, enum GbRegister16 dst);
uint8_t cpu_ld_hl_r8(struct GbCpu *, enum GbRegister8 src);
uint8_t cpu_ld_hl_n8(struct GbCpu *);
uint8_t cpu_ld_r8_hl(struct GbCpu *, enum GbRegister8 dst);
uint8_t cpu_ld_r16_a(struct GbCpu *, enum GbRegister16 dst);
uint8_t cpu_ld_n16_a(struct GbCpu *);
uint8_t cpu_ldh_n8_a(struct GbCpu *);
uint8_t cpu_ldh_c_a(struct GbCpu *);
uint8_t cpu_ld_a_r16(struct GbCpu *, enum GbRegister16 src);
uint8_t cpu_ld_a_n16(struct GbCpu *);
uint8_t cpu_ldh_a_n8(struct GbCpu *);
uint8_t cpu_ldh_a_c(struct GbCpu *);
uint8_t cpu_ld_hli_a(struct GbCpu *);
uint8_t cpu_ld_hld_a(struct GbCpu *);
uint8_t cpu_ld_a_hli(struct GbCpu *);
uint8_t cpu_ld_a_hld(struct GbCpu *);

// 8-bit arithmetic instructions
uint8_t cpu_adc_a_r8(struct GbCpu *, enum GbRegister8 src);
uint8_t cpu_adc_a_hl(struct GbCpu *);
uint8_t cpu_adc_a_n8(struct GbCpu *);
uint8_t cpu_add_a_r8(struct GbCpu *, enum GbRegister8 src);
uint8_t cpu_add_a_hl(struct GbCpu *);
uint8_t cpu_add_a_n8(struct GbCpu *);
uint8_t cpu_cp_a_r8(struct GbCpu *, enum GbRegister8 src);
uint8_t cpu_cp_a_hl(struct GbCpu *);
uint8_t cpu_cp_a_n8(struct GbCpu *);
uint8_t cpu_dec_r8(struct GbCpu *, enum GbRegister8 dst);
uint8_t cpu_dec_hl(struct GbCpu *);
uint8_t cpu_inc_r8(struct GbCpu *, enum GbRegister8 dst);
uint8_t cpu_inc_hl(struct GbCpu *);
uint8_t cpu_sbc_a_r8(struct GbCpu *, enum GbRegister8 src);
uint8_t cpu_sbc_a_hl(struct GbCpu *);
uint8_t cpu_sbc_a_n8(struct GbCpu *);
uint8_t cpu_sub_a_r8(struct GbCpu *, enum GbRegister8 src);
uint8_t cpu_sub_a_hl(struct GbCpu *);
uint8_t cpu_sub_a_n8(struct GbCpu *);

// 16-bit arithmetic instructions
uint8_t cpu_add_hl_r16(struct GbCpu *, enum GbRegister16 src);
uint8_t cpu_dec_r16(struct GbCpu *, enum GbRegister16 dst);
uint8_t cpu_inc_r16(struct GbCpu *, enum GbRegister16 dst);

// Bitwise logic instructions
uint8_t cpu_and_a_r8(struct GbCpu *, enum GbRegister8 src);
uint8_t cpu_and_a_hl(struct GbCpu *);
uint8_t cpu_and_a_n8(struct GbCpu *);
uint8_t cpu_cpl(struct GbCpu *);
uint8_t cpu_or_a_r8(struct GbCpu *, enum GbRegister8 src);
uint8_t cpu_or_a_hl(struct GbCpu *);
uint8_t cpu_or_a_n8(struct GbCpu *);
uint8_t cpu_xor_a_r8(struct GbCpu *, enum GbRegister8 src);
uint8_t cpu_xor_a_hl(struct GbCpu *);
uint8_t cpu_xor_a_n8(struct GbCpu *);

// Bit flag instructions
uint8_t cpu_bit_u3_r8(struct GbCpu *, uint8_t bit, enum GbRegister8 src);
uint8_t cpu_bit_u3_hl(struct GbCpu *, uint8_t bit);
uint8_t cpu_res_u3_r8(struct GbCpu *, uint8_t bit, enum GbRegister8 dst);
uint8_t cpu_res_u3_hl(struct GbCpu *, uint8_t bit);
uint8_t cpu_set_u3_r8(struct GbCpu *, uint8_t bit, enum GbRegister8 dst);
uint8_t cpu_set_u3_hl(struct GbCpu *, uint8_t bit);

// Bit shift instructions
uint8_t cpu_rl_r8(struct GbCpu *, enum GbRegister8 dst);
uint8_t cpu_rl_hl(struct GbCpu *);
uint8_t cpu_rla(struct GbCpu *);
uint8_t cpu_rlc_r8(struct GbCpu *, enum GbRegister8 dst);
uint8_t cpu_rlc_hl(struct GbCpu *);
uint8_t cpu_rlca(struct GbCpu *);
uint8_t cpu_rr_r8(struct GbCpu *, enum GbRegister8 dst);
uint8_t cpu_rr_hl(struct GbCpu *);
uint8_t cpu_rra(struct GbCpu *);
uint8_t cpu_rrc_r8(struct GbCpu *, enum GbRegister8 dst);
uint8_t cpu_rrc_hl(struct GbCpu *);
uint8_t cpu_rrca(struct GbCpu *);
uint8_t cpu_sla_r8(struct GbCpu *, enum GbRegister8 dst);
uint8_t cpu_sla_hl(struct GbCpu *);
uint8_t cpu_sra_r8(struct GbCpu *, enum GbRegister8 dst);
uint8_t cpu_sra_hl(struct GbCpu *);
uint8_t cpu_srl_r8(struct GbCpu *, enum GbRegister8 dst);
uint8_t cpu_srl_hl(struct GbCpu *);
uint8_t cpu_swap_r8(struct GbCpu *, enum GbRegister8 dst);
uint8_t cpu_swap_hl(struct GbCpu *);

// Jumps and subroutine instructions
uint8_t cpu_call_n16(struct GbCpu *);
uint8_t cpu_call_cc_n16(struct GbCpu *, enum GbConditionCode);
uint8_t cpu_jp_hl(struct GbCpu *);
uint8_t cpu_jp_n16(struct GbCpu *);
uint8_t cpu_jp_cc_n16(struct GbCpu *, enum GbConditionCode);
uint8_t cpu_jr_i8(struct GbCpu *);
uint8_t cpu_jr_cc_i8(struct GbCpu *, enum GbConditionCode);
uint8_t cpu_ret_cc(struct GbCpu *, enum GbConditionCode);
uint8_t cpu_ret(struct GbCpu *);
uint8_t cpu_reti(struct GbCpu *);
uint8_t cpu_rst_vec(struct GbCpu *, enum GbRst vec);

// Carry flag instructions
uint8_t cpu_ccf(struct GbCpu *);
uint8_t cpu_scf(struct GbCpu *);

// Stack manipulation instructions
uint8_t cpu_add_hl_sp(struct GbCpu *);
uint8_t cpu_add_sp_i8(struct GbCpu *);
uint8_t cpu_dec_sp(struct GbCpu *);
uint8_t cpu_inc_sp(struct GbCpu *);
uint8_t cpu_ld_sp_n16(struct GbCpu *);
uint8_t cpu_ld_n16_sp(struct GbCpu *);
uint8_t cpu_ld_hl_sp_i8(struct GbCpu *);
uint8_t cpu_ld_sp_hl(struct GbCpu *);
uint8_t cpu_pop_af(struct GbCpu *);
uint8_t cpu_pop_r16(struct GbCpu *, enum GbRegister16 dst);
uint8_t cpu_push_af(struct GbCpu *);
uint8_t cpu_push_r16(struct GbCpu *, enum GbRegister16 src);

// Interrupt-related instructions
uint8_t cpu_di(struct GbCpu *);
uint8_t cpu_ei(struct GbCpu *);
uint8_t cpu_halt(struct GbCpu *);

// Miscellaneous instructions
uint8_t cpu_daa(struct GbCpu *);
uint8_t cpu_nop(struct GbCpu *);
uint8_t cpu_stop(struct GbCpu *);

#ifdef __cplusplus
}
#endif
