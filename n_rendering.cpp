/*
 * Created by MajesticWaffle on 2/10/22.
 * Copyright (c) 2022 Thicc Industries. All rights reserved.
 * This software is under the: Thicc-Industries-Do-What-You-Want-I-Dont-Care License.
 */

#include "nails.h"
void rendering_draw_chunks(){
    glColor1c(COLOR_BLACK);
    glBegin(GL_QUADS);{
        glVertex2i(300, 0);
        glVertex2i(480, 0);
        glVertex2i(480, 360);
        glVertex2i(300, 360);
    }
    glEnd();
    glColor1c(COLOR_WHITE);
}

void rendering_draw_frames(){
    glBegin(GL_QUADS);{
        glVertex2i(0,   10);
        glVertex2i(300, 10);
        glVertex2i(300, 14);
        glVertex2i(0,   14);

        glVertex2i(0,   14);
        glVertex2i(4,   14);
        glVertex2i(4,   360);
        glVertex2i(0,   360);

        glVertex2i(0,   356);
        glVertex2i(300, 356);
        glVertex2i(300, 360);
        glVertex2i(0,   360);

        glVertex2i(296, 14);
        glVertex2i(300, 14);
        glVertex2i(300, 360);
        glVertex2i(296, 360);
    }
    glEnd();

    glColor1c(COLOR_WHITE);
    glBegin(GL_QUADS);{
        glVertex2i(310, 10);
        glVertex2i(462, 10);
        glVertex2i(462, 274);
        glVertex2i(310, 274);
    }
    glColor1c(COLOR_BLACK);
    glBegin(GL_QUADS);{
        glVertex2i(314, 14);
        glVertex2i(458, 14);
        glVertex2i(458, 270);
        glVertex2i(314, 270);
    }
    glEnd();
    glColor1c(COLOR_WHITE);
}

void rendering_draw_tiles(uint selected_tile){
    selected_tile--;
    uint tile_count = g_map -> tile_count;
    uint tile_size  = g_map -> tilemap -> tile_size;
    double tile_dx = g_map -> tilemap -> atlas_uvs.x;
    double tile_dy = g_map -> tilemap -> atlas_uvs.y;

    //TODO: Please figure this out
    Texture* fuck = texture_generate(g_map -> tilemap -> image, TEXTURE_MULTIPLE, 16);

    int x = 314;
    int y = 14;
    uint tiles_x = 9;


    glEnable(GL_TEXTURE_2D);
    texture_bind(fuck, 0);

    double uv_x1, uv_y1, uv_x2, uv_y2;

    for(uint i = 1; i < tile_count; ++i){
        //Two different textures means chunk must be animated.
        int anim_frame = 0;
        if(g_map -> tile_properties[i].options & TILE_ANIMATED){
            int anim_tick = g_time -> tick % TIME_TPS;
            int anim_rate_d = TIME_TPS / 4;
            anim_frame += anim_tick / anim_rate_d;
        }

        uv_x1 = tile_dx * ((g_map -> tile_properties[i].atlas_index + anim_frame * (fuck -> width / fuck -> tile_size)) % (fuck -> width / fuck -> tile_size));
        uv_x2 = uv_x1 + tile_dx;

        uv_y1 = tile_dy * ((g_map -> tile_properties[i].atlas_index + anim_frame * (fuck -> width / fuck -> tile_size)) / (fuck -> width / fuck -> tile_size));
        uv_y2 = uv_y1 + tile_dx;

        //Swap for flips
        if(g_map -> tile_properties[i].options & TILE_TEX_FLIP_X){
            uv_x2 += uv_x1;
            uv_x1 = uv_x2 - uv_x1;
            uv_x2 -= uv_x1;
        }
        if(g_map -> tile_properties[i].options & TILE_TEX_FLIP_Y){
            uv_y2 += uv_y1;
            uv_y1 = uv_y2 - uv_y1;
            uv_y2 -= uv_y1;
        }

        glBegin(GL_QUADS);{
            glTexCoord2d(uv_x1, uv_y1); glVertex2i(x + (((i - 1) % tiles_x) * tile_size),             y + (((i - 1) / tiles_x) * tile_size));
            glTexCoord2d(uv_x2, uv_y1); glVertex2i(x + (((i - 1) % tiles_x) * tile_size) + tile_size, y + (((i - 1) / tiles_x) * tile_size));
            glTexCoord2d(uv_x2, uv_y2); glVertex2i(x + (((i - 1) % tiles_x) * tile_size) + tile_size, y + (((i - 1) / tiles_x) * tile_size) + tile_size);
            glTexCoord2d(uv_x1, uv_y2); glVertex2i(x + (((i - 1) % tiles_x) * tile_size),             y + (((i - 1) / tiles_x) * tile_size) + tile_size);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    texture_destroy(fuck);

    glColor1c(COLOR_RED);
    glLineWidth(2);
    glBegin(GL_LINES);
    {
        glVertex2i(x + (selected_tile % tiles_x) * tile_size, y + (selected_tile / tiles_x) * tile_size);
        glVertex2i(x + (selected_tile % tiles_x) * tile_size + tile_size, y + (selected_tile / tiles_x) * tile_size);

        glVertex2i(x + (selected_tile % tiles_x) * tile_size + tile_size, y + (selected_tile / tiles_x) * tile_size);
        glVertex2i(x + (selected_tile % tiles_x) * tile_size + tile_size, y + (selected_tile / tiles_x) * tile_size + tile_size);

        glVertex2i(x + (selected_tile % tiles_x) * tile_size + tile_size, y + (selected_tile / tiles_x) * tile_size + tile_size);
        glVertex2i(x + (selected_tile % tiles_x) * tile_size, y + (selected_tile / tiles_x) * tile_size + tile_size);

        glVertex2i(x + (selected_tile % tiles_x) * tile_size, y + (selected_tile / tiles_x) * tile_size + tile_size);
        glVertex2i(x + (selected_tile % tiles_x) * tile_size, y + (selected_tile / tiles_x) * tile_size);
    }
    glEnd();
    glColor1c(COLOR_WHITE);
}
