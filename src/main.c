/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include <gameboy.h>
#include <stdio.h>

int main(const int argc, const char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: hyper_gb <ROM>\n");
        return 1;
    }

    const char *rom = argv[1];

    struct Gameboy *gameboy = gameboy_create(rom);
    gameboy_run(gameboy);
    gameboy_destroy(gameboy);

    return 0;
}
