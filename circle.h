//
// Created by Avery Drennan on 4/15/23.
//


#ifndef PHYSENGINE_SDL_CIRCLE_H
#define PHYSENGINE_SDL_CIRCLE_H

#include "Engine_SDL.h"
#include "SDL2_gfxPrimitives.h"
#include <math.h>
#include "engine.h"

struct circle
{
    int radius;
    double pos[2];
    double vel[2];
    double acc[2];
    int red;
    int green;
    int blue;
    int mass;
};

void circle_draw(struct engine_struct* engine,struct circle* circle_struct);

int do_circles_overlap(struct circle* circle1,struct circle* circle2);

#endif //PHYSENGINE_SDL_CIRCLE_H
