/*
 * Created by MajesticWaffle on 2/11/22.
 * Copyright (c) 2022 Thicc Industries. All rights reserved.
 * This software is under the: Thicc-Industries-Do-What-You-Want-I-Dont-Care License.
 */

#include "nails.h"

void ui_menubar_click(void* v){
    Panel* p = (Panel*) v;
    *(long*)(p->packet) = g_time -> tick;
    ui_dynamic_panel_activate(p);
};

Panel* ui_menubar(){
    Panel_Button*   file_button = new Panel_Button;
    Panel_Text*     file_label = new Panel_Text;
    Panel* file_menu = new Panel;
    file_button -> packet = file_menu;
    file_button -> click_func = &ui_menubar_click;
    file_button -> p.position = {0, 0};
    file_button -> p.size = {32, 8};
    file_button -> p.child_count = 1;
    file_button -> p.children = new Panel*[]{(Panel*)file_label};
    file_label -> text = "File";
    file_label -> p.position = {1, 1};
    file_label -> p.foreground_color = COLOR_WHITE;
    file_label -> font = g_def_font;

    Panel_Button*   edit_button = new Panel_Button;
    Panel_Text*     edit_label = new Panel_Text;
    edit_button -> click_func = &ui_menubar_click;
    edit_button -> p.position = {40, 0};
    edit_button -> p.size = {32, 8};
    edit_button -> p.child_count = 1;
    edit_button -> p.children = new Panel*[]{(Panel*)edit_label};
    edit_label -> text = "Edit";
    edit_label -> p.position = {1, 1};
    edit_label -> p.foreground_color = COLOR_WHITE;
    edit_label -> font = g_def_font;

    Panel_Button*   view_button = new Panel_Button;
    Panel_Text*     view_label = new Panel_Text;
    Panel* view_menu = new Panel;
    view_button -> packet = view_menu;
    view_button -> click_func = &ui_menubar_click;
    view_button -> p.position = {80, 0};
    view_button -> p.size = {32, 8};
    view_button -> p.child_count = 1;
    view_button -> p.children = new Panel*[]{(Panel*)view_label};
    view_label -> text = "View";
    view_label -> p.position = {1, 1};
    view_label -> p.foreground_color = COLOR_WHITE;
    view_label -> font = g_def_font;

    Panel* menu_bar = new Panel;
    menu_bar -> size = {300, 9};
    menu_bar -> position = {0, 0};
    menu_bar -> has_background = true;
    menu_bar -> background_color = COLOR_BLACK;
    menu_bar -> foreground_color = COLOR_BLACK;
    menu_bar -> child_count = 3;
    menu_bar -> type = PANEL_BOX;
    menu_bar -> dynamic = true;
    menu_bar -> children = new Panel*[]{(Panel*)file_button,
                                        (Panel*)edit_button,
                                        (Panel*)view_button};

    /* File menu */
    file_menu -> type = PANEL_BOX;
    file_menu -> has_background = false;
    file_menu -> foreground_color = COLOR_BLACK;
    file_menu -> size = {160, 20};

    /* View menu */
    view_menu -> type = PANEL_BOX;
    view_menu -> has_background = false;
    view_menu -> foreground_color = COLOR_BLACK;
    view_menu -> size = {160, 20};
    Panel* cb_borders = uix_create_checkbox<bool>(1, COLOR_BLACK, COLOR_WHITE, (bool*)&g_debug, true, false);
    Panel* cb_overlay = uix_create_checkbox<bool>(1, COLOR_BLACK, COLOR_WHITE, (bool*)&g_overlays, true, false);

    Panel_Text* lbl_overlay = new Panel_Text();
    Panel_Text* lbl_borders = new Panel_Text();

    view_menu -> dynamic = true;
    long* thing = new long;
    *thing = g_time -> tick;
    view_menu -> packet = thing;
    view_menu -> tick_func = [](Panel* p, void* v){
        if(input_get_button_down_tick(GLFW_MOUSE_BUTTON_1)) {
            long tick = *(long *) v;
            if (g_time->tick - tick >= 2)
                ui_dynamic_panel_deactivate(p);
        }
    };

    view_menu -> position = {80, 9};
    view_menu -> child_count = 4;
    view_menu -> children = new Panel*[4]{cb_borders, cb_overlay, (Panel*)lbl_borders, (Panel*)lbl_overlay};

    cb_borders -> parent = view_menu;
    cb_borders -> position = {2, 2};
    cb_borders -> size = {160, 8};

    cb_overlay -> parent = view_menu;
    cb_overlay -> position = {2, 14};
    cb_overlay -> size = {160, 8};

    lbl_borders -> font = g_def_font;
    lbl_borders -> p.dynamic = false;
    lbl_borders -> text = "Hitboxs and borders";
    lbl_borders -> p.position = {8, 0};
    lbl_borders -> p.foreground_color = COLOR_WHITE;
    lbl_borders -> p.has_background = false;
    lbl_borders -> p.parent = view_menu;

    lbl_overlay -> font = g_def_font;
    lbl_overlay -> p.dynamic = false;
    lbl_overlay -> text = "Overlay tiles";
    lbl_overlay -> p.position = {8, 12};
    lbl_overlay -> p.foreground_color = COLOR_WHITE;
    lbl_overlay -> p.has_background = false;
    lbl_overlay -> p.parent = view_menu;

    return menu_bar;
}

Panel* ui_tile_display(uint* selected_tile, Panel* sprite_selector, Panel* world_click, Map* map){
    Panel*          root = new Panel();
    Panel_Text*     text = new Panel_Text();
    Panel_Sprite*   sprite = new Panel_Sprite();
    Panel*          border = new Panel();
    Panel_Button*   button = new Panel_Button();

    root -> position = {0, 0};
    root -> dynamic = true;
    root -> child_count = 4;
    root -> children = new Panel*[root -> child_count]{(Panel*)text, border, (Panel*)sprite, (Panel*)button};

    text -> p.active = false;
    text -> p.position = {0, 0};
    text -> font = g_def_font;
    text -> text = "Sprite:";
    text -> p.foreground_color = COLOR_WHITE;
    text -> p.has_background = false;


    //Why the fuck does this fix it.
    Texture* fuck = texture_generate(map -> tilemap -> image, TEXTURE_MULTIPLE, 16);

    button -> p.position = {(int)text -> text.length() * (int)text -> font->t->tile_size, (int)fuck -> tile_size / -4};
    button -> p.size = {(int)fuck->tile_size, (int)fuck->tile_size};
    button -> p.parent = root;

    Panel** arr = new Panel*[2]{sprite_selector, world_click};
    button -> packet = (void*)arr;
    button -> click_func = [](void* v){
        Panel** arr = (Panel**)v;
        ui_dynamic_panel_deactivate(arr[1]);
        ui_dynamic_panel_activate(arr[0]);
    };

    sprite -> texture = fuck;
    sprite -> atlas_index = 0;
    sprite -> p.position = {(int)text -> text.length() * (int)text -> font->t->tile_size, (int)fuck -> tile_size / -4};
    sprite -> p.dynamic = true;
    sprite -> p.has_background = false;
    typedef struct{
        uint* selected_tile;
        Map* map;
    }Packet;

    sprite -> p.packet = new Packet{selected_tile, map};
    sprite -> p.tick_func = [](Panel* p, void* v){
        Packet* pac = (Packet*)v;

        ((Panel_Sprite*)p) -> atlas_index = pac->map->tile_properties[*(pac -> selected_tile)]. atlas_index;
    };

    border -> type = PANEL_BOX;
    border -> position = sprite->p.position;
    border -> size = { (int)sprite->texture->tile_size, (int)sprite->texture->tile_size };
    border -> background_color = COLOR_WHITE;
    border -> foreground_color = COLOR_BLACK;
    border -> has_background = true;

    return root;
}

Panel* ui_tile_properties(){
    Panel* container = new Panel;

    Panel_Text* lbl_solid = new Panel_Text();
    Panel_Text* lbl_flipX = new Panel_Text();
    Panel_Text* lbl_flipY = new Panel_Text();
    Panel_Text* lbl_anime = new Panel_Text();

    lbl_solid -> font = g_def_font;
    lbl_solid -> p.dynamic = false;
    lbl_solid -> text = "Solid";
    lbl_solid -> p.position = {10, 0};
    lbl_solid -> p.foreground_color = COLOR_WHITE;
    lbl_solid -> p.has_background = false;

    lbl_flipX -> font = g_def_font;
    lbl_flipX -> p.dynamic = false;
    lbl_flipX -> text = "Flip Vertically";
    lbl_flipX -> p.position = {10, 12};
    lbl_flipX -> p.foreground_color = COLOR_WHITE;
    lbl_flipX -> p.has_background = false;

    lbl_flipY -> font = g_def_font;
    lbl_flipY -> p.dynamic = false;
    lbl_flipY -> text = "Flip Horizontally";
    lbl_flipY -> p.position = {10, 24};
    lbl_flipY -> p.foreground_color = COLOR_WHITE;
    lbl_flipY -> p.has_background = false;

    lbl_anime -> font = g_def_font;
    lbl_anime -> p.dynamic = false;
    lbl_anime -> text = "Animate";
    lbl_anime -> p.position = {10, 36};
    lbl_anime -> p.foreground_color = COLOR_WHITE;
    lbl_anime -> p.has_background = false;

    Panel* cb_solid = uix_create_checkbox<bool>(2, COLOR_BLACK, COLOR_WHITE, &g_active_tile.solid, true, false);
    Panel* cb_flipX = uix_create_checkbox<bool>(2, COLOR_BLACK, COLOR_WHITE, &g_active_tile.flipY, true, false);
    Panel* cb_flipY = uix_create_checkbox<bool>(2, COLOR_BLACK, COLOR_WHITE, &g_active_tile.flipX, true, false);
    Panel* cb_anime = uix_create_checkbox<bool>(2, COLOR_BLACK, COLOR_WHITE, &g_active_tile.animated, true, false);

    cb_solid -> parent = cb_flipX -> parent = cb_flipY -> parent = cb_anime -> parent = container;

    cb_solid->position = {0, 0};
    cb_flipX->position = {0, 12};
    cb_flipY->position = {0, 24};
    cb_anime->position = {0, 36};

    container -> dynamic = true;
    container -> position = {0, 0};
    container -> child_count = 8;
    container -> children = new Panel*[8]{
            (Panel*)lbl_solid,
            (Panel*)lbl_flipX,
            (Panel*)lbl_flipY,
            (Panel*)lbl_anime,
            (Panel*)cb_solid,
            (Panel*)cb_flipX,
            (Panel*)cb_flipY,
            (Panel*)cb_anime
    };

    Tile* old_properties = new Tile;
    *old_properties = g_active_tile;
    typedef struct{
        uint old_id;
        Panel* p;
    } Packet;

    container -> packet = new Packet{g_active_tile.id, container};

    container -> tick_func = [](Panel* p, void* v){
        Packet* packet = (Packet*)v;

        if(g_active_tile.id != packet->old_id){
            packet -> old_id = g_active_tile.id;
            for(int i = 4; i < 8; ++i)
                uix_refresh_checkbox<bool>(packet -> p->children[i]);
        }

        uchar opts = (g_active_tile.solid ? TILE_SOLID : 0) | (g_active_tile.flipX ? TILE_TEX_FLIP_X : 0) | (g_active_tile.flipY ? TILE_TEX_FLIP_Y : 0) | (g_active_tile.animated ? TILE_ANIMATED : 0);

        if(g_active_tile.map -> tile_properties[g_active_tile.id].options != opts){

            world_set_tile_properties(g_active_tile);
            for(int i = 0; i < (RENDER_DISTANCE * 2 + 1) * (RENDER_DISTANCE * 2 + 1); ++i){
                world_chunk_refresh_metatextures(g_active_tile.map, g_chunk_buffer[i]);
            }
        }
    };

    return container;
}

Panel* ui_tile_select(uint* id_out){
    Panel_Button* button = new Panel_Button();
    button -> p.position = {314, 14};
    button -> p.size = {144, 256};

    typedef struct{
        uint* out;
        Panel* p;
    }Packet;

    button -> packet = (void*) new Packet{id_out, (Panel*)button};
    button -> click_func = [](void* v){
        Packet* p = (Packet*)v;

        int max_tile = g_map -> tile_count - 1;
        Coord2d pos = input_mouse_position();
        pos.x = pos.x / (g_video_mode.window_scale);
        pos.y = pos.y / (g_video_mode.window_scale);

        Coord2i gpos = ui_panel_global_position(p->p);
        int x = (pos.x - gpos.x) / g_map -> tilemap->tile_size;
        int y = (pos.y - gpos.y) / g_map -> tilemap->tile_size;
        int i = (y * 9) + x + 1;

        *p -> out = clampi(i, 1, max_tile);
    };

    return (Panel*)button;
}


Panel* ui_new_tile(Texture* texture, uint atlas_index){
    Panel_Button* button = new Panel_Button();
    Panel_Sprite* sprite = new Panel_Sprite();
    button -> p.size = {(int)texture->tile_size, (int)texture->tile_size};
    sprite -> p.size = {(int)texture->tile_size, (int)texture->tile_size};
    sprite -> texture = texture;
    sprite -> atlas_index = atlas_index;

    button -> p.child_count = 1;
    button -> p.children = new Panel*[]{(Panel*)sprite};
    sprite -> p.parent = (Panel*)button;

    button -> packet = button;
    button -> click_func = [](void* v){
        Panel* p = (Panel*)v;
        world_add_tile(g_map, Tile{0, g_map, false, false, false, false});
        int x = 314 + ((g_map->tile_count - 1) % 9) * g_map -> tilemap->tile_size;
        int y = 14 + ((g_map->tile_count - 1) / 9) * g_map -> tilemap->tile_size;
        p -> position = {x, y};
    };

    int x = 314 + ((g_map->tile_count - 1) % 9) * g_map -> tilemap->tile_size;
    int y = 14 + ((g_map->tile_count - 1) / 9) * g_map -> tilemap->tile_size;
    button -> p.position = {x, y};

    return (Panel*)button;
}

Panel* ui_tile_sprite(uint* active, Panel* world_click){
    Panel* container = new Panel;
    Panel_Sprite* display = new Panel_Sprite;
    Panel_Text* text = new Panel_Text;
    Panel* weird = new Panel;
    Panel_Button* button = new Panel_Button;

    Texture* tilemap = texture_generate(g_map -> tilemap -> image, TEXTURE_SINGLE, g_map -> tilemap -> width);

    int x = (g_video_mode.window_resolution.x / 2) - (int)(tilemap->width + 16) / 2;
    int y = (g_video_mode.window_resolution.y / 2) - (int)(tilemap->height + 10 + 16) / 2;

    container -> position = {x-8, y-8};
    container -> size = {(int)tilemap->width + 16, (int)tilemap -> height + 10 + 16};
    container -> type = PANEL_BOX;
    container -> foreground_color = COLOR_WHITE;
    container -> child_count = 4;
    container -> has_background = false;
    container -> children = new Panel*[]{weird, (Panel*) display, (Panel*)text, (Panel*)button};
    container -> dynamic = true;

    weird -> position = {4, 4};
    weird -> size = {(int)tilemap->width + 8, (int)tilemap -> height + 10 + 8};
    weird -> type = PANEL_BOX;
    weird -> has_background = false;
    weird -> foreground_color = COLOR_BLACK;
    weird -> parent = container;

    display -> p.position = {8, 18};
    display -> p.size = {(int)tilemap->width, (int)tilemap -> height};
    display -> p.has_background = false;
    display -> p.parent = container;
    display -> texture = tilemap;
    display -> atlas_index = 0;
    display -> p.parent = container;

    text -> p.position = {8, 8};
    text -> font = g_def_font;
    text -> text = "Select sprite:";
    text -> p.foreground_color = COLOR_WHITE;
    text -> p.parent = container;

    button -> p.position = {8, 18};
    button -> p.size = {(int)tilemap->width, (int)tilemap -> height};
    button -> p.parent = container;

    typedef struct{
        Panel* container;
        Panel* button;
        Panel* activate;
        uint* active_tile;
    }Packet;

    button -> packet = new Packet{container, (Panel*)button, world_click, active};
    button -> click_func = [](void* v){
        Packet* p = (Packet*)v;
        Coord2d pos = input_mouse_position();
        pos.x = pos.x / (g_video_mode.window_scale);
        pos.y = pos.y / (g_video_mode.window_scale);
        Coord2i ppos = ui_panel_global_position(p -> button);
        pos.x -= ppos.x;
        pos.y -= ppos.y;

        //Assume pos is in bounds because how would it not be.
        int tile_x = pos.x / g_map->tilemap->tile_size;
        int tile_y = pos.y / g_map->tilemap->tile_size;
        int tile_i = (tile_y * (g_map->tilemap->width / g_map->tilemap->tile_size)) + tile_x;

        g_map->tile_properties[*p->active_tile].atlas_index = tile_i;

        for(int i = 0; i < (RENDER_DISTANCE * 2 + 1) * (RENDER_DISTANCE * 2 + 1); ++i){
            world_chunk_refresh_metatextures(g_active_tile.map, g_chunk_buffer[i]);
        }

        ui_dynamic_panel_deactivate(p->container);
        ui_dynamic_panel_activate(p->activate);
    };

    return container;
}

Panel* ui_world_click(){
    Panel_Button* button = new Panel_Button;
    button -> p.position = {4, 14};
    button -> p.size = {292, 342};
    button -> click_func = &world_click;

    typedef struct{
        Map* map;
        Tool tool;
    }Packet;

    Packet* p = new Packet{g_map, g_tool};
    button -> packet = p;
    return (Panel*)button;
}