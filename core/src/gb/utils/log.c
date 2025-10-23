/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/utils/log.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

static const char *s_level_colors[] = {
    [GB_LOG_ERROR] = "\033[38;2;254;17;85m",
    [GB_LOG_WARN] = "\033[38;2;255;221;85m",
    [GB_LOG_INFO] = "\033[38;2;0;128;0m",
    [GB_LOG_DEBUG] = "\033[38;2;0;0;255m",
};

static const char *s_level_names[] = {
    [GB_LOG_ERROR] = "error",
    [GB_LOG_WARN] = "warning",
    [GB_LOG_INFO] = "info",
    [GB_LOG_DEBUG] = "debug",
};

static void vlog(const enum GbLogLevel level, const char *fmt, const va_list args)
{
    time_t current_time;
    time(&current_time);

    const struct tm *local_time = localtime(&current_time);
    const uint16_t year = (uint16_t) local_time->tm_year + 1900;
    const uint8_t month = (uint8_t) local_time->tm_mon;
    const uint8_t day = (uint8_t) local_time->tm_mday;
    const uint8_t hour = (uint8_t) local_time->tm_hour;
    const uint8_t minute = (uint8_t) local_time->tm_min;
    const uint8_t second = (uint8_t) local_time->tm_sec;

    const char *level_color = s_level_colors[level];
    const char *level_name = s_level_names[level];

    printf(
        "\033[38;2;69;69;69m%04d-%02d-%02dT%02d:%02d:%02d %s%s \033[38;2;211;211;211m",
        year,
        month,
        day,
        hour,
        minute,
        second,
        level_color,
        level_name);
    vprintf(fmt, args);
}

void gb_log(const enum GbLogLevel level, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vlog(level, fmt, args);
    va_end(args);
}
