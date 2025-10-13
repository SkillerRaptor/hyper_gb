/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include "emulator.h"

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>
#include <gb.h>
#include <stdlib.h>

#define TILE_DATA_WIDTH 24
#define TILE_DATA_HEIGHT 16

#define TILE_MAPS_WIDTH 64
#define TILE_MAPS_HEIGHT 32

#define OAM_WIDTH 10
#define OAM_HEIGHT 4

static SDL_Texture *emulator_create_texture(struct Emulator *, uint32_t width, uint32_t height);

static void setup_imgui_style(void);

static void emulator_poll_events(struct Emulator *);
static void emulator_update(struct Emulator *);
static void emulator_render_textures(struct Emulator *);

static void emulator_render_game_screen_texture(struct Emulator *);

static void render_tile(uint32_t x, uint32_t y, const uint8_t *vram, uint32_t vram_offset, uint32_t width, uint32_t *pixels);
static void emulator_render_tile_data_texture(struct Emulator *);
static void emulator_render_tile_maps_texture(struct Emulator *);
static void emulator_render_oam_texture(struct Emulator *);

static void file_callback(void *user_data, const char *const *file_list, int filter);
static void emulator_render_imgui(struct Emulator *);
static void emulator_show_cpu_state(struct Emulator *);
static void emulator_show_ppu_state(struct Emulator *);
static void emulator_show_game_screen(struct Emulator *);
static void emulator_show_tile_data(struct Emulator *);
static void emulator_show_tile_maps(struct Emulator *);
static void emulator_show_oam(struct Emulator *);

struct Emulator *emulator_create(void)
{
    SDL_Init(SDL_INIT_VIDEO);

    struct Emulator *emulator = malloc(sizeof(struct Emulator));
    emulator->window = SDL_CreateWindow("GameBoy", 1280, 720, SDL_WINDOW_RESIZABLE);
    emulator->renderer = SDL_CreateRenderer(emulator->window, "vulkan");
    emulator->game_screen_texture = emulator_create_texture(emulator, GB_SCREEN_WIDTH, GB_SCREEN_HEIGHT);
    emulator->tile_data_texture
        = emulator_create_texture(emulator, TILE_DATA_WIDTH * GB_TILE_SIZE, TILE_DATA_HEIGHT * GB_TILE_SIZE);
    emulator->tile_maps_texture
        = emulator_create_texture(emulator, TILE_MAPS_WIDTH * GB_TILE_SIZE, TILE_MAPS_HEIGHT * GB_TILE_SIZE);
    emulator->oam_texture = emulator_create_texture(emulator, OAM_WIDTH * GB_TILE_SIZE, OAM_HEIGHT * GB_TILE_SIZE);
    emulator->gb = NULL;
    emulator->should_close = false;

    igCreateContext(NULL);
    ImGui_ImplSDL3_InitForSDLRenderer(emulator->window, emulator->renderer);
    ImGui_ImplSDLRenderer3_Init(emulator->renderer);

    setup_imgui_style();

    return emulator;
}

void emulator_destroy(struct Emulator *emulator)
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    igDestroyContext(NULL);

    if (emulator->gb)
    {
        gameboy_destroy(emulator->gb);
    }

    SDL_DestroyTexture(emulator->oam_texture);
    SDL_DestroyTexture(emulator->tile_maps_texture);
    SDL_DestroyTexture(emulator->tile_data_texture);
    SDL_DestroyTexture(emulator->game_screen_texture);
    SDL_DestroyRenderer(emulator->renderer);
    SDL_DestroyWindow(emulator->window);
    SDL_Quit();

    free(emulator);
}

void emulator_run(struct Emulator *emulator)
{
    const uint64_t performance_frequency = SDL_GetPerformanceFrequency();
    uint64_t last_frame_time = SDL_GetPerformanceCounter();
    double lag = 0.0;

    while (!emulator->should_close)
    {
        emulator_poll_events(emulator);

        const uint64_t now = SDL_GetPerformanceCounter();
        const double elapsed = (double) (now - last_frame_time) / (double) performance_frequency;
        last_frame_time = now;
        lag += elapsed;

        while (lag >= GB_FRAME_TIME)
        {
            emulator_update(emulator);
            lag -= GB_FRAME_TIME;
        }

        emulator_render_textures(emulator);
        emulator_render_imgui(emulator);

        // Frame Pacing
        const uint64_t target_time = last_frame_time + (uint64_t) (GB_FRAME_TIME * (double) performance_frequency);
        const uint64_t current = SDL_GetPerformanceCounter();
        if (current < target_time)
        {
            const double wait_seconds = (double) (target_time - current) / (double) performance_frequency;
            if (wait_seconds > 0)
            {
                SDL_Delay((uint32_t) (wait_seconds * 1000.0));
            }
        }
    }
}

SDL_Texture *emulator_create_texture(struct Emulator *emulator, const uint32_t width, const uint32_t height)
{
    SDL_Texture *texture = SDL_CreateTexture(
        emulator->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, (int) width, (int) height);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    int pitch = 0;
    void *pixels_ptr = NULL;
    SDL_LockTexture(texture, NULL, &pixels_ptr, &pitch);

    uint32_t *pixels = pixels_ptr;
    for (uint32_t i = 0; i < width * height; ++i)
    {
        pixels[i] = 0xff000000;
    }

    SDL_UnlockTexture(texture);

    return texture;
}

void setup_imgui_style(void)
{
    ImGuiIO *io = igGetIO_Nil();
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->FontDefault = ImFontAtlas_AddFontFromFileTTF(io->Fonts, "./assets/fonts/Ruda-Regular.ttf", 16.0f, NULL, NULL);

    ImGuiStyle *style = igGetStyle();

    // Main
    style->WindowPadding = (ImVec2) { .x = 4.0f, .y = 4.0f };
    style->FramePadding = (ImVec2) { .x = 4.0f, .y = 4.0f };
    style->ItemSpacing = (ImVec2) { .x = 6.0f, .y = 6.0f };
    style->ItemInnerSpacing = (ImVec2) { .x = 6.0f, .y = 6.0f };
    style->TouchExtraPadding = (ImVec2) { .x = 0.0f, .y = 0.0f };
    // IndentSpacing
    style->GrabMinSize = 8.0f;

    // Borders
    style->WindowBorderSize = 0.0f;
    style->ChildBorderSize = 0.0f;
    style->PopupBorderSize = 0.0f;
    style->FrameBorderSize = 0.0f;

    // Rounding
    style->WindowRounding = 2.0f;
    style->ChildRounding = 2.0f;
    style->FrameRounding = 2.0f;
    style->PopupRounding = 2.0f;
    style->GrabRounding = 2.0f;

    // Scrollbar
    style->ScrollbarSize = 12.0f;
    style->ScrollbarRounding = 4.0f;
    style->ScrollbarPadding = 2.0f;

    ImVec4 *colors = style->Colors;
    colors[ImGuiCol_Text] = (ImVec4) { 0.90f, 0.90f, 0.95f, 1.00f };
    colors[ImGuiCol_TextDisabled] = (ImVec4) { 0.50f, 0.50f, 0.55f, 1.00f };
    colors[ImGuiCol_WindowBg] = (ImVec4) { 0.07f, 0.07f, 0.09f, 1.00f };
    colors[ImGuiCol_ChildBg] = (ImVec4) { 0.00f, 0.00f, 0.00f, 0.00f };
    colors[ImGuiCol_PopupBg] = (ImVec4) { 0.18f, 0.18f, 0.22f, 1.00f };
    colors[ImGuiCol_Border] = (ImVec4) { 0.20f, 0.20f, 0.25f, 0.50f };
    colors[ImGuiCol_BorderShadow] = (ImVec4) { 0.00f, 0.00f, 0.00f, 0.00f };
    colors[ImGuiCol_FrameBg] = (ImVec4) { 0.15f, 0.15f, 0.18f, 1.00f };
    colors[ImGuiCol_FrameBgHovered] = (ImVec4) { 0.22f, 0.22f, 0.27f, 1.00f };
    colors[ImGuiCol_FrameBgActive] = (ImVec4) { 0.25f, 0.25f, 0.30f, 1.00f };
    colors[ImGuiCol_TitleBg] = (ImVec4) { 0.12f, 0.12f, 0.15f, 1.00f };
    colors[ImGuiCol_TitleBgActive] = (ImVec4) { 0.15f, 0.15f, 0.20f, 1.00f };
    colors[ImGuiCol_TitleBgCollapsed] = (ImVec4) { 0.10f, 0.10f, 0.12f, 1.00f };
    colors[ImGuiCol_MenuBarBg] = (ImVec4) { 0.12f, 0.12f, 0.15f, 1.00f };
    colors[ImGuiCol_ScrollbarBg] = (ImVec4) { 0.10f, 0.10f, 0.12f, 1.00f };
    colors[ImGuiCol_ScrollbarGrab] = (ImVec4) { 0.30f, 0.30f, 0.35f, 1.00f };
    colors[ImGuiCol_ScrollbarGrabHovered] = (ImVec4) { 0.40f, 0.40f, 0.50f, 1.00f };
    colors[ImGuiCol_ScrollbarGrabActive] = (ImVec4) { 0.45f, 0.45f, 0.55f, 1.00f };
    colors[ImGuiCol_CheckMark] = (ImVec4) { 0.50f, 0.70f, 1.00f, 1.00f };
    colors[ImGuiCol_SliderGrab] = (ImVec4) { 0.50f, 0.70f, 1.00f, 1.00f };
    colors[ImGuiCol_SliderGrabActive] = (ImVec4) { 0.60f, 0.80f, 1.00f, 1.00f };
    colors[ImGuiCol_Button] = (ImVec4) { 0.20f, 0.22f, 0.27f, 1.00f };
    colors[ImGuiCol_ButtonHovered] = (ImVec4) { 0.30f, 0.32f, 0.40f, 1.00f };
    colors[ImGuiCol_ButtonActive] = (ImVec4) { 0.35f, 0.38f, 0.50f, 1.00f };
    colors[ImGuiCol_Header] = (ImVec4) { 0.18f, 0.18f, 0.22f, 1.00f };
    colors[ImGuiCol_HeaderHovered] = (ImVec4) { 0.30f, 0.30f, 0.40f, 1.00f };
    colors[ImGuiCol_HeaderActive] = (ImVec4) { 0.25f, 0.25f, 0.35f, 1.00f };
    // ImGuiCol_Separator
    // ImGuiCol_SeparatorHovered
    // ImGuiCol_SeparatorActive
    colors[ImGuiCol_ResizeGrip] = (ImVec4) { 0.50f, 0.70f, 1.00f, 0.50f };
    colors[ImGuiCol_ResizeGripHovered] = (ImVec4) { 0.60f, 0.80f, 1.00f, 0.75f };
    colors[ImGuiCol_ResizeGripActive] = (ImVec4) { 0.70f, 0.90f, 1.00f, 1.00f };
    colors[ImGuiCol_InputTextCursor] = (ImVec4) { 1.00f, 1.00f, 1.00f, 1.00f };
    colors[ImGuiCol_TabHovered] = (ImVec4) { 0.35f, 0.35f, 0.50f, 1.00f };
    colors[ImGuiCol_Tab] = (ImVec4) { 0.18f, 0.18f, 0.22f, 1.00f };
    // ImGuiCol_TabSelected
    // ImGuiCol_TabSelectedOverline
    // ImGuiCol_TabDimmed
    // ImGuiCol_TabDimmedSelected
    // ImGuiCol_TabDimmedSelectedOverline
    // ImGuiCol_DockingPreview
    // ImGuiCol_DockingEmptyBg
    // ImGuiCol_PlotLines
    // ImGuiCol_PlotLinesHovered
    // ImGuiCol_PlotHistogram
    // ImGuiCol_PlotHistogramHovered
    // ImGuiCol_TableHeaderBg
    // ImGuiCol_TableBorderStrong
    // ImGuiCol_TableBorderLight
    // ImGuiCol_TableRowBg
    // ImGuiCol_TableRowBgAlt
    // ImGuiCol_TextLink
    // ImGuiCol_TextSelectedBg
    // ImGuiCol_TreeLines
    // ImGuiCol_DragDropTarget
    // ImGuiCol_NavCursor
    // ImGuiCol_NavWindowingHighlight
    // ImGuiCol_NavWindowingDimBg
    // ImGuiCol_ModalWindowDimBg
}

void emulator_poll_events(struct Emulator *emulator)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);

        switch (event.type)
        {
        case SDL_EVENT_QUIT: emulator->should_close = true; break;
        default: break;
        }
    }
}

void emulator_update(struct Emulator *emulator)
{
    if (emulator->gb)
    {
        gameboy_run_frame(emulator->gb);
    }
}

void emulator_render_textures(struct Emulator *emulator)
{
    if (!emulator->gb)
    {
        return;
    }

    emulator_render_game_screen_texture(emulator);
    emulator_render_tile_data_texture(emulator);
    emulator_render_tile_maps_texture(emulator);
    emulator_render_oam_texture(emulator);
}

void emulator_render_game_screen_texture(struct Emulator *emulator) {}

void render_tile(const uint32_t x,
    const uint32_t y,
    const uint8_t *vram,
    const uint32_t vram_offset,
    const uint32_t width,
    uint32_t *pixels)
{
    for (uint32_t line = 0; line < 8; ++line)
    {
        const uint32_t real_y = (y * GB_TILE_SIZE) + line;

        const uint8_t first_byte = vram[vram_offset + (line * 2)];
        const uint8_t second_byte = vram[vram_offset + (line * 2) + 1];

        // Decoding Tile
        for (uint8_t bit = 0; bit < 8; ++bit)
        {
            const uint32_t real_x = (x * GB_TILE_SIZE) + bit;

            const uint8_t low_bit = (first_byte >> (7 - bit)) & 0b1;
            const uint8_t high_bit = (second_byte >> (7 - bit)) & 0b1;
            const uint8_t color_bits = (high_bit << 1) | low_bit;

            uint32_t color = 0xff000000;
            switch (color_bits)
            {
            case 0b00: color = 0xff000000; break;
            case 0b01: color = 0xff555555; break;
            case 0b10: color = 0xffaaaaaa; break;
            case 0b11: color = 0xffffffff; break;
            default: break;
            }

            pixels[((width * GB_TILE_SIZE) * real_y) + real_x] = color;
        }
    }
}

void emulator_render_tile_data_texture(struct Emulator *emulator)
{
    int pitch = 0;
    void *pixels_ptr = NULL;
    SDL_LockTexture(emulator->tile_data_texture, NULL, &pixels_ptr, &pitch);

    uint32_t *pixels = pixels_ptr;
    for (uint32_t i = 0; i < TILE_DATA_WIDTH * TILE_DATA_HEIGHT; ++i)
    {
        // NOTES: Tiles are 16 bytes long
        const uint32_t x = i % TILE_DATA_WIDTH;
        const uint32_t y = i / TILE_DATA_WIDTH;
        const uint32_t start_address = i * 16;
        render_tile(x, y, emulator->gb->ppu->vram, start_address, TILE_DATA_WIDTH, pixels);
    }

    SDL_UnlockTexture(emulator->tile_data_texture);
}

void emulator_render_tile_maps_texture(struct Emulator *emulator)
{
    int pitch = 0;
    void *pixels_ptr = NULL;
    SDL_LockTexture(emulator->tile_maps_texture, NULL, &pixels_ptr, &pitch);

    const uint32_t block_start = GB_BIT_CHECK(emulator->gb->ppu->lcd_control, 4) ? 0x0000 : 0x0800;

    uint32_t *pixels = pixels_ptr;
    for (uint32_t i = 0; i < (TILE_MAPS_WIDTH / 2) * TILE_MAPS_HEIGHT; ++i)
    {
        const uint32_t x = i % (TILE_MAPS_WIDTH / 2);
        const uint32_t y = i / (TILE_MAPS_WIDTH / 2);

        const uint32_t tile = emulator->gb->ppu->vram[0x1800 + i];
        const uint32_t start_address = block_start + (tile * 16);
        render_tile(x, y, emulator->gb->ppu->vram, start_address, TILE_MAPS_WIDTH, pixels);
    }

    for (uint32_t i = 0; i < (TILE_MAPS_WIDTH / 2) * TILE_MAPS_HEIGHT; ++i)
    {
        const uint32_t x = i % (TILE_MAPS_WIDTH / 2);
        const uint32_t y = i / (TILE_MAPS_WIDTH / 2);

        const uint32_t tile = emulator->gb->ppu->vram[0x1c00 + i];
        const uint32_t start_address = block_start + (tile * 16);
        render_tile(x + 32, y, emulator->gb->ppu->vram, start_address, TILE_MAPS_WIDTH, pixels);
    }

    SDL_UnlockTexture(emulator->tile_maps_texture);
}

void emulator_render_oam_texture(struct Emulator *emulator)
{
    int pitch = 0;
    void *pixels_ptr = NULL;
    SDL_LockTexture(emulator->oam_texture, NULL, &pixels_ptr, &pitch);

    uint32_t *pixels = pixels_ptr;
    for (uint32_t i = 0; i < OAM_WIDTH * OAM_HEIGHT; ++i)
    {
        const uint32_t x = i % OAM_WIDTH;
        const uint32_t y = i / OAM_WIDTH;

        const uint32_t tile_index = emulator->gb->mmu->oam[(i * 4) + 0x02];
        const uint32_t start = tile_index * 16;
        render_tile(x, y, emulator->gb->ppu->vram, start, OAM_WIDTH, pixels);
    }

    SDL_UnlockTexture(emulator->oam_texture);
}

static void file_callback(void *user_data, const char *const *file_list, const int filter)
{
    (void) filter;

    if (file_list == NULL || file_list[0] == NULL)
    {
        return;
    }

    struct Emulator *emulator = user_data;

    if (emulator->gb)
    {
        gameboy_destroy(emulator->gb);
    }

    emulator->gb = gameboy_create(file_list[0]);
}

void emulator_render_imgui(struct Emulator *emulator)
{
    SDL_RenderClear(emulator->renderer);

    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplSDLRenderer3_NewFrame();

    igNewFrame();

    ImGuiViewport *viewport = igGetMainViewport();
    igSetNextWindowPos(viewport->WorkPos, 0, (ImVec2) { 0, 0 });
    igSetNextWindowSize(viewport->WorkSize, 0);
    igSetNextWindowViewport(viewport->ID);

    static bool dockspace_open = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

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
        ImGuiID left_top_id = left_id;
        ImGuiID left_middle_id = igDockBuilderSplitNode(left_top_id, ImGuiDir_Down, 0.8f, NULL, &left_top_id);
        ImGuiID left_bottom_id = igDockBuilderSplitNode(left_middle_id, ImGuiDir_Down, 0.6f, NULL, &left_middle_id);

        ImGuiID right_id = igDockBuilderSplitNode(main_id, ImGuiDir_Right, 0.33f, NULL, &main_id);
        ImGuiID right_top_id = right_id;
        ImGuiID right_middle_id = igDockBuilderSplitNode(right_top_id, ImGuiDir_Down, 0.6f, NULL, &right_top_id);
        ImGuiID right_bottom_id = igDockBuilderSplitNode(right_middle_id, ImGuiDir_Down, 0.5f, NULL, &right_middle_id);

        igDockBuilderDockWindow("CPU State", left_top_id);
        igDockBuilderDockWindow("PPU State", left_middle_id);
        igDockBuilderDockWindow("Game Screen", main_id);
        igDockBuilderDockWindow("Tile Data", right_top_id);
        igDockBuilderDockWindow("Tile Maps", right_middle_id);
        igDockBuilderDockWindow("OAM", right_bottom_id);

        igDockBuilderFinish(dockspace_id);
    }

    igDockSpace(dockspace_id, (ImVec2) { .x = 0.0f, .y = 0.0f },
        ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_PassthruCentralNode, NULL);

    if (igBeginMenuBar())
    {
        if (igBeginMenu("File", true))
        {
            if (igMenuItem_Bool("Open", "", false, true))
            {
                static const SDL_DialogFileFilter filters[] = {
                    { "GameBoy ROMs", "gb;rom" },
                };

                SDL_ShowOpenFileDialog(file_callback, emulator, emulator->window, filters, 1, NULL, false);
            }
            igEndMenu();
        }

        igEndMenuBar();
    }

    igEnd();

    emulator_show_cpu_state(emulator);
    emulator_show_ppu_state(emulator);
    emulator_show_game_screen(emulator);
    emulator_show_tile_data(emulator);
    emulator_show_tile_maps(emulator);
    emulator_show_oam(emulator);

    igRender();
    ImGui_ImplSDLRenderer3_RenderDrawData(igGetDrawData(), emulator->renderer);

    SDL_RenderPresent(emulator->renderer);
}

void emulator_show_cpu_state(struct Emulator *emulator)
{
    igBegin("CPU State", NULL, ImGuiWindowFlags_NoScrollbar);

    igSeparatorText("Registers");
    if (igBeginTable("Registers", 4, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_RowBg, (ImVec2) { 0.0f, 0.0f }, 0.0f))
    {
        struct Registers registers = { 0 };
        if (emulator->gb)
        {
            registers = emulator->gb->cpu->registers;
        }

        igTableNextRow(0, 0.0f);

        igTableNextColumn();
        igText("A");
        igTableNextColumn();
        igPushStyleColor_U32(ImGuiCol_Text, 0xff808080);
        igText("0x%02x", registers.a);
        igPopStyleColor(1);

        igTableNextColumn();
        igText("F");
        igTableNextColumn();
        igPushStyleColor_U32(ImGuiCol_Text, 0xff808080);
        igText("0x%02x", registers.f);
        igPopStyleColor(1);

        igTableNextRow(0, 0.0f);
        igTableNextColumn();
        igText("B");
        igTableNextColumn();
        igPushStyleColor_U32(ImGuiCol_Text, 0xff808080);
        igText("0x%02x", registers.b);
        igPopStyleColor(1);

        igTableNextColumn();
        igText("C");
        igTableNextColumn();
        igPushStyleColor_U32(ImGuiCol_Text, 0xff808080);
        igText("0x%02x", registers.c);
        igPopStyleColor(1);

        igTableNextRow(0, 0.0f);
        igTableNextColumn();
        igText("D");
        igTableNextColumn();
        igPushStyleColor_U32(ImGuiCol_Text, 0xff808080);
        igText("0x%02x", registers.d);
        igPopStyleColor(1);

        igTableNextColumn();
        igText("E");
        igTableNextColumn();
        igPushStyleColor_U32(ImGuiCol_Text, 0xff808080);
        igText("0x%02x", registers.e);
        igPopStyleColor(1);

        igTableNextRow(0, 0.0f);
        igTableNextColumn();
        igText("H");
        igTableNextColumn();
        igPushStyleColor_U32(ImGuiCol_Text, 0xff808080);
        igText("0x%02x", registers.h);
        igPopStyleColor(1);
        igTableNextColumn();
        igText("L");
        igTableNextColumn();
        igPushStyleColor_U32(ImGuiCol_Text, 0xff808080);
        igText("0x%02x", registers.l);
        igPopStyleColor(1);

        igTableNextRow(0, 0.0f);
        igTableNextColumn();
        igText("SP");
        igTableNextColumn();
        igPushStyleColor_U32(ImGuiCol_Text, 0xff808080);
        igText("0x%04x", registers.sp);
        igPopStyleColor(1);

        igTableNextColumn();
        igText("PC");
        igTableNextColumn();
        igPushStyleColor_U32(ImGuiCol_Text, 0xff808080);
        igText("0x%04x", registers.pc);
        igPopStyleColor(1);

        igEndTable();
    }

    igSeparatorText("Flags");

    if (igBeginTable("Flags", 4, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_RowBg, (ImVec2) { 0.0f, 0.0f }, 0.0f))
    {
        igTableNextRow(0, 0.0f);

        igTableNextColumn();
        igText("Zero");
        igTableNextColumn();
        bool zero = emulator->gb ? cpu_is_flag(emulator->gb->cpu, FLAG_Z) : false;
        igCheckbox("##Zero", &zero);

        igTableNextColumn();
        igText("Subtraction");
        igTableNextColumn();
        bool subtraction = emulator->gb ? cpu_is_flag(emulator->gb->cpu, FLAG_N) : false;
        igCheckbox("##Subtraction", &subtraction);

        igTableNextRow(0, 0.0f);
        igTableNextColumn();
        igText("Half Carry");
        igTableNextColumn();
        bool half_carry = emulator->gb ? cpu_is_flag(emulator->gb->cpu, FLAG_H) : false;
        igCheckbox("##HalfCarry", &half_carry);

        igTableNextColumn();
        igText("Carry");
        igTableNextColumn();
        bool carry = emulator->gb ? cpu_is_flag(emulator->gb->cpu, FLAG_C) : false;
        igCheckbox("##Carry", &carry);

        igEndTable();
    }
    igEnd();
}

void emulator_show_ppu_state(struct Emulator *emulator)
{
    igBegin("PPU State", NULL, ImGuiWindowFlags_NoScrollbar);
    igEnd();
}

void emulator_show_game_screen(struct Emulator *emulator)
{
    igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, (ImVec2) { .x = 0.0f, .y = 0.0f });
    igBegin("Game Screen", NULL, ImGuiWindowFlags_NoScrollbar);
    igPopStyleVar(1);

    ImVec2 available_size;
    igGetContentRegionAvail(&available_size);

    ImVec2 cursor_screen_pos;
    igGetCursorScreenPos(&cursor_screen_pos);

    const float scale = min(available_size.x / (float) GB_SCREEN_WIDTH, available_size.y / (float) GB_SCREEN_HEIGHT);
    const ImVec2 image_size = {
        .x = GB_SCREEN_WIDTH * scale,
        .y = GB_SCREEN_HEIGHT * scale,
    };
    const ImVec2 cursor_position = {
        .x = cursor_screen_pos.x + ceilf(max(0.0f, (available_size.x - image_size.x) * 0.5f)),
        .y = cursor_screen_pos.y + ceilf(max(0.0f, (available_size.y - image_size.y) * 0.5f)),
    };
    igSetCursorScreenPos(cursor_position);

    igImage(
        (ImTextureRef) {
            ._TexData = NULL,
            ._TexID = (ImTextureID) emulator->game_screen_texture,
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

    igEnd();
}

void emulator_show_tile_data(struct Emulator *emulator)
{
    igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, (ImVec2) { .x = 0.0f, .y = 0.0f });
    igBegin("Tile Data", NULL, ImGuiWindowFlags_NoScrollbar);
    igPopStyleVar(1);

    ImVec2 available_size;
    igGetContentRegionAvail(&available_size);

    ImVec2 cursor_screen_pos;
    igGetCursorScreenPos(&cursor_screen_pos);

    const float scale = min(available_size.x / (float) (TILE_DATA_WIDTH * GB_TILE_SIZE),
        available_size.y / (float) (TILE_DATA_HEIGHT * GB_TILE_SIZE));
    const ImVec2 image_size = {
        .x = (float) (TILE_DATA_WIDTH * GB_TILE_SIZE) * scale,
        .y = (float) (TILE_DATA_HEIGHT * GB_TILE_SIZE) * scale,
    };
    const ImVec2 cursor_position = {
        .x = cursor_screen_pos.x + ceilf(max(0.0f, (available_size.x - image_size.x) * 0.5f)),
        .y = cursor_screen_pos.y,
    };
    igSetCursorScreenPos(cursor_position);

    igImage(
        (ImTextureRef) {
            ._TexData = NULL,
            ._TexID = (ImTextureID) emulator->tile_data_texture,
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

    igEnd();
}

void emulator_show_tile_maps(struct Emulator *emulator)
{
    igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, (ImVec2) { .x = 0.0f, .y = 0.0f });
    igBegin("Tile Maps", NULL, ImGuiWindowFlags_NoScrollbar);
    igPopStyleVar(1);

    ImVec2 available_size;
    igGetContentRegionAvail(&available_size);

    ImVec2 cursor_screen_pos;
    igGetCursorScreenPos(&cursor_screen_pos);

    const float scale = min(available_size.x / (float) (TILE_MAPS_WIDTH * GB_TILE_SIZE),
        available_size.y / (float) (TILE_MAPS_HEIGHT * GB_TILE_SIZE));
    const ImVec2 image_size = {
        .x = (float) (TILE_MAPS_WIDTH * GB_TILE_SIZE) * scale,
        .y = (float) (TILE_MAPS_HEIGHT * GB_TILE_SIZE) * scale,
    };
    const ImVec2 cursor_position = {
        .x = cursor_screen_pos.x + ceilf(max(0.0f, (available_size.x - image_size.x) * 0.5f)),
        .y = cursor_screen_pos.y,
    };
    igSetCursorScreenPos(cursor_position);

    igImage(
        (ImTextureRef) {
            ._TexData = NULL,
            ._TexID = (ImTextureID) emulator->tile_maps_texture,
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

    igEnd();
}

void emulator_show_oam(struct Emulator *emulator)
{
    igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, (ImVec2) { .x = 0.0f, .y = 0.0f });
    igBegin("OAM", NULL, ImGuiWindowFlags_NoScrollbar);
    igPopStyleVar(1);

    ImVec2 available_size;
    igGetContentRegionAvail(&available_size);

    ImVec2 cursor_screen_pos;
    igGetCursorScreenPos(&cursor_screen_pos);

    const float scale
        = min(available_size.x / (float) (OAM_WIDTH * GB_TILE_SIZE), available_size.y / (float) (OAM_HEIGHT * GB_TILE_SIZE));
    const ImVec2 image_size = {
        .x = (float) (OAM_WIDTH * GB_TILE_SIZE) * scale,
        .y = (float) (OAM_HEIGHT * GB_TILE_SIZE) * scale,
    };
    const ImVec2 cursor_position = {
        .x = cursor_screen_pos.x + ceilf(max(0.0f, (available_size.x - image_size.x) * 0.5f)),
        .y = cursor_screen_pos.y,
    };
    igSetCursorScreenPos(cursor_position);

    igImage(
        (ImTextureRef) {
            ._TexData = NULL,
            ._TexID = (ImTextureID) emulator->oam_texture,
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

    igEnd();
}
