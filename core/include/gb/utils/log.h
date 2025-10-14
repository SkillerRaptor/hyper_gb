/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

enum GbLogLevel
{
    GB_LOG_ERROR = 0,
    GB_LOG_WARN,
    GB_LOG_INFO,
    GB_LOG_DEBUG,
};

void gb_log(enum GbLogLevel, const char *fmt, ...);

#ifdef __cplusplus
}
#endif
