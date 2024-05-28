#include"Bullet.h"
#include "../shapes/Circle.h"
#include"../global.h"
#include "../shapes/Shape.h"
#include <allegro5/allegro_primitives.h>
#include<math.h>
#include"Monster.h"
const int scaled=30;
Elements *New_Bullet(int label,double x,double y,double radius, int velocity,int damage){
    Bullet *pDerivedObj = (Bullet *)malloc(sizeof(Bullet));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    //printf("velocity:%d",velocity);
    pDerivedObj->x = x;
    pDerivedObj->y = y;
    pDerivedObj->vx=(int)(velocity*cos(radius));
    pDerivedObj->vy=(int)(velocity*sin(radius));
    pDerivedObj->rd=radius;
    pDerivedObj->damage=damage;
    pDerivedObj->exist=700;
    pDerivedObj->timer=0;
    pDerivedObj->img = al_load_bitmap("assets/image/bullet/b5.png");
    pDerivedObj->width=al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height=al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->r=(scaled)/2;
    pDerivedObj->hitbox = New_Circle(pDerivedObj->x,
                                     pDerivedObj->y,
                                     pDerivedObj->r);                     
    pObj->inter_obj[pObj->inter_len++] = Monster_L;
    pObj->inter_obj[pObj->inter_len++] = Player_L;
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Bullet_update;
    pObj->Interact = Bullet_interact;
    pObj->Draw = Bullet_draw;
    pObj->Destroy = Bullet_destory;
    return pObj;
    
}
void Bullet_update(Elements *const self)
{  
    Bullet *Obj = ((Bullet*)(self->pDerivedObj));
    //printf("vx:%d vy:%d\n",Obj->vx,Obj->vy);
    Obj->x += Obj->vx;
    Obj->y += Obj->vy;
    Obj->timer+=5;
    if(Obj->timer>=Obj->exist){
        //printf("-1\n");
        self->dele=true;
        Obj->timer%=Obj->exist;
    }
    Shape *hitbox = Obj->hitbox;
    hitbox->update_center_x(hitbox, Obj->vx);
    hitbox->update_center_y(hitbox, Obj->vy);
}
void Bullet_interact(Elements *const self, Elements *const ele){
    Bullet *Obj = ((Bullet *)(self->pDerivedObj)); 
    if(ele->label == Monster_L){
        Monster *mon=((Monster*)(ele->pDerivedObj));
        if(mon->hitbox->overlap(Obj->hitbox,mon->hitbox)){
            self->dele=true;
        }
    }
    else if(ele->label == Player_L){
        if (Obj->x < 0 - Obj->width)
            self->dele = true;
        else if (Obj->x > WIDTH + Obj->width)
            self->dele = true;
    }
   // printf("-2\n");   
}
void Bullet_draw(Elements *const ele){
    Bullet *Obj = ((Bullet *)(ele->pDerivedObj));
    Obj->hitbox->draw_hitbox(Obj->hitbox);
    al_draw_scaled_rotated_bitmap(Obj->img,Obj->width/2,Obj->height/2,Obj->x,Obj->y,
    scaled/(double)Obj->width,scaled/(double)Obj->height,Obj->rd,0);
}
void Bullet_destory(Elements *const ele){
    Bullet *Obj = ((Bullet *)(ele->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(ele);
}