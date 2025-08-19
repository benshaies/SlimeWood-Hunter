#include "player.h"
#include "math.h"
#include "bullet.h"
#include "level.h"
#include "enemies.h"
#include "sound.h"
#include <stdio.h>

float timers = 0.0f;
Player player;

powerUp power[5];

Texture2D playerIdleTexture, playerRunTexture;
Texture2D livesTexture;
Texture2D timePowerUpTexture, fireRatePowerUpTexture, speedPowerUpTexture;

char livesChar[1];

Rectangle playerHitBox;

void playerInit(){
    playerIdleTexture = LoadTexture("../Assets/playerIdle.png");
    playerRunTexture = LoadTexture("../Assets/playerRun.png");
    livesTexture = LoadTexture("../Assets/lives.png");

    timePowerUpTexture = LoadTexture("../Assets/timePowerUpFinal.png");
    fireRatePowerUpTexture = LoadTexture("../Assets/fireRatePowerUp.png");
    speedPowerUpTexture = LoadTexture("../Assets/speedPowerUp.png");

    player.pos = (Vector2){500, 500};
    player.rec = (Rectangle){player.pos.x, player.pos.y, 65, 65};
    player.dir = 1;
    player.direction = (Vector2){0,0};
    player.fireRate = 0.5f;
    player.speed = 5.0f;
    player.lives = 3;

    player.currentPower = 0;
    player.powerTimer = 0;

    animationInit(&player.playerIdle, 0,playerIdleTexture, 16, 4, 0, 0);
    animationInit(&player.playerRun, 0, playerRunTexture, 16, 4, 0, 0);
    
    timers = GetTime();

}

void playerMovement(){

    //Movement Code
    player.rec.x = player.pos.x;
    player.rec.y = player.pos.y;

    if(IsKeyDown(KEY_D)){
        player.direction.x += 1;
        player.dir = 1;
    }
    if(IsKeyDown(KEY_A)){
        player.direction.x -= 1;
        player.dir = -1;
    }

    if(IsKeyDown(KEY_W)){
        player.direction.y -= 1;
    }
    if(IsKeyDown(KEY_S)){
        player.direction.y += 1;
    }

    float lenght = sqrtf(player.direction.x * player.direction.x + player.direction.y * player.direction.y);
    if(lenght != 0){
        player.direction.x/=lenght;
        player.direction.y/=lenght;
        player.isMoving = true;
    }
    else{
        player.isMoving = false;
    }
        
    player.pos.x += player.direction.x * player.speed;
    player.pos.y += player.direction.y * player.speed;

    player.direction.x = 0;
    player.direction.y = 0;

    //Shooting code
    if(GetTime() - timers >= player.fireRate){

        Vector2 direction = {0,0};
        if(IsKeyDown(KEY_RIGHT)){
            direction = (Vector2){1,0};
        }
        else if(IsKeyDown(KEY_LEFT)){
            direction = (Vector2){-1,0};
        }
        else if(IsKeyDown(KEY_UP)){
            direction = (Vector2){0,-1};
        }
        else if(IsKeyDown(KEY_DOWN)){
            direction = (Vector2){0,1};
        }
        if(IsKeyDown(KEY_UP)&&IsKeyDown(KEY_RIGHT)){
            direction = (Vector2){1,-1};
        }
        else if(IsKeyDown(KEY_DOWN)&&IsKeyDown(KEY_RIGHT)){
            direction = (Vector2){1,1};
        }
        else if(IsKeyDown(KEY_DOWN)&&IsKeyDown(KEY_LEFT)){
            direction = (Vector2){-1,1};
        }
        else if(IsKeyDown(KEY_UP)&&IsKeyDown(KEY_LEFT)){
            direction = (Vector2){-1,-1};
        }

        if(direction.x != 0 || direction.y != 0){
            bulletInit((Vector2){player.pos.x+32.5, player.pos.y+40}, direction);
            SetSoundVolume(shoot, 2);
            PlaySound(shoot);
        }

        

        timers = GetTime();
    }

}

void playerCollisions(gameState *currentState){
    
    Vector2 topPoint = {player.pos.x+player.rec.width/2, player.pos.y};
    Vector2 rightPoint = {player.pos.x+player.rec.width, player.pos.y+player.rec.height/2};
    Vector2 bottomPoint = {player.pos.x+player.rec.width/2, player.pos.y+player.rec.height};
    Vector2 leftPoint = {player.pos.x, player.pos.y+player.rec.height/2};
            
    //Level Collision Check
    for(int y = 0; y < LEVELSIZE; y++){
        for(int x = 0; x < LEVELSIZE; x++){

            if(level1[y][x] == 1){
                if(CheckCollisionPointRec(topPoint, (Rectangle){x*50, y*50, 50, 50})){
                    player.pos.y = (y*50) + 50;
                }
                else if(CheckCollisionPointRec(rightPoint, (Rectangle){x*50, y*50, 50, 50})){
                    player.pos.x = (x*50) - 65;
                }
                else if(CheckCollisionPointRec(bottomPoint, (Rectangle){x*50, y*50, 50, 50})){
                    player.pos.y = (y*50) - 65;
                }
                else if(CheckCollisionPointRec(leftPoint, (Rectangle){x*50, y*50, 50, 50})){
                    player.pos.x = (x*50) + 50;
                }
            }
    
        }
    }
    if(player.pos.x < 0){
        player.pos.x = 0;
    }
    else if(player.pos.x > 1000-65){
        player.pos.x = 1000-65;
    }
    else if(player.pos.y < 0){
        player.pos.y = 0;
    }
    else if(player.pos.y > 1000 - 65){
        player.pos.y = 1000-65;
    }


    //Enemy Collision Check
    for(int i = 0; i < MAXENEMIES; i++){
        if(enemies[i].isAlive){
            playerHitBox = (Rectangle){player.rec.x+15, player.rec.y+15, player.rec.width-25, player.rec.height-25};
            if(CheckCollisionRecs(playerHitBox, enemies[i].rec)){
                *currentState = PLAYERDIED;
                resetPowerUp(-1);
                player.lives -= 1;
                return;
            }
        }
    }

    if(player.lives <= 0){
        PlaySound(gameOver);
        *currentState = GAMEOVER;
    }

    //Power ups
    for(int i = 0; i < 5; i++){
        if(power[i].isActive){
            if(CheckCollisionRecs(playerHitBox, power[i].rec) && player.currentPower == 0){
                SetSoundVolume(powerUpSound, 2);
                PlaySound(powerUpSound);
                player.currentPower = power[i].num;
                if(power[i].num == 1){
                    player.powerTimer = 5.0;
                }
                else{
                    player.powerTimer = 15.0f;
                }
                resetPowerUp(i);
                break;
            }
        }
    }

    if(player.currentPower == 1){
        for(int i = 0; i < MAXENEMIES; i++){
            if(enemies[i].isAlive){
                enemies[i].speed = 0.0f;
            }
        }
    }
    else if(player.currentPower == 2){
        player.fireRate = 0.20f;
    }
    else if(player.currentPower == 3){
        player.speed = 6.5f;
    }

    if(player.currentPower != 0){
        player.powerTimer -= GetFrameTime();
        if(player.powerTimer <= 0){
            player.currentPower = 0;
            player.fireRate = FIRERATE;
            player.speed = SPEED;

            for(int i = 0; i < MAXENEMIES; i++){
                if(enemies[i].isAlive){
                    if(enemies[i].type == 1){
                        enemies[i].speed = GetRandomValue(2,4);   
                    }
                    else{
                        enemies[i].speed = GetRandomValue(1,2);   
                    }
                      
                }
            }

        }
    }

    if(IsKeyPressed(KEY_SPACE)){
        player.currentPower = 0;
        player.fireRate = FIRERATE;
        player.speed = SPEED;

        for(int i = 0; i < MAXENEMIES; i++){
                if(enemies[i].type == 1){
                    enemies[i].speed = GetRandomValue(2,4);   
                }
                else{
                    enemies[i].speed = GetRandomValue(1,2);   
                }
        }
    }
    
}

void drawPlayer(){

    if(!player.isMoving){
        playAnimation(&player.playerIdle, player.rec, player.dir, 0.15);
    }
    else{
        playAnimation(&player.playerRun, player.rec, player.dir, 0.15);
    }

    DrawTexturePro(livesTexture, (Rectangle){0,0, 16,16}, (Rectangle){10,10,40,40}, (Vector2){0,0}, 0.0f, WHITE);
    sprintf(livesChar, "%d", player.lives);
    DrawText(livesChar, 60, 10, 50, WHITE);

    //Draw powerups
    for(int i = 0; i < 5; i++){
        if(power[i].isActive){
            if(power[i].num == 1){
                DrawTexturePro(timePowerUpTexture, (Rectangle){0,0,16,16}, power[i].rec, (Vector2){0,0}, 0, WHITE);
            }
            else if(power[i].num == 2){
                DrawTexturePro(fireRatePowerUpTexture, (Rectangle){0,0,16,16}, power[i].rec, (Vector2){0,0}, 0, WHITE);
            }
            else if(power[i].num == 3){
                DrawTexturePro(speedPowerUpTexture, (Rectangle){0,0,16,16}, power[i].rec, (Vector2){0,0}, 0, WHITE);
            }
        }
    }
}

void resetPlayer(bool completeReset){
    player.pos = (Vector2){500, 500};
    player.rec = (Rectangle){player.pos.x, player.pos.y, 65, 65};
    player.dir = 1;
    player.direction = (Vector2){0,0};
    player.fireRate = 0.5f;
    player.speed = 5.0f;
    player.currentPower = 0;
    player.powerTimer = 0;


    if(completeReset){
        player.lives = 3;
    }

}

void createPowerUp(Vector2 pos){
    for(int i  = 0; i < 5; i++){
        if(!power[i].isActive){
            power[i].isActive = true;
            power[i].timer = 10.0f;
            power[i].rec = (Rectangle){pos.x,pos.y, 50, 50};
            power[i].num = GetRandomValue(1,3);
            break;
        }
    }
}

void resetPowerUp(int num){
    if(num == -1){
        for(int i = 0; i < 5; i++){
            power[i].rec = (Rectangle){0,0,0,0};
            power[i].num = 0;
            power[i].isActive = false;
            power[i].timer = 0.0f;    
        }
    }
    else{
        power[num].rec = (Rectangle){0,0,0,0};
        power[num].num = 0;
        power[num].isActive = false;
        power[num].timer = 0.0f;  
    }
    
}

void updatePowerUps(){

    for(int i = 0; i < 5; i++){
        if(power[i].isActive){
            power[i].timer -= GetFrameTime();
        }

        if(power[i].timer <= 0.0f){
            resetPowerUp(i);
            break;
        }
    }
}

