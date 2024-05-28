#ifndef Bullet_H_INCLUDED
#define Bullet_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
/*
   [Bullet object]
*/
typedef struct _Bullet
{
    double x, y, r;  
    int height,width;
    float vx,vy;
    int damage;
    int exist;
    int timer;
    double rd;
    ALLEGRO_BITMAP *img;
        // the position of image
    Shape *hitbox; // the hitbox of object
} Bullet;
Elements *New_Bullet(int label,double x,double y,double radius, int velocity,int damage);
void Bullet_update(Elements *const ele);
void Bullet_interact(Elements *const, Elements *const);
void Bullet_draw(Elements *const ele);
void Bullet_destory(Elements *const ele);

#endif
