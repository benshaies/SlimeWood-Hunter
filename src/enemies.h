#ifndef ENEMIES_H
#define ENEMIES_H
#include <raylib.h>
#include "animation.h"

#define MAXENEMIES 50

    typedef struct{
        Vector2 pos;
        Rectangle rec;
        bool isAlive;
        float speed;
        Animation enemyOne;
        int health;
        int type;
    }Enemy;

    extern Enemy enemies[MAXENEMIES];

    void enemyInit(int type);

    void enemyTextureInit();

    void updateEnemy();

    void followTarget(int i, Vector2 target);

    void drawEnemy();

    void resetEnemy();

    int getActiveEnemies();

#endif