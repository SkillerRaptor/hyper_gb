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

#define GB_BIT(n) (1ull << (n))
#define GB_BIT_SET(x, n) ((x) |= GB_BIT(n))
#define GB_BIT_CLEAR(x, n) ((x) &= ~GB_BIT(n))
#define GB_BIT_CHECK(x, n) (((x) & GB_BIT(n)) != 0)
#define GB_BIT_VALUE(x, n) (((x) >> (n)) & 0b1)

#ifdef __cplusplus
}
#endif
