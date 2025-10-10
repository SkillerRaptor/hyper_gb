/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

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

struct Cpu
{
    struct Mmu *mmu;

    struct Registers registers;
};

struct Cpu *cpu_create(struct Mmu *);
void cpu_destroy(struct Cpu *);

void cpu_tick(struct Cpu *);
