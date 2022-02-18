/*
 * Created by MajesticWaffle on 2/10/22.
 * Copyright (c) 2022 Thicc Industries. All rights reserved.
 * This software is under the: Thicc-Industries-Do-What-You-Want-I-Dont-Care License.
 */

#include "nails.h"
void rendering_draw_tiles(){
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
}
