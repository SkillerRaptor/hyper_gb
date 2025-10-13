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
u8 cpu_ld_r8_r8(struct Cpu *, enum Register8 dst, enum Register8 src);
u8 cpu_ld_r8_n8(struct Cpu *, enum Register8 dst);
u8 cpu_ld_r16_n16(struct Cpu *, enum Register16 dst);
u8 cpu_ld_hl_r8(struct Cpu *, enum Register8 src);
u8 cpu_ld_hl_n8(struct Cpu *);
u8 cpu_ld_r8_hl(struct Cpu *, enum Register8 dst);
u8 cpu_ld_r16_a(struct Cpu *, enum Register16 dst);
u8 cpu_ld_n16_a(struct Cpu *);
u8 cpu_ldh_n8_a(struct Cpu *);
u8 cpu_ldh_c_a(struct Cpu *);
u8 cpu_ld_a_r16(struct Cpu *, enum Register16 src);
u8 cpu_ld_a_n16(struct Cpu *);
u8 cpu_ldh_a_n8(struct Cpu *);
u8 cpu_ldh_a_c(struct Cpu *);
u8 cpu_ld_hli_a(struct Cpu *);
u8 cpu_ld_hld_a(struct Cpu *);
u8 cpu_ld_a_hli(struct Cpu *);
u8 cpu_ld_a_hld(struct Cpu *);

// 8-bit arithmetic instructions
u8 cpu_adc_a_r8(struct Cpu *, enum Register8 src);
u8 cpu_adc_a_hl(struct Cpu *);
u8 cpu_adc_a_n8(struct Cpu *);
u8 cpu_add_a_r8(struct Cpu *, enum Register8 src);
u8 cpu_add_a_hl(struct Cpu *);
u8 cpu_add_a_n8(struct Cpu *);
u8 cpu_cp_a_r8(struct Cpu *, enum Register8 src);
u8 cpu_cp_a_hl(struct Cpu *);
u8 cpu_cp_a_n8(struct Cpu *);
u8 cpu_dec_r8(struct Cpu *, enum Register8 dst);
u8 cpu_dec_hl(struct Cpu *);
u8 cpu_inc_r8(struct Cpu *, enum Register8 dst);
u8 cpu_inc_hl(struct Cpu *);
u8 cpu_sbc_a_r8(struct Cpu *, enum Register8 src);
u8 cpu_sbc_a_hl(struct Cpu *);
u8 cpu_sbc_a_n8(struct Cpu *);
u8 cpu_sub_a_r8(struct Cpu *, enum Register8 src);
u8 cpu_sub_a_hl(struct Cpu *);
u8 cpu_sub_a_n8(struct Cpu *);

// 16-bit arithmetic instructions
u8 cpu_add_hl_r16(struct Cpu *, enum Register16 src);
u8 cpu_dec_r16(struct Cpu *, enum Register16 dst);
u8 cpu_inc_r16(struct Cpu *, enum Register16 dst);

// Bitwise logic instructions
u8 cpu_and_a_r8(struct Cpu *, enum Register8 src);
u8 cpu_and_a_hl(struct Cpu *);
u8 cpu_and_a_n8(struct Cpu *);
u8 cpu_cpl(struct Cpu *);
u8 cpu_or_a_r8(struct Cpu *, enum Register8 src);
u8 cpu_or_a_hl(struct Cpu *);
u8 cpu_or_a_n8(struct Cpu *);
u8 cpu_xor_a_r8(struct Cpu *, enum Register8 src);
u8 cpu_xor_a_hl(struct Cpu *);
u8 cpu_xor_a_n8(struct Cpu *);

// Bit flag instructions
u8 cpu_bit_u3_r8(struct Cpu *, u8 bit, enum Register8 src);
u8 cpu_bit_u3_hl(struct Cpu *, u8 bit);
u8 cpu_res_u3_r8(struct Cpu *, u8 bit, enum Register8 dst);
u8 cpu_res_u3_hl(struct Cpu *, u8 bit);
u8 cpu_set_u3_r8(struct Cpu *, u8 bit, enum Register8 dst);
u8 cpu_set_u3_hl(struct Cpu *, u8 bit);

// Bit shift instructions
u8 cpu_rl_r8(struct Cpu *, enum Register8 dst);
u8 cpu_rl_hl(struct Cpu *);
u8 cpu_rla(struct Cpu *);
u8 cpu_rlc_r8(struct Cpu *, enum Register8 dst);
u8 cpu_rlc_hl(struct Cpu *);
u8 cpu_rlca(struct Cpu *);
u8 cpu_rr_r8(struct Cpu *, enum Register8 dst);
u8 cpu_rr_hl(struct Cpu *);
u8 cpu_rra(struct Cpu *);
u8 cpu_rrc_r8(struct Cpu *, enum Register8 dst);
u8 cpu_rrc_hl(struct Cpu *);
u8 cpu_rrca(struct Cpu *);
u8 cpu_sla_r8(struct Cpu *, enum Register8 dst);
u8 cpu_sla_hl(struct Cpu *);
u8 cpu_sra_r8(struct Cpu *, enum Register8 dst);
u8 cpu_sra_hl(struct Cpu *);
u8 cpu_srl_r8(struct Cpu *, enum Register8 dst);
u8 cpu_srl_hl(struct Cpu *);
u8 cpu_swap_r8(struct Cpu *, enum Register8 dst);
u8 cpu_swap_hl(struct Cpu *);

// Jumps and subroutine instructions
u8 cpu_call_n16(struct Cpu *);
u8 cpu_call_cc_n16(struct Cpu *, enum ConditionCode);
u8 cpu_jp_hl(struct Cpu *);
u8 cpu_jp_n16(struct Cpu *);
u8 cpu_jp_cc_n16(struct Cpu *, enum ConditionCode);
u8 cpu_jr_i8(struct Cpu *);
u8 cpu_jr_cc_i8(struct Cpu *, enum ConditionCode);
u8 cpu_ret_cc(struct Cpu *, enum ConditionCode);
u8 cpu_ret(struct Cpu *);
u8 cpu_reti(struct Cpu *);
u8 cpu_rst_vec(struct Cpu *, enum Rst vec);

// Carry flag instructions
u8 cpu_ccf(struct Cpu *);
u8 cpu_scf(struct Cpu *);

// Stack manipulation instructions
u8 cpu_add_hl_sp(struct Cpu *);
u8 cpu_add_sp_i8(struct Cpu *);
u8 cpu_dec_sp(struct Cpu *);
u8 cpu_inc_sp(struct Cpu *);
u8 cpu_ld_sp_n16(struct Cpu *);
u8 cpu_ld_n16_sp(struct Cpu *);
u8 cpu_ld_hl_sp_i8(struct Cpu *);
u8 cpu_ld_sp_hl(struct Cpu *);
u8 cpu_pop_af(struct Cpu *);
u8 cpu_pop_r16(struct Cpu *, enum Register16 dst);
u8 cpu_push_af(struct Cpu *);
u8 cpu_push_r16(struct Cpu *, enum Register16 src);

// Interrupt-related instructions
u8 cpu_di(struct Cpu *);
u8 cpu_ei(struct Cpu *);
u8 cpu_halt(struct Cpu *);

// Miscellaneous instructions
u8 cpu_daa(struct Cpu *);
u8 cpu_nop(struct Cpu *);
u8 cpu_stop(struct Cpu *);

#ifdef __cplusplus
}
#endif
