/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

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

