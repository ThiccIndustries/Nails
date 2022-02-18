/*
 * Created by MajesticWaffle on 2/10/22.
 * Copyright (c) 2022 Thicc Industries. All rights reserved.
 * This software is under the: Thicc-Industries-Do-What-You-Want-I-Dont-Care License.
 */

#include "nails.h"
#include <cstdlib>

std::string g_nails_path;
std::string g_game_path;
Map* g_map;
bool g_debug = true;

void tick(){ui_tick();};
Chunk* load_chunk(Map* map, Coord2i coord);
void unload_chunk(Map* map, Chunk* chunk);

#undef RENDER_DISTANCE

int main(int argc, char* argv[]) {
    g_nails_path = argv[0];
    if(argc > 1)    //Load game if specified by command-line property
        g_game_path = argv[1];
    if(argc > 2)    //Load map if specified by command-line property
        g_map = world_map_read(strtol(argv[2], NULL, 10));

    GLFWwindow* windowptr = rendering_init_opengl(480, 360, 1, 1, 1, "Nails Map Editor");

    //Load textures
    g_def_font = new Font{
            texture_load_bmp(get_resource_path(g_nails_path, "resources/font.bmp"), TEXTURE_MULTIPLE, 8),
            R"(ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!"#$%&'()*+,-./:;<=>?@[\]^_ {|}~0123456789)"
    };

    Texture* ui = texture_load_bmp(get_resource_path(g_nails_path, "resources/ui.bmp"), TEXTURE_MULTIPLE, 16);

    glfwSwapInterval(0);

    time_set_tick_callback(&tick);
    world_set_chunk_callbacks(&load_chunk, &unload_chunk);
    input_register_callbacks(windowptr);

    Entity* entity_cam = new Entity;
    entity_cam -> position = {0, 0};
    entity_cam -> map = g_map;

    uint test = 0;

    Panel* menubar = ui_construct_menubar();
    Panel* tile = ui_construct_tile_display(&test, g_map);
    Panel* checkbox_test = ui_create_checkbox<uint>(3, COLOR_BLACK, COLOR_WHITE, &test, 0, 2);
    Panel* field = ui_construct_tile_properties(&test, g_map);

    tile -> position = {307, 292};
    field -> position = {308, 308};

    ui_dynamic_panel_activate(tile);
    ui_dynamic_panel_activate(menubar);
    ui_dynamic_panel_activate(checkbox_test);
    ui_dynamic_panel_activate(field);

    while(!glfwWindowShouldClose(windowptr)){
        input_poll_input();
        time_update_time(glfwGetTime());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world_populate_chunk_buffer(entity_cam);

        rendering_draw_chunk_buffer(entity_cam);
        rendering_draw_frames();
        rendering_draw_tiles();

        for(int i = 0; i <=  g_dynamic_panel_highest_id; ++i){
            if(g_dynamic_panel_registry[i] == nullptr)
                continue;

            rendering_draw_panel(g_dynamic_panel_registry[i]);
        }

        rendering_draw_cursor(ui, 0);
        double speed = 128.0f;
        double dx = speed * ((input_get_key(GLFW_KEY_D) ? 1 : 0) - (input_get_key(GLFW_KEY_A) ? 1 : 0));
        double dy = speed * ((input_get_key(GLFW_KEY_S) ? 1 : 0) - (input_get_key(GLFW_KEY_W) ? 1 : 0));

        entity_cam -> camera.position.x += dx * g_time -> delta;
        entity_cam -> camera.position.y += dy * g_time -> delta;
        glfwSwapBuffers(windowptr);
    }

    glfwDestroyWindow(windowptr); //Destroy window
    glfwTerminate();

    return 0;
}

Chunk* load_chunk(Map* map, Coord2i coord) {
    Chunk *chunkptr = world_chunkfile_read(map, coord);

    //Chunk successfully loaded from file
    if (chunkptr != nullptr) return chunkptr;

    chunkptr = new Chunk;
    chunkptr -> pos = coord;

    for(int i = 0; i < 256; ++i)
        chunkptr -> background_tiles[i] = 255;

    return chunkptr;
}

void unload_chunk(Map* map, Chunk* chunk){ }