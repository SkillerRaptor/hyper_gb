/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct Cpu;

struct Timer
{
    struct Cpu *cpu;

    uint16_t div_counter;
    uint16_t counter;

    // Registers
    uint8_t div; // 0xff04 - Divider register
    uint8_t tima; // 0xff05 - Timer counter
    uint8_t tma; // 0xff06 - Timer modulo
    uint8_t tac; // 0xff07 - Timer control
};

struct Timer *timer_create();
void timer_destroy(struct Timer *);

void timer_tick(struct Timer *, uint8_t t_cycles);

#ifdef __cplusplus
}
#endif
