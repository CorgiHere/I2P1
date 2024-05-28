#include <stdbool.h>
#include "Circle.h"

/**
 * @see Shape.cpp
 */
Circle *Circle_Self(Shape *self)
{
    return (Circle *)self->pDerivedObj;
}
Shape *New_Circle(double x, double y, double r)
{
    Circle *pDerivedObj = (Circle *)malloc(sizeof(Circle));
    pDerivedObj->x = x;
    pDerivedObj->y = y;
    pDerivedObj->r = r;
    Shape *pObj = New_Shape();
    pObj->overlap = Circle_overlap;
    pObj->center_x = Circle_center_x;
    pObj->center_y = Circle_center_y;
    pObj->update_center_x = Circle_update_center_x;
    pObj->update_center_y = Circle_update_center_y;\
    pObj->draw_hitbox = Circle_draw_hitbox;
    pObj->getType = Circle_getType;
    pObj->pDerivedObj = pDerivedObj;
    return pObj;
}
double Circle_center_x(Shape *const self)
{
    return Circle_Self(self)->x;
}
double Circle_center_y(Shape *const self)
{
    return Circle_Self(self)->y;
}
void Circle_update_center_x(Shape *const self, int x)
{
    Circle_Self(self)->x += x;
}
void Circle_update_center_y(Shape *const self, int y)
{
    Circle_Self(self)->y += y;
}
void Circle_draw_hitbox(Shape *const self)
{
	al_draw_circle(Circle_Self(self)->x,Circle_Self(self)->y,Circle_Self(self)->r,al_map_rgb(255,255,255),0);
}
ShapeType Circle_getType()
{
    return CIRCLE;
}
