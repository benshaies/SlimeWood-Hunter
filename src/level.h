#ifndef LEVEL_H
#define LEVEL_H
#include <raylib.h>

#define LEVELSIZE 20

extern int level1[20][20];
extern int level2[20][20];
extern int level3[20][20];

void levelInit();
void drawLevel(int currentLevel[20][20]);

#endif