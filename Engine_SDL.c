//
// Created by Avery Drennan on 4/13/23.

//
#include "Engine_SDL.h"
#include <SDL.h>
#include "engine.h"
#include "SDL2_gfxPrimitives.h"
#include "circle.h"
#include <math.h>
#include "collisionPair.h"

#define elapsed_time 0.1

typedef struct engine_struct engine_struct;
typedef struct collisionPair collisionPair;

engine_struct* engine_init(char* title, int w, int h) {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Error: %s\n",SDL_GetError());
    }

    engine_struct* engine = (void*)malloc(sizeof(engine_struct));
    SDL_Window* engine_window = SDL_CreateWindow
            (title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w,h,SDL_WINDOW_SHOWN);

    if(!engine_window)
    {
        printf("Error: %s\n",SDL_GetError());
    }

    engine->window = engine_window;

    SDL_Renderer* engine_renderer = SDL_CreateRenderer
            (engine_window,-1,SDL_RENDERER_ACCELERATED);
    if(!engine_renderer)
    {
        printf("Error: %s\n",SDL_GetError());
    }
    engine->renderer = engine_renderer;
    return engine;
}

void engine_quit(engine_struct* engine) {
    SDL_DestroyWindow(engine->window);
    SDL_DestroyRenderer(engine->renderer);
    SDL_Quit();

}

void event_handler(SDL_Event event,struct engine_struct* engine)
{

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            engine->engine_event_data->quit = 1;

        }


        else if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            if(SDL_BUTTON_LEFT == event.button.button) {
                engine->engine_event_data->left_click = 1;
            }

            if(SDL_BUTTON_RIGHT == event.button.button) {

                engine->engine_event_data->right_click = 1;
            }

        }

        else if(event.type == SDL_MOUSEBUTTONUP)
        {
            if(event.button.button == SDL_BUTTON_LEFT)
             {
                engine->engine_event_data->left_click = 0;
                engine->selected_circle = NULL;
            }

            if(event.button.button == SDL_BUTTON_RIGHT && engine->selected_circle != NULL)
            {
                engine->engine_event_data->right_click = 0;

                engine->selected_circle->vel[0] = 5.f * (engine->selected_circle->pos[0] - engine->engine_event_data->mouse_pos[0]);
                engine->selected_circle->vel[1] = 5.f * (engine->selected_circle->pos[1] - engine->engine_event_data->mouse_pos[1]);

                printf("(%f,%f)",engine->selected_circle->vel[0],engine->selected_circle->vel[1]);

                engine->selected_circle = NULL;
            }
        }

        else if(event.type == SDL_MOUSEMOTION && engine->engine_event_data->left_click == 1)
        {
            int x,y;
            SDL_GetMouseState(&x,&y);
            engine->engine_event_data->mouse_pos[0] = x;
            engine->engine_event_data->mouse_pos[1] = y;
            for(int i = 0;i < sizeof(engine->circle_array)/sizeof(struct circle*);i++)
            {
                if(engine->circle_array[i] != NULL)
                {
                    if (distance_cursor_circle(engine,engine->circle_array[i]) < engine->circle_array[i]->radius)
                    {
                        engine->circle_array[i]->pos[0] = engine->engine_event_data->mouse_pos[0];
                        engine->circle_array[i]->pos[1] = engine->engine_event_data->mouse_pos[1];
                        engine->selected_circle=engine->circle_array[i];
                        engine->selected_circle->vel[0] = 0;
                        engine->selected_circle->vel[1] = 0;
                        printf("%p\n",engine->selected_circle);

                    }

                }

            }
        }

        else if(event.type == SDL_MOUSEMOTION && engine->engine_event_data->right_click == 1)
        {
            int x,y;
            SDL_GetMouseState(&x,&y);
            engine->engine_event_data->mouse_pos[0] = x;
            engine->engine_event_data->mouse_pos[1] = y;
            if(engine->selected_circle != NULL)
            {
                break;
            }
            for (int i = 0; i < sizeof(engine->circle_array) / sizeof(struct circle *); i++) {
                    if (engine->circle_array[i] != NULL) {
                        if (distance_cursor_circle(engine, engine->circle_array[i]) < engine->circle_array[i]->radius) {
                            engine->selected_circle = engine->circle_array[i];
                        }

                    }

                }
        }
    }
}

void rendering(engine_struct* engine)
{
    SDL_SetRenderDrawColor(engine->renderer,0xFF,0xFF,0xFF,SDL_ALPHA_OPAQUE);
    SDL_RenderSetViewport(engine->renderer,NULL);
    SDL_RenderClear(engine->renderer);

    if(engine->engine_event_data->right_click == 1 && engine->selected_circle != NULL)
    {
        SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(engine->renderer, engine->engine_event_data->mouse_pos[0],
                           engine->engine_event_data->mouse_pos[1], engine->selected_circle->pos[0],
                           engine->selected_circle->pos[1]);
    }


    draw(engine);

    SDL_RenderPresent(engine->renderer);

}

void balls_overlapping_update(engine_struct* engine,struct circle* current,struct circle* target)
{
     if(current != target)
     {
         int overlapping = do_circles_overlap(current,target);
         if(overlapping == 1)
         {
             int contains = does_pair_array_contain(engine,current,target);
             if(contains == 0) {
                 struct collisionPair *pair = (void *) malloc(sizeof(collisionPair));

                 pair->circle1 = current;
                 pair->circle2 = target;

                 engine->pair_array[engine->pair_count] = pair;
                 engine->pair_count++;

             }
             double distance = sqrt(pow(current->pos[0] - target->pos[0],2) +
                                    pow(current->pos[1] - target->pos[1],2));

             double overlap = (distance -(current->radius + target->radius))/2;


             current->pos[0] -= overlap * ((current->pos[0] -
                                                            target->pos[0]) / distance);

             current->pos[1] -= overlap * ((current->pos[1] -
                                                            target->pos[1]) / distance);

             target->pos[0] += overlap * ((current->pos[0] -
                                                            target->pos[0]) / distance);

             target->pos[1] += overlap * ((current->pos[1] -
                                                            target->pos[1]) / distance);

         }
     }

}

void update(struct engine_struct* engine){

        for(int i = 0; i<sizeof(engine->circle_array)/sizeof(struct circle*);i++)
        {
            integrate_circle(engine->circle_array[i]);
            circle_collision_wall(engine->circle_array[i]);
            for(int j = 0; j<sizeof(engine->circle_array)/sizeof(struct circle*);j++) {
                balls_overlapping_update(engine,engine->circle_array[i],engine->circle_array[j]);

            }
        }

}

void draw(struct engine_struct *engine)
{
    for(int i = 0; i < sizeof(engine->circle_array)/sizeof(struct circle*);i++)
    {
        if(engine->circle_array[i] != NULL) {
            circle_draw(engine, engine->circle_array[i]);
        }
    }

}

int distance_cursor_circle(struct engine_struct *engine, struct circle *circle_struct)
        {
    return sqrt(pow((engine->engine_event_data->mouse_pos[0] - circle_struct->pos[0]),2)+
    pow(engine->engine_event_data->mouse_pos[1] - circle_struct->pos[1],2));
}

int does_pair_array_contain(struct engine_struct* engine,struct circle *current, struct circle *target) {
    for(int i = 0;i<engine->pair_count;i++)
    {
        if((engine->pair_array[i]->circle1 == current && engine->pair_array[i]->circle2 == target)||
        (engine->pair_array[i]->circle1 == target && engine->pair_array[i]->circle2 == current))
        {
            return 1;
        }

    }

    return 0;
}

void integrate_circle(struct circle *current){

    current->acc[0] = -0.1f * current->vel[0];
    current->acc[1] = -0.1f * current->vel[1];

    current->vel[0] += current->acc[0] * elapsed_time;
    current->vel[1] += current->acc[1] * elapsed_time;


    if(sqrt(pow(current->vel[0],2) + pow(current->vel[1],2)) < 0.5f)
    {
        current->vel[0] = 0;
        current->vel[1] = 0;
    }


    current->pos[0] += current->vel[0] * elapsed_time;
    current->pos[1] += current->vel[1] * elapsed_time;




}

void circle_collision_wall(struct circle* current)
{
    if(current->pos[0] < 0)
    {
        current->pos[0] = 0;
        current->vel[0] *= -1;
    }

    if(current->pos[0] > 800)
    {
        current->pos[0] = 800;
        current->vel[0] *= -1;
    }


    if(current->pos[1] < 0)
    {
        current->pos[1] = 0;
        current->vel[1] *= -1;
    }

    else if(current->pos[1] > 600)
    {
        current->pos[1] = 600;
        current->vel[1] *= -1;
    }
}

void dynamic_circle_collision(struct collisionPair *collision_pair){

    //distance between the two circles
    double distance = sqrt(pow(collision_pair->circle1->pos[0] - collision_pair->circle2->pos[0],2)
            + pow(collision_pair->circle1->pos[1] - collision_pair->circle2->pos[1],2));

    //normal
    double nx = (collision_pair->circle2->pos[0] - collision_pair->circle1->pos[0]) / distance;
    double ny = (collision_pair->circle2->pos[1] - collision_pair->circle1->pos[1]) / distance;

    //tangent
    double tx = -ny;
    double ty = nx;

    //tangent dot product
    double circle1_tandp = (collision_pair->circle1->vel[0] * tx) + (collision_pair->circle1->vel[1] * ty);
    double circle2_tandp = (collision_pair->circle2->vel[0] * tx) + (collision_pair->circle2->vel[1] * ty);

    //normal dot product
    double circle1_normdp = (collision_pair->circle1->vel[0] * nx) + (collision_pair->circle1->vel[1] * ny);
    double circle2_normdp = (collision_pair->circle2->vel[0] * nx) + (collision_pair->circle2->vel[1] * ny);



    //conservation of momentum in 1-D

    double m1 = (circle1_normdp*(collision_pair->circle1->mass - collision_pair->circle2->mass) +2.0f *
            collision_pair->circle2->mass * circle2_normdp) / (collision_pair->circle1->mass +
                    collision_pair->circle2->mass);

    double m2 = (circle2_normdp*(collision_pair->circle2->mass - collision_pair->circle1->mass) +2.0f *
            collision_pair->circle1->mass * circle1_normdp) / (collision_pair->circle1->mass +
                    collision_pair->circle2->mass);

    //assign response
    collision_pair->circle1->vel[0] = tx * circle1_tandp + nx * m1;
    collision_pair->circle1->vel[1] = ty * circle1_tandp + ny * m1;

    collision_pair->circle2->vel[0] = tx * circle2_tandp + nx * m2;
    collision_pair->circle2->vel[1] = ty * circle2_tandp + ny * m2;

}
