#include "playerManager.h"

/*
   [player_Manager function]
*/


Elements *New_player_Manager(int label)
{   
    Elements *Obj = New_Elements(label);
    Obj->inter_obj[Obj->inter_len++] = Player_L;
    Obj->Draw = playerManager_draw;
    Obj->Update = playerManager_update;
    Obj->Interact = playerManager_interact;
    Obj->Destroy = playerManager_destory;
    return Obj;
}
void playerManager_update(Elements *const ele)
{    
    
}
void playerManager_draw(Elements *const ele)
{
}
void playerManager_destory(Elements *const ele)
{
  
}
/*-------------------------------------------------------
    bulletDamage,
    bulletRecovery,
    hpRecovery,
    mpRecovery,
    hpMax,
    mpMax,
    expMax*/

void playerManager_interact(Elements *const self, Elements *const target) {
    int skill_data[10][8]={ 
        {5,4,3,3,2,2,2,2},//spGain
        {1,2,3,4,5,6,7,9},//moveSpeed
        {2,3,4,5,6,7,8,10},//bulletSpeed
        {30,40,50,60,70,80,90,100},//bulletDamage
        {300,230,170,120,80,50,25,10},//bulletReload
        {5,10,15,20,25,30,35,40},//hpRecovery
        {5,6,7,8,9,10,11,12},//mpRecovery
        {300,400,500,600,700,800,900,1000},//hpMax
        {150,170,190,210,230,250,270,290},//mpMax
        {300,600,1000,1400,1800,2300,2800,3200}//expMax
    };
    if(target->label == Player_L){
        Player *pl=((Player*)(target->pDerivedObj));
        if(pl->update_change){
            pl->move_speed=skill_data[moveSpeed][pl->skill_level[moveSpeed]];
            pl->bullet_speed=skill_data[bulletSpeed][pl->skill_level[bulletSpeed]];
            pl->bullet_damage=skill_data[bulletDamage][pl->skill_level[bulletDamage]];
            pl->bullet_reload=skill_data[bulletReload][pl->skill_level[bulletReload]];
            pl->hp_recovery=skill_data[hpRecovery][pl->skill_level[hpRecovery]];
            pl->mp_recovery=skill_data[mpRecovery][pl->skill_level[mpRecovery]];
            pl->hp_max=skill_data[hpMax][pl->skill_level[hpMax]];
            pl->mp_max=skill_data[mpMax][pl->skill_level[mpMax]];
            pl->update_change=false;
        }
        if(pl->skill_level[expMax]<10 && pl->exp > skill_data[expMax][pl->skill_level[expMax]]){
            pl->sp+=skill_data[spGain][pl->skill_level[spGain]];
            pl->skill_level[expMax]+=1;
            pl->skill_level[spGain]+=1;
        }
    }    
}


