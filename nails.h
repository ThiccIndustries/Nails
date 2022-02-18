/*
 * Created by MajesticWaffle on 2/10/22.
 * Copyright (c) 2022 Thicc Industries. All rights reserved.
 * This software is under the: Thicc-Industries-Do-What-You-Want-I-Dont-Care License.
 */

#ifndef NAILS_H
#define NAILS_H

#include "geode.h"

extern Map* g_map;                  //Active map being edited
extern std::string g_nails_path;    //Resource path directory for nails

void rendering_draw_frames();
void rendering_draw_tiles();
Panel* ui_construct_menubar();
Panel* ui_construct_tile_display(uint* selected_tile, Map* map);
Panel* ui_construct_tile_properties(uint* selected_tile, Map* map);
void ui_refresh_tile_properties(Panel* panel, Map* map, uint selected_tile);

#endif
