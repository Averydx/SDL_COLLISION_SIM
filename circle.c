//
// Created by Avery Drennan on 4/15/23.
//
#include "circle.h"
#include "engine.h"

void circle_draw(struct engine_struct* engine, struct circle* circle_struct)
        {
            filledCircleRGBA(engine->renderer,circle_struct->pos[0],circle_struct->pos[1],
                             circle_struct->radius,circle_struct->red,circle_struct->green,
                             circle_struct->blue,SDL_ALPHA_OPAQUE);
        }

        int do_circles_overlap(struct circle* circle1, struct circle* circle2)
        {
            double distance = sqrt(pow(circle1->pos[0] - circle2->pos[0],2) +
                    pow(circle1->pos[1] - circle2->pos[1],2));

            if(distance < (circle1->radius + circle2->radius))
            {
                return 1;
            }

            return 0;
        }
