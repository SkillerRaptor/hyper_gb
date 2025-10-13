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

void gb_logger_info(const char *fmt, ...);
void gb_logger_warn(const char *fmt, ...);
void gb_logger_err(const char *fmt, ...);
void gb_logger_dbg(const char *fmt, ...);

#ifdef __cplusplus
}
#endif
