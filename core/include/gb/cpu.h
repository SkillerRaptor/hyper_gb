/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "gb/prerequisites.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct Gameboy;

struct Registers
{
    union
    {
        struct
        {
#if LITTLE_ENDIAN
            uint8_t f;
            uint8_t a;
#else
            uint8_t a;
            uint8_t f;
#endif
        };
        uint16_t af;
    };

    union
    {
        struct
        {
#if LITTLE_ENDIAN
            uint8_t c;
            uint8_t b;
#else
            uint8_t b;
            uint8_t c;
#endif
        };
        uint16_t bc;
    };

    union
    {
        struct
        {
#if LITTLE_ENDIAN
            uint8_t e;
            uint8_t d;
#else
            uint8_t d;
            uint8_t e;
#endif
        };
        uint16_t de;
    };

    union
    {
        struct
        {
#if LITTLE_ENDIAN
            uint8_t l;
            uint8_t h;
#else
            uint8_t h;
            uint8_t l;
#endif
        };
        uint16_t hl;
    };

    uint16_t sp;
    uint16_t pc;
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

enum ConditionCode
{
    CC_Z,
    CC_NZ,
    CC_C,
    CC_NC,
};

enum Rst
{
    RST_00 = 0x00,
    RST_08 = 0x08,
    RST_10 = 0x10,
    RST_18 = 0x18,
    RST_20 = 0x20,
    RST_28 = 0x28,
    RST_30 = 0x30,
    RST_38 = 0x38,
};

struct Cpu
{
    struct Gameboy *gb;

    struct Registers registers;

    bool interrupt_master_enable; // IME flag
    uint8_t ime_delay;

    uint8_t interrupt_enable; // IE
    uint8_t interrupt_flag; // IF
};

struct Cpu *cpu_create(struct Gameboy *);
void cpu_destroy(struct Cpu *);

void cpu_set_register8(struct Cpu *, enum Register8, uint8_t);
uint8_t cpu_get_register8(struct Cpu *, enum Register8);

void cpu_set_register16(struct Cpu *, enum Register16, uint16_t);
uint16_t cpu_get_register16(struct Cpu *, enum Register16);

void cpu_set_flag(struct Cpu *, enum Flag, bool);
bool cpu_is_flag(struct Cpu *, enum Flag);

bool cpu_is_condition(struct Cpu *, enum ConditionCode);

void cpu_push_stack(struct Cpu *, uint16_t);
uint16_t cpu_pop_stack(struct Cpu *);

int8_t cpu_fetch_i8(struct Cpu *cpu);
uint8_t cpu_fetch_u8(struct Cpu *cpu);
uint16_t cpu_fetch_u16(struct Cpu *cpu);

uint8_t cpu_tick(struct Cpu *);

#ifdef __cplusplus
}
#endif
