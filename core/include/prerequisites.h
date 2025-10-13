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

#define CHECK_BIT(value, bit) ((value & (1 << (bit))) != 0)
#define SET_BIT(value, bit) (value | (1 << (bit)))
#define CLEAR_BIT(value, bit) (value & ~(1 << (bit)))

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#    define LITTLE_ENDIAN 1
#    define BIG_ENDIAN 0
#else
#    define LITTLE_ENDIAN 0
#    define BIG_ENDIAN 1
#endif

#ifdef __cplusplus
}
#endif
