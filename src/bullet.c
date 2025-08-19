#include "bullet.h"

Bullet bullet[MAXBULLETS];

void bulletInit(Vector2 pos, Vector2 direction){
    for(int i = 0; i < MAXBULLETS; i++){
        if(!bullet[i].isActive){
            bullet[i].pos = pos;
            bullet[i].radius = 5;
            bullet[i].isActive = true;
            bullet[i].direction = direction;
            bullet[i].speed = 12.5;
            break;
        }
    }
}

void updateBullet(){
    for(int i = 0; i < MAXBULLETS; i++){
        if(bullet[i].isActive){
            bullet[i].pos.x += bullet[i].direction.x * bullet[i].speed;
            bullet[i].pos.y += bullet[i].direction.y * bullet[i].speed;

            //Out of bounds Check
            if(bullet[i].pos.x  > 1000 || bullet[i].pos.x < 0 || bullet[i].pos.y < 0 || bullet[i].pos.y > 1000){
                bullet[i].radius = 0;
                bullet[i].isActive = false;
                bullet[i].pos = (Vector2){-100, -100};
                bullet[i].direction = (Vector2){0,0};
            }
        }
    }
}

void drawBullet(){
    for(int i = 0; i < MAXBULLETS; i++){
        if(bullet[i].isActive){
            DrawCircleV(bullet[i].pos, bullet[i].radius, BLACK);
        }
    }
}

void resetBullet(){
    for(int num = 0; num < MAXBULLETS; num++){
        bullet[num].isActive = false;
        bullet[num].radius = 0;
        bullet[num].direction = (Vector2){0,0};
        bullet[num].speed = 0;
        bullet[num].pos = (Vector2){0,0};
    }
}