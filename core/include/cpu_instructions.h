/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "cpu.h"

// Load instructions

/// LD r8,r8
void cpu_ld_r8_r8(struct Cpu *, enum Register8 dst, enum Register8 src);

/// LD r8,n8
void cpu_ld_r8_n8(struct Cpu *, enum Register8 dst);

/// LD r16,n16
void cpu_ld_r16_n16(struct Cpu *, enum Register16 dst);

/// LD [HL],r8
void cpu_ld_hl_r8(struct Cpu *, enum Register8 src);

/// LD [HL],n8
void cpu_ld_hl_n8(struct Cpu *);

/// LD r8,[HL]
void cpu_ld_r8_hl(struct Cpu *, enum Register8 dst);

/// LD [r16],A
void cpu_ld_r16_a(struct Cpu *, enum Register16 dst);

/// LD [n16],A
void cpu_ld_n16_a(struct Cpu *);

/// LDH [n8],A
void cpu_ldh_n8_a(struct Cpu *);

/// LDH [C],A
void cpu_ldh_c_a(struct Cpu *);

/// LD A,[r16]
void cpu_ld_a_r16(struct Cpu *, enum Register16 src);

/// LD A,[n16]
void cpu_ld_a_n16(struct Cpu *);

/// LDH A,[n8]
void cpu_ldh_a_n8(struct Cpu *);

/// LDH A,[C]
void cpu_ldh_a_c(struct Cpu *);

/// LD [HLI],A
void cpu_ld_hli_a(struct Cpu *);

/// LD [HLD],A
void cpu_ld_hld_a(struct Cpu *);

/// LD A,[HLI]
void cpu_ld_a_hli(struct Cpu *);

/// LD A,[HLD]
void cpu_ld_a_hld(struct Cpu *);

// 8-bit arithmetic instructions

/// ADC A,r8
void cpu_adc_a_r8(struct Cpu *, enum Register8 src);

/// ADC A,[HL]
void cpu_adc_a_hl(struct Cpu *);

/// ADC A,n8
void cpu_adc_a_n8(struct Cpu *);

/// ADD A,r8
void cpu_add_a_r8(struct Cpu *, enum Register8 src);

/// ADD A,[HL]
void cpu_add_a_hl(struct Cpu *);

/// ADD A,n8
void cpu_add_a_n8(struct Cpu *);

/// CP A,r8
void cpu_cp_a_r8(struct Cpu *, enum Register8 src);

/// CP A,[HL]
void cpu_cp_a_hl(struct Cpu *);

/// CP A,n8
void cpu_cp_a_n8(struct Cpu *);

/// DEC r8
void cpu_dec_r8(struct Cpu *, enum Register8 dst);

/// DEC [HL]
void cpu_dec_hl(struct Cpu *);

/// INC r8
void cpu_inc_r8(struct Cpu *, enum Register8 dst);

/// INC [HL]
void cpu_inc_hl(struct Cpu *);

/// SBC A,r8
void cpu_sbc_a_r8(struct Cpu *, enum Register8 src);

/// SBC A,[HL]
void cpu_sbc_a_hl(struct Cpu *);

/// SBC A,n8
void cpu_sbc_a_n8(struct Cpu *);

/// SUB A,r8
void cpu_sub_a_r8(struct Cpu *, enum Register8 src);

/// SUB A,[HL]
void cpu_sub_a_hl(struct Cpu *);

/// SUB A,n8
void cpu_sub_a_n8(struct Cpu *);

// 16-bit arithmetic instructions

/// ADD HL,r16
void cpu_add_hl_r16(struct Cpu *, enum Register16 src);

/// DEC r16
void cpu_dec_r16(struct Cpu *, enum Register16 dst);

/// INC r16
void cpu_inc_r16(struct Cpu *, enum Register16 dst);

// Bitwise logic instructions

/// AND A,r8
void cpu_and_a_r8(struct Cpu *, enum Register8 src);

/// AND A,[HL]
void cpu_and_a_hl(struct Cpu *);

/// AND A,n8
void cpu_and_a_n8(struct Cpu *);

/// CPL
void cpu_cpl(struct Cpu *);

/// OR A,r8
void cpu_or_a_r8(struct Cpu *, enum Register8 src);

/// OR A,[HL]
void cpu_or_a_hl(struct Cpu *);

/// OR A,n8
void cpu_or_a_n8(struct Cpu *);

/// XOR A,r8
void cpu_xor_a_r8(struct Cpu *, enum Register8 src);

/// XOR A,[HL]
void cpu_xor_a_hl(struct Cpu *);

/// XOR A,n8
void cpu_xor_a_n8(struct Cpu *);

// Bit flag instructions

/// BIT u3,r8
void cpu_bit_u3_r8(struct Cpu *, u8 bit, enum Register8 src);

/// BIT u3,[HL]
void cpu_bit_u3_hl(struct Cpu *, u8 bit);

/// RES u3,r8
void cpu_res_u3_r8(struct Cpu *, u8 bit, enum Register8 dst);

/// RES u3,[HL]
void cpu_res_u3_hl(struct Cpu *, u8 bit);

/// SET u3,r8
void cpu_set_u3_r8(struct Cpu *, u8 bit, enum Register8 dst);

/// SET u3,[HL]
void cpu_set_u3_hl(struct Cpu *, u8 bit);

// Bit shift instructions

/// RL r8
void cpu_rl_r8(struct Cpu *, enum Register8 dst);

/// RL [HL]
void cpu_rl_hl(struct Cpu *);

/// RLA
void cpu_rla(struct Cpu *);

/// RLC r8
void cpu_rlc_r8(struct Cpu *, enum Register8 dst);

/// RLC [HL]
void cpu_rlc_hl(struct Cpu *);

/// RLCA
void cpu_rlca(struct Cpu *);

/// RR r8
void cpu_rr_r8(struct Cpu *, enum Register8 dst);

/// RR [HL]
void cpu_rr_hl(struct Cpu *);

/// RRA
void cpu_rra(struct Cpu *);

/// RRC r8
void cpu_rrc_r8(struct Cpu *, enum Register8 dst);

/// RRC [HL]
void cpu_rrc_hl(struct Cpu *);

/// RRCA
void cpu_rrca(struct Cpu *);

/// SLA r8
void cpu_sla_r8(struct Cpu *, enum Register8 dst);

/// SLA [HL]
void cpu_sla_hl(struct Cpu *);

/// SRA r8
void cpu_sra_r8(struct Cpu *, enum Register8 dst);

/// SRA [HL]
void cpu_sra_hl(struct Cpu *);

/// SRL r8
void cpu_srl_r8(struct Cpu *, enum Register8 dst);

/// SRL [HL]
void cpu_srl_hl(struct Cpu *);

/// SWAP r8
void cpu_swap_r8(struct Cpu *, enum Register8 dst);

/// SWAP [HL]
void cpu_swap_hl(struct Cpu *);

// Jumps and subroutine instructions

/// CALL n16
void cpu_call_n16(struct Cpu *);

/// CALL cc,n16
void cpu_call_cc_n16(struct Cpu *, enum ConditionCode);

/// JP HL
void cpu_jp_hl(struct Cpu *);

/// JP n16
void cpu_jp_n16(struct Cpu *);

/// JP cc,n16
void cpu_jp_cc_n16(struct Cpu *, enum ConditionCode);

/// JR i8
void cpu_jr_i8(struct Cpu *);

/// JR cc,i8
void cpu_jr_cc_i8(struct Cpu *, enum ConditionCode);

/// RET cc
void cpu_ret_cc(struct Cpu *, enum ConditionCode);

/// RET
void cpu_ret(struct Cpu *);

/// RETI
void cpu_reti(struct Cpu *);

/// RST vec
void cpu_rst_vec(struct Cpu *, enum Rst vec);

// Carry flag instructions

/// CCF
void cpu_ccf(struct Cpu *);

/// SCF
void cpu_scf(struct Cpu *);

// Stack manipulation instructions

/// ADD HL,SP
void cpu_add_hl_sp(struct Cpu *);

/// ADD SP,i8
void cpu_add_sp_i8(struct Cpu *);

/// DEC SP
void cpu_dec_sp(struct Cpu *);

/// INC SP
void cpu_inc_sp(struct Cpu *);

/// LD SP,n16
void cpu_ld_sp_n16(struct Cpu *);

/// LD [n16],SP
void cpu_ld_n16_sp(struct Cpu *);

/// LD HL,SP+i8
void cpu_ld_hl_sp_i8(struct Cpu *);

/// LD SP,HL
void cpu_ld_sp_hl(struct Cpu *);

/// POP AF
void cpu_pop_af(struct Cpu *);

/// POP r16
void cpu_pop_r16(struct Cpu *, enum Register16 dst);

/// PUSH AF
void cpu_push_af(struct Cpu *);

/// PUSH r16
void cpu_push_r16(struct Cpu *, enum Register16 src);

// Interrupt-related instructions

/// DI
void cpu_di(struct Cpu *);

/// EI
void cpu_ei(struct Cpu *);

/// HALT
void cpu_halt(struct Cpu *);

// Miscellaneous instructions

/// DAA
void cpu_daa(struct Cpu *);

/// NOP
void cpu_nop(struct Cpu *);

/// STOP
void cpu_stop(struct Cpu *);

#ifdef __cplusplus
}
#endif
