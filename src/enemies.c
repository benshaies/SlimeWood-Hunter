#include "enemies.h"
#include <raymath.h>
#include "player.h"
#include "bullet.h"
#include "sound.h"

Enemy enemies[MAXENEMIES];

Texture2D enemyOneTexture;
Texture2D enemyTwoTexture;

void enemyTextureInit(){
    enemyOneTexture = LoadTexture("../Assets/enemyOne.png");
    enemyTwoTexture = LoadTexture("../Assets/enemyTwo.png");
}

void enemyInit(int type){
    for(int i = 0; i < MAXENEMIES; i++){
        if(!enemies[i].isAlive){
            //Spawn Code
            int spawn = GetRandomValue(1,4);
            switch(spawn){
                case 1:
                    enemies[i].pos = (Vector2){GetRandomValue(450, 550), 0};
                    break;
                case 2:
                    enemies[i].pos = (Vector2){1000, GetRandomValue(450, 550)};
                    break;
                case 3:
                    enemies[i].pos = (Vector2){GetRandomValue(450, 550), 1000};
                    break;
                case 4:
                    enemies[i].pos = (Vector2){0, GetRandomValue(450, 550)};
                    break;
            }

            //Other Variables
            enemies[i].rec = (Rectangle){enemies[i].pos.x, enemies[i].pos.y, 50, 50};
            enemies[i].isAlive = true;
            enemies[i].type = type;
            if(type == 1){
                enemies[i].speed = GetRandomValue(2,4);
                animationInit(&enemies[i].enemyOne, 0, enemyOneTexture, 16, 4, 0, 0);
                enemies[i].health = 1;
            }
            else if(type == 2){
                enemies[i].speed = GetRandomValue(1,2);
                animationInit(&enemies[i].enemyOne, 0, enemyTwoTexture, 16, 4, 0, 0);
                enemies[i].health = 2;
            }

            break;
        }
    }
}



void followTarget(int i, Vector2 target){

    Vector2 direction  = {target.x - enemies[i].pos.x, target.y - enemies[i].pos.y};
    direction  = Vector2Normalize(direction);

    Vector2 movement = {direction.x * enemies[i].speed, direction.y * enemies[i].speed};

    enemies[i].pos.x += movement.x;
    enemies[i].pos.y += movement.y;
}
void updateEnemy(){

    for(int i = 0; i < MAXENEMIES; i++){
        //Check bullet collision
        if(enemies[i].isAlive){
            
            //Call follow function
            followTarget(i, player.pos);

            //Check bullet and enemy collision
            //Kill enemy and bullet if colliding
            for(int num = 0; num < MAXBULLETS; num++){
                if(bullet[num].isActive && CheckCollisionCircleRec(bullet[num].pos, bullet[num].radius, enemies[i].rec)){
                    
                    enemies[i].health--;
                    if(enemies[i].health <= 0){
                        //Create powerup
                        int rand = GetRandomValue(1,20);
                        if(rand == 10){
                            createPowerUp(enemies[i].pos);
                        }

                        enemies[i].isAlive = false;
                        enemies[i].pos = (Vector2){0,0};
                        enemies[i].speed = 0.0f;
                        enemies[i].rec = (Rectangle){0,0,0,0};
                        SetSoundVolume(enemyDead, 3);
                        PlaySound(enemyDead);
                    }
                    

                    
                    
                    bullet[num].isActive = false;
                    bullet[num].radius = 0;
                    bullet[num].direction = (Vector2){0,0};
                    bullet[num].speed = 0;
                    bullet[num].pos = (Vector2){0,0};

                    

                }
            }

            //Updating Rec
            enemies[i].rec.x = enemies[i].pos.x;
            enemies[i].rec.y = enemies[i].pos.y;
            

        }   

    }
}

void drawEnemy(){
    
    for(int i = 0; i < MAXENEMIES; i++){
        if(enemies[i].isAlive){
            playAnimation(&enemies[i].enemyOne, enemies[i].rec, 1, 0.15);
        }
    }
}

void resetEnemy(){
    for(int i = 0; i < MAXENEMIES; i++){
        enemies[i].isAlive = false;
        enemies[i].pos = (Vector2){0,0};
        enemies[i].speed = 0.0f;
        enemies[i].rec = (Rectangle){0,0,0,0};
        enemies[i].health = 0;
    }
}

int getActiveEnemies(){
    int num = 0;

    for(int i = 0; i < MAXENEMIES; i++){
        if(enemies[i].isAlive){
            num++;
        }
    }

    return num;
}