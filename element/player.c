#include "player.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../shapes/Circle.h"
#include "../algif5/src/algif.h"
#include "bullet.h"
#include "monster.h"
#include <allegro5/allegro_primitives.h>
#include<math.h>
#include<stdio.h>
#include<string.h>
/*
   [Player function]
*/
Elements *New_Player(int label)
{  
    
    // setting derived object member
    Player *pDerivedObj = (Player *)malloc(sizeof(Player));
    Elements *pObj = New_Elements(label);
    
    // load Player images
    pDerivedObj->img=al_load_bitmap("assets/image/player.png");
    pDerivedObj->r=al_get_bitmap_width(pDerivedObj->img)/2;
    
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/atk_sound.wav");
    pDerivedObj->atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_Sound, al_get_default_mixer());
    // load font
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 15, 0);
    // initial the geometric information of Player
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);    
    pDerivedObj->x = 300, pDerivedObj->y = 300;
    pDerivedObj->hitbox = New_Circle( pDerivedObj->x, pDerivedObj->y, pDerivedObj->width/2);

    memset(pDerivedObj->skill_level, 0, sizeof(pDerivedObj->skill_level));
    strcpy(pDerivedObj->name,"Wayne");

    // initialise properties
    pDerivedObj->level=1;
    pDerivedObj->bullet_mp_consumption=5;
    pDerivedObj->hp=300;
    pDerivedObj->mp=150;
    pDerivedObj->sp_recover_time=100;
    pDerivedObj->recover_time=100;
    pDerivedObj->exp=0;
    pDerivedObj->sp=40;

    // initialise timers
    pDerivedObj->timer_for_mphp=0;
    pDerivedObj->timer_for_bullet=0;
    pDerivedObj->anime_time=0;
    pDerivedObj->total_timer=10000;

    pDerivedObj->show_information=false;
    pDerivedObj->show_information_permanent=true;
    pDerivedObj->update_change=false;
    
    // initial the animation component
    pObj->inter_obj[pObj->inter_len++] = Monster_L;
    pDerivedObj->wlk_state = p_STOP;
    pDerivedObj->atk_state = p_CEASE_FIRE;
    pDerivedObj->new_shot = 0;
    pObj->pDerivedObj = pDerivedObj;
    
    // setting derived object function
    pObj->Draw = Player_draw;
    pObj->Update = Player_update;
    pObj->Interact = Player_interact;
    pObj->Destroy = Player_destory;
    return pObj;
}

/*
    moveSpeed,
    bulletSpeed,
    bulletDamage,
    bulletReload,
    hpRecovery,
    mpRecovery,
    hpMax,
    mpMax,
*/
void _Player_limit_timer(Elements *const ele){
    Player* chara=((Player*)(ele->pDerivedObj));
    chara->timer_for_bullet%=chara->total_timer;
    chara->timer_for_mphp%=chara->total_timer;
}

void _Player_mphp_recover(Elements *const ele)
{
     Player* chara=((Player*)(ele->pDerivedObj));
     if(chara->timer_for_mphp>=chara->recover_time){
        if(chara->mp<chara->mp_max){
           chara->mp+=chara->mp_recovery;
           if(chara->wlk_state == p_STOP && chara->atk_state == p_CEASE_FIRE) chara->mp+=10;
           chara->mp=(chara->mp>chara->mp_max)?chara->mp_max:chara->mp;
        }
        if(chara->hp<chara->hp_max){
           chara->hp+=chara->hp_recovery;
           if(chara->wlk_state == p_STOP && chara->atk_state == p_CEASE_FIRE) chara->hp+=5;
           chara->hp=(chara->hp > chara->hp_max)?chara->hp_max:chara->hp;
        }
        chara->timer_for_mphp%=chara->recover_time;
     }
}
void _Player_sp_update(Elements *const ele){
    Player* chara=((Player*)(ele->pDerivedObj));
    if(chara->sp > 0 && chara->anime_time == 0){
        int f = 1;
        chara->update_change=true;
        for(int i = 0; i < 8; i++)
            if(key_state[ALLEGRO_KEY_1 + i] && chara->skill_level[i+1] < 8)
                chara->skill_level[i+1]++, chara->sp--, chara->anime_time = 60,f=0;
        if(!f) chara->update_change=false;
    }
}
void Player_update(Elements *const ele)
{    
    // use the idea of finite state machine to deal with different state
    Player *chara = ((Player *)(ele->pDerivedObj));    
    if(chara->anime_time) chara->anime_time-=3;
    chara->timer_for_bullet+=5, chara->timer_for_mphp+=5, chara->exp+=50;
    double origin_x=chara->x,origin_y=chara->y;
    if (chara->atk_state == p_FIRE && chara->timer_for_bullet >= chara->bullet_reload && chara->mp >= chara->bullet_damage){
        double mx=mouse.x,my=mouse.y;
        double r = 0.075;
        double angle = atan2(my-chara->y,mx-chara->x) + (double)(rand() % 200 - 100) / 50 * r;
        Elements *bullet;
        bullet=New_Bullet(Bullet_L,chara->x+chara->r*cos(angle),chara->y+chara->r*sin(angle),angle,chara->bullet_speed,chara->bullet_damage);
        _Player_update_position(ele, -chara->bullet_speed * cos(angle) , -chara->bullet_speed  * sin(angle));
        _Register_elements(scene,bullet);
        chara->timer_for_bullet%=chara->bullet_reload;
        chara->mp-=chara->bullet_mp_consumption;
    }
    
    // position update correct
    if (key_state[ALLEGRO_KEY_A]) _Player_update_position(ele, -chara->move_speed, 0);
    if (key_state[ALLEGRO_KEY_D]) _Player_update_position(ele, chara->move_speed, 0);
    if (key_state[ALLEGRO_KEY_W]) _Player_update_position(ele, 0, -chara->move_speed);
    if (key_state[ALLEGRO_KEY_S]) _Player_update_position(ele, 0, chara->move_speed);
    if (chara->x - origin_x != 0 && chara->y - origin_y != 0) _Player_update_position(ele, (int)(origin_x-chara->x)/2, (int)(origin_y-chara->y)/2);
    
    chara->angle=atan2(mouse.y-chara->y,mouse.x-chara->x);
    if (key_state[ALLEGRO_KEY_SPACE] || mouse_state[1]) chara->atk_state = p_FIRE;
    else chara->atk_state = p_CEASE_FIRE; 
    
    // determine whther to open skill board
    if(key_state[ALLEGRO_KEY_TILDE] || key_state[ALLEGRO_KEY_TAB] || key_state[ALLEGRO_KEY_LCTRL] || mouse_state[2])
        chara->show_information=true;
    else
        chara->show_information=false;

    if(key_state[ALLEGRO_KEY_O] && chara->anime_time==0){
        chara->show_information_permanent = chara->show_information_permanent ? false: true;
        chara->anime_time = 60;
    }

    _Player_sp_update(ele);
    _Player_mphp_recover(ele);
    _Player_limit_timer(ele);
}

char skill[10][20] = {
    "moveSpeed",
    "bulletSpeed",
    "bulletDamage",
    "bulletReload",
    "hpRecovery",
    "mpRecovery",
    "hpMax",
    "mpMax"
};
int hardColor[8][3] = 
    {{223, 122, 122},
    {223, 161, 122},
    {203, 223, 122},
    {122, 153, 223},
    {122, 205, 223},
    {122, 153, 223},
    {223, 122, 207},
    {223, 122, 177}};
int baseColor[3] = {172, 231, 232};

void Player_draw(Elements *const ele)
    {
        int x_init = 20, y_init = HEIGHT-170;
        // with the state, draw corresponding image
        Player *Obj = ((Player *)(ele->pDerivedObj));
        int w = al_get_text_width(Obj->font, Obj->name)/ 2+5;
        al_draw_rotated_bitmap(Obj->img,Obj->width/2,Obj->height/2,Obj->x,Obj->y,Obj->angle+2.355,0);
        al_draw_text(Obj->font, al_map_rgb(255,255,255),Obj->x, Obj->y-Obj->height/2, ALLEGRO_ALIGN_CENTRE, Obj->name);
        char tmp[50];
        if(Obj->show_information || Obj->show_information_permanent){
            al_draw_filled_rectangle(x_init-5, y_init-5, x_init+18*7+16+5, y_init + 22*7+15+5, al_map_rgba(0,0,0,80));
            al_draw_filled_rectangle(x_init-5, y_init-5+300, x_init+18*7+16+5, y_init + 22*7+15+5+300, al_map_rgba(0,0,0,80));
            for(int i = 0; i < 8; i++){
                sprintf(tmp, "%s: %d", skill[i], Obj->skill_level[i+1]+1); 
                al_draw_text(Obj->font, al_map_rgb(255,255,255), 0, 25*i, ALLEGRO_ALIGN_LEFT, tmp);
                for(int j = 0; j < 8; j++){
                    al_draw_filled_rectangle(x_init+18*i, y_init+22*j, x_init+18*i+16, y_init + 22*j+15, 
                    i <= Obj -> skill_level[j+1] ? al_map_rgb(hardColor[j][0],hardColor[j][1],hardColor[j][2]) : 
                    al_map_rgb(baseColor[0],baseColor[1],baseColor[2]));
                }
            }
            sprintf(tmp,"mp:%d",Obj->mp);
            al_draw_text(Obj->font,al_map_rgb(255,255,255),0,25*8,ALLEGRO_ALIGN_LEFT,tmp);
            sprintf(tmp,"hp:%d",Obj->hp);
            al_draw_text(Obj->font,al_map_rgb(255,255,255),0,25*9,ALLEGRO_ALIGN_LEFT,tmp);
        }
        Obj->hitbox->draw_hitbox(Obj->hitbox);
    }
void Player_destory(Elements *const ele)
{
    Player *Obj = ((Player *)(ele->pDerivedObj));
    al_destroy_sample_instance(Obj->atk_Sound);
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(ele);
}

void _Player_update_position(Elements *const ele, double dx, double dy)
{
    dx = (int) dx, dy = (int)dy;
    Player *chara = ((Player *)(ele->pDerivedObj));
    chara->x += dx, chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

void Player_interact(Elements *const self, Elements *const target) {
    Player *pl=((Player*)(self->pDerivedObj));
    if(target->label == Monster_L){
        Monster *mon=((Monster*)(target->pDerivedObj));
        if(mon->hitbox->overlap(mon->hitbox,pl->hitbox) && mon->atk_timer>=mon->atk_frequency){
            if(pl->hp>mon->damage) pl->hp-=mon->damage;
            else self->dele=true;
            mon->atk_timer%=mon->atk_frequency;
            pl->timer_for_mphp=0;
        }
    }
}


