/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>

#include "gameboy.h"

int main(const int argc, const char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: hyper_gb <ROM>\n");
        return 1;
    }

    const char *rom = argv[1];

    struct Gameboy gameboy = { 0 };
    gameboy_init(&gameboy, rom);
    gameboy_run(&gameboy);
    gameboy_shutdown(&gameboy);

    return 0;
}
