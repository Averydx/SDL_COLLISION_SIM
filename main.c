//
// Created by Avery Drennan on 4/13/23.
//
#include "Engine_SDL.h"
#include "engine.h"
#include "circle.h"
#include "SDL2_framerate.h"

int main() {

    FPSmanager* fps_manager = malloc(sizeof(struct FPSmanager*));
    SDL_initFramerate(fps_manager);
    SDL_setFramerate( fps_manager, 60);
    struct engine_struct* engine = engine_init("Test",800,600);
    engine->engine_event_data = (void*)malloc(sizeof(struct event_data));
    struct circle* circle_struct_0 = (void*)malloc(sizeof(struct circle));
    struct circle* circle_struct_1 = (void*)malloc(sizeof(struct circle));
    struct circle* circle_struct_2 = (void*)malloc(sizeof(struct circle));

    circle_struct_0->pos[0] = 400;
    circle_struct_0->pos[1] = 400;
    circle_struct_0->vel[0] = 1;
    circle_struct_0->vel[1] = 1;
    circle_struct_0->mass =1;


    circle_struct_0->radius = 40;
    circle_struct_0->red = 0;
    circle_struct_0->green = 255;
    circle_struct_0->blue = 0;
    engine->circle_array[0] = circle_struct_0;




    circle_struct_1->pos[0] = 600;
    circle_struct_1->pos[1] = 400;
    circle_struct_1->radius = 40;
    circle_struct_1->red = 255;
    circle_struct_1->green = 0;
    circle_struct_1->blue = 0;
    circle_struct_1->mass = 1;
    engine->circle_array[1] = circle_struct_1;

    circle_struct_2->pos[0] = 700;
    circle_struct_2->pos[1] = 400;
    circle_struct_2->radius = 40;
    circle_struct_2->red = 255;
    circle_struct_2->green = 0;
    circle_struct_2->blue = 0;
    circle_struct_2->mass = 1;
    engine->circle_array[2] = circle_struct_2;


    SDL_Event event;
    int close = 0;
    while(close != 1)
    {
        event_handler(event,engine);
        close = engine->engine_event_data->quit;
        update(engine);
        for(int i = 0; i < engine->pair_count;i++)
        {
            dynamic_circle_collision(engine->pair_array[i]);
        }

        rendering(engine);

        for(int i = 0;i<engine->pair_count-1;i++)
        {
            free(engine->pair_array[i]);
        }

        engine->pair_count = 0;

        SDL_framerateDelay(fps_manager);


    }

    engine_quit(engine);
    return 0;
}

