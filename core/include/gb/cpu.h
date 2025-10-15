/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#    define GB_DEFINE_REGISTER(a, b) \
        union                        \
        {                            \
            struct                   \
            {                        \
                uint8_t b;           \
                uint8_t a;           \
            };                       \
            uint16_t a##b;           \
        }
#else
#    define GB_DEFINE_REGISTER(a, b) \
        union                        \
        {                            \
            struct                   \
            {                        \
                uint8_t a;           \
                uint8_t b;           \
            };                       \
            uint16_t a##b;           \
        }
#endif

struct GbMmu;

struct GbRegisters
{
    GB_DEFINE_REGISTER(a, f);
    GB_DEFINE_REGISTER(b, c);
    GB_DEFINE_REGISTER(d, e);
    GB_DEFINE_REGISTER(h, l);
    uint16_t sp;
    uint16_t pc;
};

enum GbRegister8
{
    GB_R8_A,
    GB_R8_B,
    GB_R8_C,
    GB_R8_D,
    GB_R8_E,
    GB_R8_H,
    GB_R8_L,
};

enum GbRegister16
{
    GB_R16_AF,
    GB_R16_BC,
    GB_R16_DE,
    GB_R16_HL,
};

enum GbFlag
{
    GB_FLAG_Z = 1 << 7,
    GB_FLAG_N = 1 << 6,
    GB_FLAG_H = 1 << 5,
    GB_FLAG_C = 1 << 4,
};

enum GbConditionCode
{
    GB_CC_Z,
    GB_CC_NZ,
    GB_CC_C,
    GB_CC_NC,
};

enum GbRst
{
    GB_RST_00 = 0x00,
    GB_RST_08 = 0x08,
    GB_RST_10 = 0x10,
    GB_RST_18 = 0x18,
    GB_RST_20 = 0x20,
    GB_RST_28 = 0x28,
    GB_RST_30 = 0x30,
    GB_RST_38 = 0x38,
};

enum GbInterrupt
{
    GB_INTERRUPT_VBLANK,
    GB_INTERRUPT_LCD,
    GB_INTERRUPT_TIMER,
    GB_INTERRUPT_SERIAL,
    GB_INTERRUPT_JOYPAD,
};

struct GbCpu
{
    struct GbMmu *mmu;

    struct GbRegisters registers;

    bool interrupt_master_enable; // IME flag
    uint8_t ime_delay;

    uint8_t interrupt_enable; // IE
    uint8_t interrupt_flag; // IF
};

struct GbCpu *gb_cpu_create(void);
void gb_cpu_destroy(struct GbCpu *);

void gb_cpu_set_register8(struct GbCpu *, enum GbRegister8, uint8_t);
uint8_t gb_cpu_get_register8(struct GbCpu *, enum GbRegister8);

void gb_cpu_set_register16(struct GbCpu *, enum GbRegister16, uint16_t);
uint16_t gb_cpu_get_register16(struct GbCpu *, enum GbRegister16);

void gb_cpu_set_flag(struct GbCpu *, enum GbFlag, bool);
bool gb_cpu_is_flag(struct GbCpu *, enum GbFlag);

bool gb_cpu_is_condition(struct GbCpu *, enum GbConditionCode);

void gb_cpu_push_stack(struct GbCpu *, uint16_t);
uint16_t gb_cpu_pop_stack(struct GbCpu *);

int8_t gb_cpu_fetch_i8(struct GbCpu *);
uint8_t gb_cpu_fetch_u8(struct GbCpu *);
uint16_t gb_cpu_fetch_u16(struct GbCpu *);

void gb_cpu_request_interrupt(struct GbCpu *, enum GbInterrupt);

uint8_t gb_cpu_tick(struct GbCpu *);

#undef GB_DEFINE_REGISTER

#ifdef __cplusplus
}
#endif
