#ifndef BULLET_H
#define BULLET_H
#include <raylib.h>

#define MAXBULLETS 50

typedef struct{
    Vector2 pos;
    float radius;
    bool isActive;
    Vector2 direction;
    int speed;
}Bullet;

extern Bullet bullet[MAXBULLETS];

void bulletInit( Vector2 pos, Vector2 direction);

void updateBullet();

void drawBullet();

void resetBullet();


#endif