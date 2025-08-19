#ifndef SOUND_H
#define SOUND_H
#include <raylib.h>

extern Sound shoot;
extern Sound enemyDead;
extern Sound click;
extern Sound gameOver;
extern Sound death;
extern Sound powerUpSound;
extern Sound levelCompleteSound;

extern Music mainMenuMusic;
extern Music levelOneMusic;


void loadSounds();

void unloadSounds();

#endif