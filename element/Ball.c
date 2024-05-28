#include"Ball.h"
#include"../global.h"
#include"../shapes/Circle.h"
#include "../shapes/Shape.h"
#include "player.h"
#include "monster.h"
#include <allegro5/allegro_primitives.h>
Elements *New_Ball(int label){
    Ball *pDerivedObj = (Ball *)malloc(sizeof(Ball));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->x = mouse.x;
    pDerivedObj->y = mouse.y;
    pDerivedObj->r = 10;
    pDerivedObj->c = al_map_rgb(255,0,0);
    pDerivedObj->hitbox = New_Circle(pDerivedObj->x,
                                     pDerivedObj->y,
                                     pDerivedObj->r);
    // setting the interact object
    pObj->inter_obj[pObj->inter_len++] = Player_L;
    pObj->inter_obj[pObj->inter_len++] = Monster_L;
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Ball_update;
    pObj->Interact = Ball_interact;
    pObj->Draw = Ball_draw;
    pObj->Destroy = Ball_destory;
    return pObj;
    
}
void Ball_update(Elements *const ele){
    Ball *Obj = ((Ball *)(ele->pDerivedObj));
    Shape *hitbox=Obj->hitbox;
    hitbox->update_center_x(hitbox,mouse.x-Obj->x);
    hitbox->update_center_y(hitbox,mouse.y-Obj->y);
    Obj->x=mouse.x;
    Obj->y=mouse.y;
}
void Ball_interact(Elements *const self, Elements *const ele){

    Ball *Obj = ((Ball *)(self->pDerivedObj));
   // printf("-2\n");
    if (ele->label == Player_L)
    {   
        Player* chara=((Player*)(ele->pDerivedObj));
        if(chara->hitbox->overlap(chara->hitbox,Obj->hitbox)){
            Obj->c=al_map_rgb(255,0,0);
            Obj->in=Player_L;
        }
        else if(Obj->in == Player_L){
           // printf("-4\n");
            Obj->c=al_map_rgb(255,0,0);
        }
    }
    else if (ele->label == Monster_L)
     {
         Monster * tree=((Monster*)(ele->pDerivedObj));
         if(tree->hitbox->overlap(tree->hitbox,Obj->hitbox)){
             Obj->c=al_map_rgb(0,255,0);
             Obj->in=Monster_L;
         }
         else if(Obj->in == Monster_L){
            Obj->c=al_map_rgb(0,255,0);
         }
    }
}
void Ball_draw(Elements *const ele){
    Ball *Obj = ((Ball *)(ele->pDerivedObj));
    al_draw_circle(Obj->x, Obj->y,Obj->r,Obj->c,5);
}
void Ball_destory(Elements *const ele){
    Ball *Obj = ((Ball *)(ele->pDerivedObj));
    free(Obj->hitbox);
    free(Obj);
    free(ele);
}