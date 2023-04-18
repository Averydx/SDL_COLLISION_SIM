//
// Created by Avery Drennan on 4/13/23.
//

#ifndef PHYSENGINE_SDL_ENGINE_SDL_H
#define PHYSENGINE_SDL_ENGINE_SDL_H

#include <SDL.h>
#include "circle.h"

struct event_data
{
    int mouse_pos[2];
    int quit;
    int left_click;
    int right_click;
};


//initializes the SDL and returns a pointer to the main window
struct engine_struct* engine_init(char* title ,int w,int h);

//Destroys the main window and closes SDL
void engine_quit(struct engine_struct* engine);

//handles events(place in main loop)
void event_handler(SDL_Event event,struct engine_struct* engine);

//renders objects to screen
void rendering(struct engine_struct* engine);

//performs all requested draw calls
void draw(struct engine_struct* engine);

//finds the current distance between the cursor and the circle
int distance_cursor_circle(struct engine_struct* engine, struct circle* circle_struct);


//moves balls to prevent any overlap
void balls_overlapping_update(struct engine_struct* engine, struct circle* current,struct circle* target);

//performs any necessary updates each frame
void update(struct engine_struct* engine);

//checks if the pair_array contains the pair
int does_pair_array_contain(struct engine_struct* engine,struct circle* current, struct circle* target);

//integrates equations of motion for the circle
void integrate_circle(struct circle* current);

//checks circle for collision with wall
void circle_collision_wall(struct circle* current);

//dynamic circle collision
void dynamic_circle_collision(struct collisionPair* collision_pair);


#endif //PHYSENGINE_SDL_ENGINE_SDL_H
