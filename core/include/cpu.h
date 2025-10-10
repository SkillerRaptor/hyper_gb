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

#include <stdbool.h>

#include "prerequisites.h"
#include "types.h"

struct Mmu;

struct Registers
{
    union
    {
        struct
        {
#if LITTLE_ENDIAN
            u8 f;
            u8 a;
#else
            u8 a;
            u8 f;
#endif
        };
        u16 af;
    };

    union
    {
        struct
        {
#if LITTLE_ENDIAN
            u8 c;
            u8 b;
#else
            u8 b;
            u8 c;
#endif
        };
        u16 bc;
    };

    union
    {
        struct
        {
#if LITTLE_ENDIAN
            u8 e;
            u8 d;
#else
            u8 d;
            u8 e;
#endif
        };
        u16 de;
    };

    union
    {
        struct
        {
#if LITTLE_ENDIAN
            u8 l;
            u8 h;
#else
            u8 h;
            u8 l;
#endif
        };
        u16 hl;
    };

    u16 sp;
    u16 pc;
};

enum Register8
{
    R8_A,
    R8_B,
    R8_C,
    R8_D,
    R8_E,
    R8_H,
    R8_L,
};

enum Register16
{
    R16_AF,
    R16_BC,
    R16_DE,
    R16_HL,
};

enum Flag
{
    FLAG_Z = 1 << 7,
    FLAG_N = 1 << 6,
    FLAG_H = 1 << 5,
    FLAG_C = 1 << 4,
};

struct Cpu
{
    struct Mmu *mmu;

    struct Registers registers;
    bool ime;
};

struct Cpu *cpu_create(struct Mmu *);
void cpu_destroy(struct Cpu *);

void cpu_set_register8(struct Cpu *, enum Register8, u8);
uint8_t cpu_get_register8(struct Cpu *, enum Register8);

void cpu_set_register16(struct Cpu *, enum Register16, u16);
uint16_t cpu_get_register16(struct Cpu *, enum Register16);

void cpu_set_flag(struct Cpu *, enum Flag, bool);
bool cpu_is_flag(struct Cpu *, enum Flag);

i8 cpu_fetch_i8(struct Cpu *cpu);
u8 cpu_fetch_u8(struct Cpu *cpu);
u16 cpu_fetch_u16(struct Cpu *cpu);

void cpu_tick(struct Cpu *);

#ifdef __cplusplus
}
#endif
