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

#define GB_SCREEN_WIDTH 160
#define GB_SCREEN_HEIGHT 144
#define GB_TILE_SIZE 8

#define GB_MASTER_CLOCK_HZ 4194304.0
#define GB_FRAME_HZ 59.73
#define GB_FRAME_TIME (1.0 / GB_FRAME_HZ)
#define GB_FRAME_CYCLES (GB_MASTER_CLOCK_HZ / GB_FRAME_HZ)

#ifdef __cplusplus
}
#endif
