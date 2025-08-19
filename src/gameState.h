#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <raylib.h>

    typedef enum {
        MENU,
        PLAYING,
        PLAYERDIED,
        GAMEOVER,
        CLOSEGAME,
    }gameState;

extern gameState currentState;

#endif