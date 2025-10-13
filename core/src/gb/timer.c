/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/timer.h"

#include <stdbool.h>
#include <stdlib.h>

#include "gb/gameboy.h"
#include "gb/logger.h"

// This defines how many M-Cycles correspond to T-Cycles
#define GAMEBOY_DIVIDER_HZ 16384.0
#define GAMEBOY_DIVIDER_CYCLES (GAMEBOY_MASTER_CLOCK_HZ / GAMEBOY_DIVIDER_HZ)

static bool timer_is_enabled(struct Timer *);
static uint32_t timer_get_frequency(struct Timer *);

struct Timer *timer_create(struct Gameboy *gb)
{
    struct Timer *timer = malloc(sizeof(struct Gameboy));
    timer->gb = gb;
    timer->div_counter = 0;
    timer->counter = 0;
    timer->div = 0;
    timer->tima = 0;
    timer->tma = 0;
    timer->tac = 0;

    return timer;
}

void timer_destroy(struct Timer *timer) { free(timer); }

void timer_tick(struct Timer *timer, const uint8_t t_cycles)
{
    // DIV is incremented at a rate of 16384Hz which is equal to 256 t-cycles or 64 m-cycles
    timer->div_counter += t_cycles;
    if (timer->div_counter >= (uint16_t) GAMEBOY_DIVIDER_CYCLES)
    {
        timer->div += 1;
        timer->div_counter -= (uint16_t) GAMEBOY_DIVIDER_CYCLES;
    }

    if (!timer_is_enabled(timer))
    {
        return;
    }

    timer->counter += t_cycles;

    const uint16_t frequency = (uint16_t) (GAMEBOY_MASTER_CLOCK_HZ / timer_get_frequency(timer));
    while (timer->counter >= frequency)
    {
        timer->tima += 1;
        timer->counter -= frequency;

        // If TIMA overflows
        if (timer->tima == 0)
        {
            timer->tima = timer->tma;
            logger_warn("Unimplemented interrupt trigger");
        }
    }
}

static bool timer_is_enabled(struct Timer *timer) { return ((timer->tac >> 2) & 0b1) != 0; }

static uint32_t timer_get_frequency(struct Timer *timer)
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
