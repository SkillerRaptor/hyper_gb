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

struct GbCpu;

struct GbTimer
{
    struct GbCpu *cpu;

    uint16_t div_counter;
    uint16_t counter;

    // Registers
    uint8_t div; // 0xff04 - Divider register
    uint8_t tima; // 0xff05 - Timer counter
    uint8_t tma; // 0xff06 - Timer modulo
    uint8_t tac; // 0xff07 - Timer control
};

struct GbTimer *gb_timer_create();
void gb_timer_destroy(struct GbTimer *);

void gb_timer_tick(struct GbTimer *, uint8_t t_cycles);

#ifdef __cplusplus
}
#endif
