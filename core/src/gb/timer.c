/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/timer.h"

#include <stdbool.h>
#include <stdlib.h>

#include "gb/cpu.h"
#include "gb/definitions.h"
#include "gb/utils/bits.h"

// This defines how many M-Cycles correspond to T-Cycles
#define GB_DIVIDER_HZ 16384.0
#define GB_DIVIDER_CYCLES (GB_MASTER_CLOCK_HZ / GB_DIVIDER_HZ)

struct GbTimer *gb_timer_create(void)
{
    struct GbTimer *timer = malloc(sizeof(struct GbTimer));
    timer->cpu = NULL;
    timer->div_counter = 0;
    timer->counter = 0;
    timer->div = 0;
    timer->tima = 0;
    timer->tma = 0;
    timer->tac = 0;

    return timer;
}

void gb_timer_destroy(struct GbTimer *timer) { free(timer); }

static bool is_enabled(struct GbTimer *timer) { return GB_BIT_CHECK(timer->tac, 2); }

static uint32_t get_frequency(struct GbTimer *timer)
{
    const uint8_t clock = timer->tac & 0b11;
    switch (clock)
    {
    case 0b00: return 4096;
    case 0b01: return 262144;
    case 0b10: return 65536;
    case 0b11: return 16384;
    default: return 0;
    }
}

void gb_timer_tick(struct GbTimer *timer, const uint8_t t_cycles)
{
    // DIV is incremented at a rate of 16384Hz which is equal to 256 t-cycles or 64 m-cycles
    timer->div_counter += t_cycles;
    if (timer->div_counter >= (uint16_t) GB_DIVIDER_CYCLES)
    {
        timer->div += 1;
        timer->div_counter -= (uint16_t) GB_DIVIDER_CYCLES;
    }

    if (!is_enabled(timer))
    {
        return;
    }

    timer->counter += t_cycles;

    const uint16_t frequency = (uint16_t) (GB_MASTER_CLOCK_HZ / get_frequency(timer));
    while (timer->counter >= frequency)
    {
        timer->tima += 1;
        timer->counter -= frequency;

        // If TIMA overflows
        if (timer->tima == 0)
        {
            timer->tima = timer->tma;
            gb_cpu_request_interrupt(timer->cpu, GB_INTERRUPT_TIMER);
        }
    }
}
