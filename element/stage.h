#ifndef Stage_H_INCLUDED
#define Stage_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
#include "monster.h"
/*
   [Stage object]
*/

typedef struct _Stage
{     int stage; 
      int exp;
      int money;
} Stage;
Elements *New_Stage(int label);
void Stage_update(Elements *const ele);
void Stage_interact(Elements *const, Elements *const);
void Stage_draw(Elements *const ele);
void Stage_destory(Elements *const ele);

#endif
