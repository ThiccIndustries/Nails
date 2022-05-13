/*
 * Created by MajesticWaffle on 3/6/22.
 * Copyright (c) 2022 Thicc Industries. All rights reserved.
 * This software is under the: Thicc-Industries-Do-What-You-Want-I-Dont-Care License.
 */

#include <cstring>
#include "nails.h"

Tile g_active_tile;

Tile world_get_tile_properties(uint index, Map* map){
    return Tile{
            index,
            map,
            (map->tile_properties[index].options & TILE_SOLID) > 0,
            (map->tile_properties[index].options & TILE_TEX_FLIP_X) > 0,
            (map->tile_properties[index].options & TILE_TEX_FLIP_Y) > 0,
            (map->tile_properties[index].options & TILE_ANIMATED) > 0};
}

void world_set_tile_properties(Tile tile){
    Map* map = tile.map;

    map -> tile_properties[tile.id].options = (tile.solid ? TILE_SOLID : 0)
                                            | (tile.flipX ? TILE_TEX_FLIP_X : 0)
                                            | (tile.flipY ? TILE_TEX_FLIP_Y: 0)
                                            | (tile.animated ? TILE_ANIMATED : 0);

}

void world_select_active_tile(uint index, Map* map){
    g_active_tile = world_get_tile_properties(index, map);
}

void world_add_tile(Map* map, Tile tile){
    Block* properties = new Block[map -> tile_count + 1];
    memcpy(properties, map -> tile_properties, map -> tile_count * sizeof(Block));

    uchar options =   (tile.solid ? TILE_SOLID : 0)
                    | (tile.flipX ? TILE_TEX_FLIP_X : 0)
                    | (tile.flipY ? TILE_TEX_FLIP_Y: 0)
                    | (tile.animated ? TILE_ANIMATED : 0);

    properties[map -> tile_count] = {tile.id, 0x00, 0, 0};

    delete[] map -> tile_properties;
    map -> tile_properties = properties;
    map -> tile_count++;
}

void world_remove_tile(Map* map, uint index){
    Block* properties = new Block[map -> tile_count - 1];
    memcpy(properties, map -> tile_properties, (index - 1) * sizeof(Block));
    memcpy(&properties[index], &map -> tile_properties[index], (map -> tile_count - index) * sizeof(Block));

    delete[] map -> tile_properties;
    map -> tile_properties = properties;
    map -> tile_count++;
}

//Execute tool function
void world_click(Map* map, Chunk* chunk, Coord2i position, Tool tool){

    switch(tool){
        case CURSOR:
            g_selected_index = chunk -> background_tiles[(position.y * 16) + position.x];
            break;
    }
}

//Execute tool function, button edition
void world_click(void* v){
    typedef struct{
        Map* map;
        Tool tool;
    }Packet;

    Packet* p = (Packet*)v;

    double scl = g_video_mode.ui_scale;

    Coord2d worldspace_pos = rendering_viewport_to_world_pos(g_camera, input_mouse_position());

    Coord2i chunk{(int) floor(worldspace_pos.x / 256.0),
                  (int) floor(worldspace_pos.y / 256.0)};

    Coord2i tile{(int) (worldspace_pos.x - (chunk.x * 256)) / 16,
                 (int) (worldspace_pos.y - (chunk.y * 256)) / 16};

    Chunk* chunkptr = world_get_chunk(chunk);

    world_click(p->map, chunkptr, tile, p->tool);
}


