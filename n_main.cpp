/*
 * Created by MajesticWaffle on 2/10/22.
 * Copyright (c) 2022 Thicc Industries. All rights reserved.
 * This software is under the: Thicc-Industries-Do-What-You-Want-I-Dont-Care License.
 */

#include "nails.h"
#include <cstdlib>
#include <cstring>

std::string g_nails_path = "";
std::string g_game_path = "";
uint g_selected_index = 1;

Map* g_map = nullptr;
Entity* g_camera;
Tool g_tool = CURSOR;
Option g_debug = true;

void tick(){
    entity_tick();
    ui_tick();
};
Chunk* load_chunk(Map* map, Coord2i coord);
void unload_chunk(Map* map, Chunk* chunk);

int main(int argc, char* argv[]) {
    g_nails_path = argv[0];
    if(argc > 1)    //Load game if specified by command-line property
        g_game_path = argv[1];
    if(argc > 2) {    //Load map if specified by command-line property
        g_map = world_map_read(strtol(argv[2], NULL, 10));

    }

    GLFWwindow* windowptr = rendering_init_opengl(480, 360, 1, 1, 1, "Nails Map Editor", false);

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


    g_camera = entity_create();
    g_camera -> transform -> position = {0, 0};
    g_camera -> transform -> map = g_map;

    Panel* menubar = ui_menubar();
    ui_dynamic_panel_activate(menubar);

    /*Prestart loops*/
    //No game selected
    while(!glfwWindowShouldClose(windowptr) && g_game_path.empty()){
        input_poll_input();
        time_update_time(glfwGetTime());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        rendering_draw_frames();
        rendering_draw_text("Select a game folder.", 1, g_def_font, COLOR_WHITE, {150 - (double)(strlen("Select a game folder.") / 2) * g_def_font->t->tile_size, 50});
        rendering_draw_text("File", 1, g_def_font, COLOR_WHITE, {150 - (double)(strlen("Select a game folder.") / 2) * g_def_font->t->tile_size, 70});
        rendering_draw_text("-> Open game", 1, g_def_font, COLOR_WHITE, {150 - (double)(strlen("Select a game folder.") / 2) * g_def_font->t->tile_size, 80});

        for(int i = 0; i <=  g_dynamic_panel_highest_id; ++i){
            if(g_dynamic_panel_registry[i] == nullptr)
                continue;

            rendering_draw_panel(g_dynamic_panel_registry[i]);
        }

        rendering_draw_cursor(ui, 0);
        glfwSwapBuffers(windowptr);
    }
    //No map selected
    while(!glfwWindowShouldClose(windowptr) && g_map == nullptr){
        input_poll_input();
        time_update_time(glfwGetTime());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        rendering_draw_frames();
        rendering_draw_text("Select a map.", 1, g_def_font, COLOR_WHITE, {150 - (double)(strlen("Select a map.") / 2) * g_def_font->t->tile_size, 50});
        rendering_draw_text("File", 1, g_def_font, COLOR_WHITE, {150 - (double)(strlen("Select a map.") / 2) * g_def_font->t->tile_size, 70});
        rendering_draw_text("-> Open map", 1, g_def_font, COLOR_WHITE, {150 - (double)(strlen("Select a map.") / 2) * g_def_font->t->tile_size, 80});

        for(int i = 0; i <=  g_dynamic_panel_highest_id; ++i){
            if(g_dynamic_panel_registry[i] == nullptr)
                continue;

            rendering_draw_panel(g_dynamic_panel_registry[i]);
        }

        rendering_draw_cursor(ui, 0);
        glfwSwapBuffers(windowptr);
    }

    if(glfwWindowShouldClose(windowptr)){
        glfwDestroyWindow(windowptr); //Destroy window
        glfwTerminate();
        return 0;
    }

    g_active_tile = world_get_tile_properties(0, g_map);

    Panel* checkbox_test = ui_create_checkbox<uint>(3, COLOR_BLACK, COLOR_WHITE, &g_selected_index, 1, 2);
    Panel* field = ui_tile_properties();
    Panel* tile_sel = ui_tile_select(&g_selected_index);
    Panel* new_tile = ui_new_tile(ui, 8);
    Panel* world = ui_world_click();
    Panel* tile_sprite = ui_tile_sprite(&g_selected_index, world);
    Panel* tile = ui_tile_display(&g_active_tile.id, tile_sprite, world, g_map);

    tile -> position = {308, 292};
    field -> position = {311, 308};

    ui_dynamic_panel_activate(tile);
    ui_dynamic_panel_activate(tile_sel);
    ui_dynamic_panel_activate(field);
    ui_dynamic_panel_activate(new_tile);
    ui_dynamic_panel_activate(world);

    while(!glfwWindowShouldClose(windowptr)){
        input_poll_input();
        time_update_time(glfwGetTime());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world_populate_chunk_buffer(g_camera);

        //TODO: put this not here
        if(option_changed(&g_overlays)){
            for(int i = 0; i < (RENDER_DISTANCE * 2 + 1) * (RENDER_DISTANCE * 2 + 1); ++i){
                world_chunk_refresh_metatextures(g_active_tile.map, g_chunk_buffer[i]);
            }
        }

        rendering_draw_chunk_buffer(g_camera);
        rendering_draw_chunks();
        rendering_draw_frames();
        rendering_draw_tiles(g_selected_index);

        //TODO: gross
        if(g_selected_index != g_active_tile.id)
        {
            g_active_tile = world_get_tile_properties(g_selected_index, g_map);
        }

        for(int i = 0; i <=  g_dynamic_panel_highest_id; ++i){
            if(g_dynamic_panel_registry[i] == nullptr)
                continue;

            rendering_draw_panel(g_dynamic_panel_registry[i]);
        }


        rendering_draw_cursor(ui, 0);
        double speed = 128.0f;
        double dx = speed * ((input_get_key(GLFW_KEY_D) ? 1 : 0) - (input_get_key(GLFW_KEY_A) ? 1 : 0));
        double dy = speed * ((input_get_key(GLFW_KEY_S) ? 1 : 0) - (input_get_key(GLFW_KEY_W) ? 1 : 0));

        g_camera-> transform->velocity = {dx / TIME_TPS, dy / TIME_TPS};
        glfwSwapBuffers(windowptr);
    }

    world_map_write(g_map);
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

    return chunkptr;
}

void unload_chunk(Map* map, Chunk* chunk){
    world_chunkfile_write(map, chunk);
}