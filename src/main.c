/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>
#include <SDL3/SDL.h>
#include <stdlib.h>

#include "cpu.h"
#include "gameboy.h"
#include "logger.h"
#include "ppu.h"

enum Color
{
    COLOR_WHITE = 0,
    COLOR_LIGHT_GRAY,
    COLOR_DARK_GRAY,
    COLOR_BLACK,
};

static uint32_t get_real_color(const enum Color color)
{
    uint8_t color_value = 0;
    switch (color)
    {
    case COLOR_WHITE: color_value = 255; break;
    case COLOR_LIGHT_GRAY: color_value = 170; break;
    case COLOR_DARK_GRAY: color_value = 85; break;
    case COLOR_BLACK: color_value = 0; break;
    default: break;
    }

    return (0xff << 24) | (color_value << 16) | (color_value << 8) | (color_value << 0);
}

static struct Gameboy *gameboy = NULL;

static void file_callback(void *user_data, const char *const *file_list, int filter)
{
    if (file_list == NULL || file_list[0] == NULL)
    {
        return;
    }

    if (gameboy)
    {
        gameboy_destroy(gameboy);
    }
    gameboy = gameboy_create(file_list[0]);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("GameBoy", 1280, 720, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, "vulkan");
    SDL_Texture *render_target
        = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, GAMEBOY_WIDTH, GAMEBOY_HEIGHT);
    SDL_SetTextureScaleMode(render_target, SDL_SCALEMODE_NEAREST);

    // 384 Tiles - 8x8 tiles
    // 24 width
    // 16 height
    const uint32_t vram_tile_data_width = 24 * 8;
    const uint32_t vram_tile_data_height = 16 * 8;
    SDL_Texture *vram_tile_data = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        (int) vram_tile_data_width, (int) vram_tile_data_height);
    SDL_SetTextureScaleMode(vram_tile_data, SDL_SCALEMODE_NEAREST);

    // 1 Tile Map - 32x32 maps - 8x8 pixels
    // 1 width
    // 1 height
    const uint32_t vram_tile_maps_width = 32 * 8;
    const uint32_t vram_tile_maps_height = 32 * 8;
    SDL_Texture *vram_tile_maps_1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        (int) vram_tile_maps_width, (int) vram_tile_maps_height);
    SDL_SetTextureScaleMode(vram_tile_maps_1, SDL_SCALEMODE_NEAREST);

    SDL_Texture *vram_tile_maps_2 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        (int) vram_tile_maps_width, (int) vram_tile_maps_height);
    SDL_SetTextureScaleMode(vram_tile_maps_2, SDL_SCALEMODE_NEAREST);

    {
        void *pixels_ptr;
        int pitch;
        SDL_LockTexture(render_target, NULL, &pixels_ptr, &pitch);
        uint32_t *pixels = pixels_ptr;
        for (uint32_t y = 0; y < GAMEBOY_HEIGHT; ++y)
        {
            for (uint32_t x = 0; x < GAMEBOY_WIDTH; ++x)
            {
                pixels[(GAMEBOY_WIDTH * y) + x] = 0xff000000;
            }
        }
        SDL_UnlockTexture(render_target);
    }

    {
        void *pixels_ptr;
        int pitch;
        SDL_LockTexture(vram_tile_data, NULL, &pixels_ptr, &pitch);
        uint32_t *pixels = pixels_ptr;
        for (uint32_t y = 0; y < vram_tile_data_height; ++y)
        {
            for (uint32_t x = 0; x < vram_tile_data_width; ++x)
            {
                pixels[(vram_tile_data_width * y) + x] = 0xff000000;
            }
        }
        SDL_UnlockTexture(vram_tile_data);
    }

    {
        void *pixels_ptr;
        int pitch;
        SDL_LockTexture(vram_tile_maps_1, NULL, &pixels_ptr, &pitch);
        uint32_t *pixels = pixels_ptr;
        for (uint32_t y = 0; y < vram_tile_maps_height; ++y)
        {
            for (uint32_t x = 0; x < vram_tile_maps_width; ++x)
            {
                pixels[(vram_tile_maps_width * y) + x] = 0xff000000;
            }
        }
        SDL_UnlockTexture(vram_tile_maps_1);
    }

    {
        void *pixels_ptr;
        int pitch;
        SDL_LockTexture(vram_tile_maps_2, NULL, &pixels_ptr, &pitch);
        uint32_t *pixels = pixels_ptr;
        for (uint32_t y = 0; y < vram_tile_maps_height; ++y)
        {
            for (uint32_t x = 0; x < vram_tile_maps_width; ++x)
            {
                pixels[(vram_tile_maps_width * y) + x] = 0xff000000;
            }
        }
        SDL_UnlockTexture(vram_tile_maps_2);
    }

    igCreateContext(NULL);

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    ImGuiIO *io = igGetIO_Nil();
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGuiStyle *style = igGetStyle();
    style->WindowBorderSize = 0.0f;
    style->FrameBorderSize = 0.0f;
    style->PopupBorderSize = 0.0f;

    ImVec4 *colors = style->Colors;

    // Primary background
    colors[ImGuiCol_WindowBg] = (ImVec4) { 0.07f, 0.07f, 0.09f, 1.00f }; // #131318
    colors[ImGuiCol_MenuBarBg] = (ImVec4) { 0.12f, 0.12f, 0.15f, 1.00f }; // #131318

    colors[ImGuiCol_PopupBg] = (ImVec4) { 0.18f, 0.18f, 0.22f, 1.00f };

    // Headers
    colors[ImGuiCol_Header] = (ImVec4) { 0.18f, 0.18f, 0.22f, 1.00f };
    colors[ImGuiCol_HeaderHovered] = (ImVec4) { 0.30f, 0.30f, 0.40f, 1.00f };
    colors[ImGuiCol_HeaderActive] = (ImVec4) { 0.25f, 0.25f, 0.35f, 1.00f };

    // Buttons
    colors[ImGuiCol_Button] = (ImVec4) { 0.20f, 0.22f, 0.27f, 1.00f };
    colors[ImGuiCol_ButtonHovered] = (ImVec4) { 0.30f, 0.32f, 0.40f, 1.00f };
    colors[ImGuiCol_ButtonActive] = (ImVec4) { 0.35f, 0.38f, 0.50f, 1.00f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = (ImVec4) { 0.15f, 0.15f, 0.18f, 1.00f };
    colors[ImGuiCol_FrameBgHovered] = (ImVec4) { 0.22f, 0.22f, 0.27f, 1.00f };
    colors[ImGuiCol_FrameBgActive] = (ImVec4) { 0.25f, 0.25f, 0.30f, 1.00f };

    // Tabs
    colors[ImGuiCol_Tab] = (ImVec4) { 0.18f, 0.18f, 0.22f, 1.00f };
    colors[ImGuiCol_TabHovered] = (ImVec4) { 0.35f, 0.35f, 0.50f, 1.00f };
    // colors[ImGuiCol_TabActive] = (ImVec4) { 0.25f, 0.25f, 0.38f, 1.00f };
    // colors[ImGuiCol_TabUnfocused] = (ImVec4) { 0.13f, 0.13f, 0.17f, 1.00f };
    // colors[ImGuiCol_TabUnfocusedActive] = (ImVec4) { 0.20f, 0.20f, 0.25f, 1.00f };

    // Title
    colors[ImGuiCol_TitleBg] = (ImVec4) { 0.12f, 0.12f, 0.15f, 1.00f };
    colors[ImGuiCol_TitleBgActive] = (ImVec4) { 0.15f, 0.15f, 0.20f, 1.00f };
    colors[ImGuiCol_TitleBgCollapsed] = (ImVec4) { 0.10f, 0.10f, 0.12f, 1.00f };

    // Borders
    colors[ImGuiCol_Border] = (ImVec4) { 0.20f, 0.20f, 0.25f, 0.50f };
    colors[ImGuiCol_BorderShadow] = (ImVec4) { 0.00f, 0.00f, 0.00f, 0.00f };

    // Text
    colors[ImGuiCol_Text] = (ImVec4) { 0.90f, 0.90f, 0.95f, 1.00f };
    colors[ImGuiCol_TextDisabled] = (ImVec4) { 0.50f, 0.50f, 0.55f, 1.00f };

    // Highlights
    colors[ImGuiCol_CheckMark] = (ImVec4) { 0.50f, 0.70f, 1.00f, 1.00f };
    colors[ImGuiCol_SliderGrab] = (ImVec4) { 0.50f, 0.70f, 1.00f, 1.00f };
    colors[ImGuiCol_SliderGrabActive] = (ImVec4) { 0.60f, 0.80f, 1.00f, 1.00f };
    colors[ImGuiCol_ResizeGrip] = (ImVec4) { 0.50f, 0.70f, 1.00f, 0.50f };
    colors[ImGuiCol_ResizeGripHovered] = (ImVec4) { 0.60f, 0.80f, 1.00f, 0.75f };
    colors[ImGuiCol_ResizeGripActive] = (ImVec4) { 0.70f, 0.90f, 1.00f, 1.00f };

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = (ImVec4) { 0.10f, 0.10f, 0.12f, 1.00f };
    colors[ImGuiCol_ScrollbarGrab] = (ImVec4) { 0.30f, 0.30f, 0.35f, 1.00f };
    colors[ImGuiCol_ScrollbarGrabHovered] = (ImVec4) { 0.40f, 0.40f, 0.50f, 1.00f };
    colors[ImGuiCol_ScrollbarGrabActive] = (ImVec4) { 0.45f, 0.45f, 0.55f, 1.00f };

    // Style tweaks
    style->WindowRounding = 5.0f;
    style->FrameRounding = 5.0f;
    style->GrabRounding = 5.0f;
    style->TabRounding = 5.0f;
    style->PopupRounding = 5.0f;
    style->ScrollbarRounding = 5.0f;
    style->WindowPadding = (ImVec2) { 10, 10 };
    style->FramePadding = (ImVec2) { 6, 4 };
    style->ItemSpacing = (ImVec2) { 8, 6 };
    style->PopupBorderSize = 0.f;

    igStyleColorsDark(NULL);

    bool should_exit = false;
    while (!should_exit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            switch (event.type)
            {
            case SDL_EVENT_QUIT: should_exit = true; break;
            default: break;
            }
        }

        if (gameboy)
        {
            for (usize i = 0; i < 4096; ++i)
            {
                gameboy_tick(gameboy);
            }
        }

        {
            void *pixels_ptr;
            int pitch;
            SDL_LockTexture(render_target, NULL, &pixels_ptr, &pitch);
            uint32_t *pixels = pixels_ptr;
            for (uint32_t y = 0; y < GAMEBOY_HEIGHT; ++y)
            {
                for (uint32_t x = 0; x < GAMEBOY_WIDTH; ++x)
                {
                    const uint8_t red = (uint8_t) (((float) y / (float) GAMEBOY_HEIGHT) * 255.0f);
                    const uint8_t green = 0x00;
                    const uint8_t blue = 0x00;
                    const uint32_t color = 0xff000000 | (blue << 16) | (green << 8) | (red << 0);
                    pixels[(GAMEBOY_WIDTH * y) + x] = color;
                }
            }
            SDL_UnlockTexture(render_target);
        }

        if (gameboy)
        {
            void *pixels_ptr;
            int pitch;
            SDL_LockTexture(vram_tile_data, NULL, &pixels_ptr, &pitch);
            uint32_t *pixels = pixels_ptr;

            // 0x8000-0x97ff
            for (uint32_t i = 0; i < 384; ++i)
            {
                // 24 = Width
                const uint32_t x = i % 24;
                const uint32_t y = i / 24;

                const uint32_t start = i * 16;
                for (uint32_t k = 0; k < 8; ++k)
                {
                    const uint8_t first_byte = gameboy->ppu->vram[start + k * 2 + 0];
                    const uint8_t second_byte = gameboy->ppu->vram[start + k * 2 + 1];

                    // Decoding Tile
                    for (uint8_t j = 0; j < 8; ++j)
                    {
                        const uint8_t low_bit = (first_byte >> (7 - j)) & 0b1;
                        const uint8_t high_bit = (second_byte >> (7 - j)) & 0b1;
                        const uint8_t color_bits = (high_bit << 1) | low_bit;

                        enum Color color_enum = COLOR_BLACK;
                        switch (color_bits)
                        {
                        case 0: color_enum = COLOR_BLACK; break;
                        case 1: color_enum = COLOR_DARK_GRAY; break;
                        case 2: color_enum = COLOR_LIGHT_GRAY; break;
                        case 3: color_enum = COLOR_WHITE; break;
                        default: break;
                        }

                        pixels[vram_tile_data_width * (y * 8 + k) + (x * 8 + j)] = get_real_color(color_enum);
                    }
                }
            }

            SDL_UnlockTexture(vram_tile_data);
        }

        if (gameboy)
        {
            void *pixels_ptr;
            int pitch;
            SDL_LockTexture(vram_tile_maps_1, NULL, &pixels_ptr, &pitch);
            uint32_t *pixels = pixels_ptr;

            for (uint32_t i = 0; i < 32 * 32; ++i)
            {
                const uint32_t x = i % 32;
                const uint32_t y = i / 32;

                const uint32_t tile = gameboy->ppu->vram[0x1800 + i];
                const uint32_t start_address = ((gameboy->ppu->lcd_control >> 4) & 0b1) == 1 ? 0x0000 : 0x0800;

                const uint32_t start = start_address + (tile * 16);
                for (uint32_t k = 0; k < 8; ++k)
                {
                    const uint8_t first_byte = gameboy->ppu->vram[start + k * 2 + 0];
                    const uint8_t second_byte = gameboy->ppu->vram[start + k * 2 + 1];

                    // Decoding Tile
                    for (uint8_t j = 0; j < 8; ++j)
                    {
                        const uint8_t low_bit = (first_byte >> (7 - j)) & 0b1;
                        const uint8_t high_bit = (second_byte >> (7 - j)) & 0b1;
                        const uint8_t color_bits = (high_bit << 1) | low_bit;

                        enum Color color_enum = COLOR_BLACK;
                        switch (color_bits)
                        {
                        case 0: color_enum = COLOR_BLACK; break;
                        case 1: color_enum = COLOR_DARK_GRAY; break;
                        case 2: color_enum = COLOR_LIGHT_GRAY; break;
                        case 3: color_enum = COLOR_WHITE; break;
                        default: break;
                        }

                        pixels[vram_tile_maps_width * (y * 8 + k) + (x * 8 + j)] = get_real_color(color_enum);
                    }
                }
            }

            SDL_UnlockTexture(vram_tile_maps_1);
        }

        if (gameboy)
        {
            void *pixels_ptr;
            int pitch;
            SDL_LockTexture(vram_tile_maps_2, NULL, &pixels_ptr, &pitch);
            uint32_t *pixels = pixels_ptr;

            for (uint32_t i = 0; i < 32 * 32; ++i)
            {
                const uint32_t x = i % 32;
                const uint32_t y = i / 32;

                const uint32_t tile = gameboy->ppu->vram[0x1c00 + i];
                const uint32_t start_address = ((gameboy->ppu->lcd_control >> 4) & 0b1) == 1 ? 0x0000 : 0x0800;

                const uint32_t start = start_address + (tile * 16);
                for (uint32_t k = 0; k < 8; ++k)
                {
                    const uint8_t first_byte = gameboy->ppu->vram[start + k * 2 + 0];
                    const uint8_t second_byte = gameboy->ppu->vram[start + k * 2 + 1];

                    // Decoding Tile
                    for (uint8_t j = 0; j < 8; ++j)
                    {
                        const uint8_t low_bit = (first_byte >> (7 - j)) & 0b1;
                        const uint8_t high_bit = (second_byte >> (7 - j)) & 0b1;
                        const uint8_t color_bits = (high_bit << 1) | low_bit;

                        enum Color color_enum = COLOR_BLACK;
                        switch (color_bits)
                        {
                        case 0: color_enum = COLOR_BLACK; break;
                        case 1: color_enum = COLOR_DARK_GRAY; break;
                        case 2: color_enum = COLOR_LIGHT_GRAY; break;
                        case 3: color_enum = COLOR_WHITE; break;
                        default: break;
                        }

                        pixels[vram_tile_maps_width * (y * 8 + k) + (x * 8 + j)] = get_real_color(color_enum);
                    }
                }
            }

            SDL_UnlockTexture(vram_tile_maps_2);
        }

        SDL_RenderClear(renderer);

        ImGui_ImplSDL3_NewFrame();
        ImGui_ImplSDLRenderer3_NewFrame();

        igNewFrame();

        ImGuiViewport *viewport = igGetMainViewport();
        igSetNextWindowPos(viewport->WorkPos, 0, (ImVec2) { 0, 0 });
        igSetNextWindowSize(viewport->WorkSize, 0);
        igSetNextWindowViewport(viewport->ID);

        static bool dockspace_open = true;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

        igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, (ImVec2) { .x = 0.0f, .y = 0.0f });
        igBegin("DockSpace", &dockspace_open, window_flags);
        igPopStyleVar(1);

        ImGuiID dockspace_id = igGetID_Str("DockSpace");
        if (!igDockBuilderGetNode(dockspace_id))
        {
            igDockBuilderRemoveNode(dockspace_id);
            igDockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);

            ImGuiID main_id = dockspace_id;
            ImGuiID left_id = igDockBuilderSplitNode(main_id, ImGuiDir_Left, 0.25f, NULL, &main_id);
            ImGuiID right_id = igDockBuilderSplitNode(main_id, ImGuiDir_Right, 0.33f, NULL, &main_id);

            igDockBuilderDockWindow("CPU", left_id);
            igDockBuilderDockWindow("Game Screen", main_id);
            igDockBuilderDockWindow("PPU", right_id);

            igDockBuilderFinish(dockspace_id);
        }

        igDockSpace(dockspace_id, (ImVec2) { .x = 0.0f, .y = 0.0f }, ImGuiDockNodeFlags_PassthruCentralNode, NULL);

        if (igBeginMenuBar())
        {
            if (igBeginMenu("File", true))
            {
                if (igMenuItem_Bool("Open", "", false, true))
                {
                    static const SDL_DialogFileFilter filters[] = {
                        { "GameBoy ROMs", "gb;rom" },
                    };
                    SDL_ShowOpenFileDialog(file_callback, NULL, window, filters, 1, NULL, false);
                }
                igEndMenu();
            }

            igEndMenuBar();
        }

        igEnd();

        igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, (ImVec2) { .x = 0.0f, .y = 0.0f });
        igBegin("Game Screen", NULL, 0);
        igPopStyleVar(1);

        {
            ImVec2 available_size;
            igGetContentRegionAvail(&available_size);

            ImVec2 cursor_screen_pos;
            igGetCursorScreenPos(&cursor_screen_pos);

            const float scale = min(available_size.x / (float) GAMEBOY_WIDTH, available_size.y / (float) GAMEBOY_HEIGHT);

            const ImVec2 image_size = { .x = GAMEBOY_WIDTH * scale, .y = GAMEBOY_HEIGHT * scale };
            const ImVec2 cursor_position = {
                .x = cursor_screen_pos.x + ceilf(max(0.0f, (available_size.x - image_size.x) * 0.5f)),
                .y = cursor_screen_pos.y + ceilf(max(0.0f, (available_size.y - image_size.y) * 0.5f)),
            };
            igSetCursorScreenPos(cursor_position);

            igImage(
                (ImTextureRef) {
                    ._TexData = NULL,
                    ._TexID = (ImTextureID) render_target,
                },
                image_size,
                (ImVec2) {
                    .x = 0.0f,
                    .y = 0.0f,
                },
                (ImVec2) {
                    .x = 1.0f,
                    .y = 1.0f,
                });
        }

        igEnd();

        igBegin("CPU", NULL, 0);
        if (igCollapsingHeader_BoolPtr("Registers", NULL, ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (gameboy)
            {
                igText("A: 0x%02x", gameboy->cpu->registers.a);
                igText("F: 0x%02x", gameboy->cpu->registers.f);
                igText("B: 0x%02x", gameboy->cpu->registers.b);
                igText("C: 0x%02x", gameboy->cpu->registers.c);
                igText("D: 0x%02x", gameboy->cpu->registers.d);
                igText("E: 0x%02x", gameboy->cpu->registers.e);
                igText("H: 0x%02x", gameboy->cpu->registers.h);
                igText("L: 0x%02x", gameboy->cpu->registers.l);
                igText("SP: 0x%04x", gameboy->cpu->registers.sp);
                igText("PC: 0x%04x", gameboy->cpu->registers.pc);
            }
        }
        igEnd();

        igBegin("PPU", NULL, 0);

        if (igCollapsingHeader_BoolPtr("VRAM Tile Data", NULL, ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImVec2 available_size;
            igGetContentRegionAvail(&available_size);

            ImVec2 cursor_screen_pos;
            igGetCursorScreenPos(&cursor_screen_pos);

            const float scale
                = min(available_size.x / (float) vram_tile_data_width, available_size.y / (float) vram_tile_data_height);

            const ImVec2 image_size
                = { .x = (float) vram_tile_data_width * scale, .y = (float) vram_tile_data_height * scale };
            const ImVec2 cursor_position = {
                .x = cursor_screen_pos.x + ceilf(max(0.0f, (available_size.x - image_size.x) * 0.5f)),
                .y = cursor_screen_pos.y,
            };
            igSetCursorScreenPos(cursor_position);

            igImage(
                (ImTextureRef) {
                    ._TexData = NULL,
                    ._TexID = (ImTextureID) vram_tile_data,
                },
                image_size,
                (ImVec2) {
                    .x = 0.0f,
                    .y = 0.0f,
                },
                (ImVec2) {
                    .x = 1.0f,
                    .y = 1.0f,
                });
        }

        if (igCollapsingHeader_BoolPtr("VRAM Tile Maps", NULL, ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImVec2 available_size;
            igGetContentRegionAvail(&available_size);

            available_size.x *= 0.49f;

            {
                ImVec2 cursor_screen_pos;
                igGetCursorScreenPos(&cursor_screen_pos);

                const float scale
                    = min(available_size.x / (float) vram_tile_maps_width, available_size.y / (float) vram_tile_maps_height);

                const ImVec2 image_size
                    = { .x = (float) vram_tile_maps_width * scale, .y = (float) vram_tile_maps_height * scale };
                const ImVec2 cursor_position = {
                    .x = cursor_screen_pos.x + ceilf(max(0.0f, (available_size.x - image_size.x) * 0.5f)),
                    .y = cursor_screen_pos.y,
                };
                igSetCursorScreenPos(cursor_position);

                igImage(
                    (ImTextureRef) {
                        ._TexData = NULL,
                        ._TexID = (ImTextureID) vram_tile_maps_1,
                    },
                    image_size,
                    (ImVec2) {
                        .x = 0.0f,
                        .y = 0.0f,
                    },
                    (ImVec2) {
                        .x = 1.0f,
                        .y = 1.0f,
                    });
            }

            igSameLine(0.0f, -1.0f);

            {
                ImVec2 cursor_screen_pos;
                igGetCursorScreenPos(&cursor_screen_pos);

                const float scale
                    = min(available_size.x / (float) vram_tile_maps_width, available_size.y / (float) vram_tile_maps_height);

                const ImVec2 image_size
                    = { .x = (float) vram_tile_maps_width * scale, .y = (float) vram_tile_maps_height * scale };
                const ImVec2 cursor_position = {
                    .x = cursor_screen_pos.x + ceilf(max(0.0f, (available_size.x - image_size.x) * 0.5f)),
                    .y = cursor_screen_pos.y,
                };
                igSetCursorScreenPos(cursor_position);

                igImage(
                    (ImTextureRef) {
                        ._TexData = NULL,
                        ._TexID = (ImTextureID) vram_tile_maps_2,
                    },
                    image_size,
                    (ImVec2) {
                        .x = 0.0f,
                        .y = 0.0f,
                    },
                    (ImVec2) {
                        .x = 1.0f,
                        .y = 1.0f,
                    });
            }
        }
        igEnd();

        igRender();

        ImGui_ImplSDLRenderer3_RenderDrawData(igGetDrawData(), renderer);

        SDL_RenderPresent(renderer);
    }

    if (gameboy)
    {
        gameboy_destroy(gameboy);
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    igDestroyContext(NULL);

    SDL_DestroyTexture(vram_tile_maps_2);
    SDL_DestroyTexture(vram_tile_maps_1);
    SDL_DestroyTexture(vram_tile_data);
    SDL_DestroyTexture(render_target);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
