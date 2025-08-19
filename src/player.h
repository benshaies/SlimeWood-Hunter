#ifndef PLAYER_H
#define PLAYER_H
#include <raylib.h>
#include "animation.h"
#include "gameState.h"

#define FIRERATE 0.5
#define SPEED 5.0


    typedef struct{
        Vector2 pos;
        Rectangle rec;
        int dir;
        Vector2 direction;
        bool isMoving;
        float fireRate;
        Animation playerIdle;
        Animation playerRun;
        float speed;
        int lives;
        int currentPower;
        float powerTimer;
    }Player;

    typedef struct{
        Rectangle rec;
        int num;
        bool isActive;
        float timer;
    }powerUp;


    extern Player player;
    extern powerUp power[5];

    void playerInit();

    void playerMovement();

    void playerCollisions(gameState *currentState);

    void drawPlayer();

    void resetPlayer(bool completeReset);

    void createPowerUp(Vector2 pos);

    void resetPowerUp(int num);

    void updatePowerUps();


#endif