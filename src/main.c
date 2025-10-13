/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "emulator.h"

int main()
{
    struct Emulator *emulator = emulator_create();
    emulator_run(emulator);
    emulator_destroy(emulator);

    return 0;
}
