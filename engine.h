//
// Created by Avery Drennan on 4/13/23.
//

#ifndef PHYSENGINE_SDL_ENGINE_H
#define PHYSENGINE_SDL_ENGINE_H
#define n 3
#define n1 (int) tgamma(n + 1)

#include <SDL.h>
#include "Engine_SDL.h"
#include "collisionPair.h"
#include "math.h"
struct engine_struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    struct event_data* engine_event_data;
    struct circle* circle_array[n];
    struct collisionPair* pair_array[6];
    int pair_count;

    struct circle* selected_circle;
};


#endif //PHYSENGINE_SDL_ENGINE_H
