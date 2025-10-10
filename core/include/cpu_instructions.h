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

#ifdef __cplusplus
}
#endif
