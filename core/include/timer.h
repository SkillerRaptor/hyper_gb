/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct Gameboy;

struct Timer
{
    struct Gameboy *gb;

    u16 div_counter;
    u16 counter;

    // Registers
    u8 div; // 0xff04 - Divider register
    u8 tima; // 0xff05 - Timer counter
    u8 tma; // 0xff06 - Timer modulo
    u8 tac; // 0xff07 - Timer control
};

struct Timer *timer_create(struct Gameboy *);
void timer_destroy(struct Timer *);

void timer_tick(struct Timer *, u8 t_cycles);

#ifdef __cplusplus
}
#endif
