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
