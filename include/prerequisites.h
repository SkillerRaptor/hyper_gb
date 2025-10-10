/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#    define LITTLE_ENDIAN 1
#    define BIG_ENDIAN 0
#else
#    define LITTLE_ENDIAN 0
#    define BIG_ENDIAN 1
#endif
