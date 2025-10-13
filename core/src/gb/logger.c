/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "gb/logger.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

enum LogLevel
{
    LOG_LEVEL_INFO = 0,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_LENGTH,
};

static const char *s_log_level_colors[LOG_LEVEL_LENGTH] = {
    [LOG_LEVEL_INFO] = "\033[38;2;0;128;0m",
    [LOG_LEVEL_WARNING] = "\033[38;2;255;221;85m",
    [LOG_LEVEL_ERROR] = "\033[38;2;254;17;85m",
    [LOG_LEVEL_DEBUG] = "\033[38;2;0;0;255m",
};

static const char *s_log_level_strings[LOG_LEVEL_LENGTH] = {
    [LOG_LEVEL_INFO] = "info",
    [LOG_LEVEL_WARNING] = "warning",
    [LOG_LEVEL_ERROR] = "error",
    [LOG_LEVEL_DEBUG] = "debug",
};

static void logger_vlog(const enum LogLevel level, const char *fmt, va_list args)
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

    const char *level_color = s_log_level_colors[level];
    const char *level_string = s_log_level_strings[level];

    printf("\033[38;2;69;69;69m%04d-%02d-%02dT%02d:%02d:%02d %s%s \033[38;2;211;211;211m", year, month, day, hour, minute,
        second, level_color, level_string);
    vprintf(fmt, args);
    printf("\n");
}

void logger_info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logger_vlog(LOG_LEVEL_INFO, fmt, args);
    va_end(args);
}

void logger_warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logger_vlog(LOG_LEVEL_WARNING, fmt, args);
    va_end(args);
}

void logger_err(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logger_vlog(LOG_LEVEL_ERROR, fmt, args);
    va_end(args);
}

void logger_dbg(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    logger_vlog(LOG_LEVEL_DEBUG, fmt, args);
    va_end(args);
}
