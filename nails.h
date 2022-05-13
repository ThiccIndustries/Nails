/*
 * Created by MajesticWaffle on 2/10/22.
 * Copyright (c) 2022 Thicc Industries. All rights reserved.
 * This software is under the: Thicc-Industries-Do-What-You-Want-I-Dont-Care License.
 */

#ifndef NAILS_H
#define NAILS_H

#include "geode.h"

enum Tool{
    CURSOR,
    PENCIL,
    ERASE,
    FILL,
    SELECT
};

typedef struct Tile{
    uint id;
    Map* map;
    bool solid;
    bool flipX;
    bool flipY;
    bool animated;
} Tile;

typedef struct NChunk{
    Chunk c;
    bool empty;

}NChunk;

typedef struct UndoList{
    uint change_count;
    Coord2i previous_locations;
    Tile* previous_tiles;
    UndoList* next = nullptr;
}UndoList;

extern UndoList* g_undobuffer;
extern Map* g_map;                  //Active map being edited
extern Tool g_tool;
extern Tile g_active_tile;          //Active tile selected
extern uint g_selected_index;        //TODO: this is gross
extern Entity* g_camera;
extern std::string g_nails_path;    //Resource path directory for nails

void rendering_draw_frames();
void rendering_draw_chunks();
void rendering_draw_tiles(uint selected_tile);

Panel* ui_menubar();
Panel* ui_tile_display(uint* selected_tile, Panel* sprite_selector, Panel* world_click, Map* map);
Panel* ui_tile_properties();
Panel* ui_tile_select(uint* id_out);
Panel* ui_new_tile(Texture* texture, uint atlas_index);
Panel* ui_tile_sprite(uint* active_tile, Panel* world_click);
Panel* ui_world_click();

Tile world_get_tile_properties(uint index, Map* map);
void world_set_tile_properties(Tile tile);

void world_add_tile(Map* map, Tile tile);
void world_remove_tile(Map* map, uint index);
void world_click(void* v);

#endif
