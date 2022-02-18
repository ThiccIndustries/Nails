/*
 * Created by MajesticWaffle on 2/11/22.
 * Copyright (c) 2022 Thicc Industries. All rights reserved.
 * This software is under the: Thicc-Industries-Do-What-You-Want-I-Dont-Care License.
 */

#include "geode.h"

void ui_menubar_file(void* v){std::cout << "file" << std::endl;}
void ui_menubar_edit(void* v){std::cout << "edit" << std::endl;}
void ui_menubar_view(void* v){std::cout << "view" << std::endl;}

Panel* ui_construct_menubar(){
    Panel_Button*   file_button = new Panel_Button;
    Panel_Text*     file_label = new Panel_Text;

    file_button -> click_func = &ui_menubar_file;
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
    edit_button -> click_func = &ui_menubar_edit;
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
    view_button -> click_func = &ui_menubar_view;
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

    return menu_bar;
}

Panel* ui_construct_tile_display(uint* selected_tile, Map* map){
    Panel*          root = new Panel();
    Panel_Text*     text = new Panel_Text();
    Panel_Sprite*   sprite = new Panel_Sprite();
    Panel*          border = new Panel();

    root -> position = {0, 0};
    root -> dynamic = true;
    root -> child_count = 3;
    root -> children = new Panel*[root -> child_count]{(Panel*)text, border, (Panel*)sprite};

    text -> p.active = false;
    text -> p.position = {0, 0};
    text -> font = g_def_font;
    text -> text = "Current Tile:";
    text -> p.foreground_color = COLOR_WHITE;
    text -> p.has_background = false;

    //Why the fuck does this fix it.
    Texture* fuck = texture_generate(map -> tilemap -> image, TEXTURE_MULTIPLE, 16);

    sprite -> texture = fuck;
    sprite -> atlas_index = 0;
    sprite -> p.position = {(int)text -> text.length() * (int)text -> font->t->tile_size, (int)fuck -> tile_size / -4};
    sprite -> p.dynamic = true;

    sprite -> p.packet = (void*)selected_tile;
    sprite -> p.tick_func = [](Panel* p, void* v){
        ((Panel_Sprite*)p) -> atlas_index = *(uint*)v;
    };

    border -> type = PANEL_BOX;
    border -> position = sprite->p.position;
    border -> size = { (int)sprite->texture->tile_size, (int)sprite->texture->tile_size };
    border -> background_color = COLOR_WHITE;
    border -> foreground_color = COLOR_BLACK;
    border -> has_background = true;

    return root;
}

void ui_refresh_tile_properties(Panel* panel, Map* map, uint selected_tile){
    bool props[] = {(map -> tile_properties[selected_tile].options & TILE_SOLID) > 0,
                    (map -> tile_properties[selected_tile].options & TILE_TEX_FLIP_X) > 0,
                    (map -> tile_properties[selected_tile].options & TILE_TEX_FLIP_Y) > 0,
                    (map -> tile_properties[selected_tile].options & TILE_ANIMATED) > 0};

    typedef struct Packet{
        Color fg;
        Color bg;
        Panel* indicator;
        bool* out;
        bool sel;
        bool dsel;
    } Packet;

    for(int i = 0; i < 4; ++i){
        Packet* packet = ((Packet*)panel -> children[4 + i] -> packet);
        *packet -> out = props[i];
        packet -> indicator -> foreground_color = *packet -> out ? packet -> bg : packet -> fg;
    }
}

Panel* ui_construct_tile_properties(uint* selected_tile, Map* map){
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
    lbl_flipX -> text = "Flip Horizontally";
    lbl_flipX -> p.position = {10, 12};
    lbl_flipX -> p.foreground_color = COLOR_WHITE;
    lbl_flipX -> p.has_background = false;

    lbl_flipY -> font = g_def_font;
    lbl_flipY -> p.dynamic = false;
    lbl_flipY -> text = "Flip Vertically";
    lbl_flipY -> p.position = {10, 24};
    lbl_flipY -> p.foreground_color = COLOR_WHITE;
    lbl_flipY -> p.has_background = false;

    lbl_anime -> font = g_def_font;
    lbl_anime -> p.dynamic = false;
    lbl_anime -> text = "Animate";
    lbl_anime -> p.position = {10, 36};
    lbl_anime -> p.foreground_color = COLOR_WHITE;
    lbl_anime -> p.has_background = false;

    bool* test = new bool;
    *test = false;

    bool* solid = new bool{(map -> tile_properties[*selected_tile].options & TILE_SOLID) > 0};
    bool* flipX = new bool{(map -> tile_properties[*selected_tile].options & TILE_TEX_FLIP_X) > 0};
    bool* flipY = new bool{(map -> tile_properties[*selected_tile].options & TILE_TEX_FLIP_Y) > 0};
    bool* anime = new bool{(map -> tile_properties[*selected_tile].options & TILE_ANIMATED) > 0};

    Panel* cb_solid = ui_create_checkbox<bool>(2, COLOR_BLACK, COLOR_WHITE, solid, true, false);
    Panel* cb_flipX = ui_create_checkbox<bool>(2, COLOR_BLACK, COLOR_WHITE, flipX, true, false);
    Panel* cb_flipY = ui_create_checkbox<bool>(2, COLOR_BLACK, COLOR_WHITE, flipY, true, false);
    Panel* cb_anime = ui_create_checkbox<bool>(2, COLOR_BLACK, COLOR_WHITE, anime, true, false);

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

    typedef struct Packet{
        Map* map;
        uint* selected_tile;
        bool* solid;
        bool* flipX;
        bool* flipY;
        bool* anime;
    } Packet;

    container -> packet = new Packet{map, selected_tile, solid, flipX, flipY, anime};

    container -> tick_func = [](Panel* p, void* v){
        Packet* pac = (Packet*)v;
        std::cout << (int)*(pac -> anime) << std::endl;

        uchar new_opts = *pac->solid << 0 | *pac->flipX << 2 | *pac->flipY << 4 | *pac->anime << 8;
        if(pac -> map -> tile_properties[ *(pac -> selected_tile) ].options != new_opts){
            pac -> map -> tile_properties[ *(pac -> selected_tile) ].options = new_opts;
            for(int i = 0; i <= (RENDER_DISTANCE * 2) * (RENDER_DISTANCE * 2) + 1; ++i){
                world_chunk_refresh_metatextures(pac -> map, g_chunk_buffer[i]);
            }
        }

    };

    return container;
}