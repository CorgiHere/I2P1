#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
/*
   [ball object]
*/
typedef struct _Ball
{
    int x, y;     
    int r; 
    int in;
    ALLEGRO_COLOR c;
        // the position of image
    Shape *hitbox; // the hitbox of object
} Ball;
Elements *New_Ball(int label);
void Ball_update(Elements *const ele);
void Ball_interact(Elements *const, Elements *const);
void Ball_draw(Elements *const ele);
void Ball_destory(Elements *const ele);

#endif
