#include "raylib.h"
#include "animation.h"
#include "player.h"
#include "bullet.h"
#include "enemies.h"
#include "level.h" 
#include "gameState.h"
#include "stdio.h"
#include "sound.h"

const int screenWidth = 1025;
const int screenHeight = 1000;

float timer = 0.0f;
float enemyDelay = 1.25f;

float currentLevelTimer = 60.0f;
int currentLevel = 1;

float transitionFadeLevel = 0.0f;
bool fadingOut = true;
bool transitionDone = false;


gameState currentState;

//Main Menu Stuff
Animation mainMenu;
Animation mainMenuEnemy;
Texture2D mainMenuTexture;
Texture2D enemyTexture;
Texture2D playButtonTexture;
Texture2D exitButtonTexture;
Rectangle enemyRect = {0,900,100, 100};
Rectangle playerRect = {-300, 900, 100, 100};
Rectangle playButtonRect = {350, 500, 300, 75};
Rectangle exitButtonRect = {350, 625, 300, 75};
Rectangle playButtonSourceRect = {0,0, 300, 75};
Rectangle exitButtonSourceRect = {0,0,300,75};

//Game Over
Texture2D gameOverTexture;

Texture2D victoryScreenTexture;



bool isHovering(Rectangle rec, Vector2 mousePos){
    if(CheckCollisionPointRec(mousePos, rec)){
        return true;
    }
    else{
        return false;
    }
}

void drawGame(int level){
    BeginDrawing();

    ClearBackground(RAYWHITE);  

    if(level == 1){
        drawLevel(level1);
    }
    else if (level == 2){
        drawLevel(level2);
    }
    else if (level == 3){
        drawLevel(level3);
    }
    

    drawBullet();
    drawPlayer();
    drawEnemy();
    updatePowerUps();

    DrawRectangle(1000, 0, 25, 1000*(currentLevelTimer/60), BLACK);
    
    EndDrawing();
}

void drawMainMenu(){
    BeginDrawing();
    ClearBackground(RAYWHITE);

    //Menu animations
    playAnimation(&mainMenu, (Rectangle){0,0, 1000, 1000}, 1, 0.075);

    if(playerRect.x  < 1000){
        playerRect.x += 10;
    }
    if(playerRect.x  >= 1000){
        playerRect.x = -100;
    }
    playAnimation(&player.playerRun, playerRect, 1, 0.15);
    
    if(enemyRect.x < 1000){
        enemyRect.x += 10;
    }
    if(enemyRect.x >= 1000){
        enemyRect.x = -100;
    }
    playAnimation(&mainMenuEnemy, enemyRect, 1, 0.15);

    DrawTexturePro(playButtonTexture, playButtonSourceRect, playButtonRect, (Vector2){0,0}, 0.0f, WHITE);
    DrawTexturePro(exitButtonTexture, exitButtonSourceRect, exitButtonRect, (Vector2){0,0}, 0.0f, WHITE);
    
    //Button highlighting
    if(isHovering(playButtonRect, GetMousePosition())){
        playButtonSourceRect.x = 300;
    }
    else{
        playButtonSourceRect.x = 0;
    }

    if(isHovering(exitButtonRect, GetMousePosition())){
        exitButtonSourceRect.x = 300;
    }
    else{
        exitButtonSourceRect.x = 0;
    }

    //Button functionality
    if(isHovering(playButtonRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        currentState = PLAYING;
        PlaySound(click);
    }
    if(isHovering(exitButtonRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        currentState = CLOSEGAME;
    }
    

    EndDrawing();
}

void resetGame(bool complete, bool livesReset){
    resetPlayer(livesReset);
    resetEnemy();
    resetBullet();

    if(complete){
        currentLevelTimer = 60.0f;
        resetPowerUp(-1);
    }

    if(complete && livesReset){
        currentLevel = 1;
    }
}

void drawGameOverScreen(){
    BeginDrawing();

    DrawTexture(gameOverTexture, 0, 0, WHITE);

    DrawTexturePro(playButtonTexture, playButtonSourceRect, playButtonRect, (Vector2){0,0}, 0.0f, WHITE);
    DrawTexturePro(exitButtonTexture, exitButtonSourceRect, exitButtonRect, (Vector2){0,0}, 0.0f, WHITE);
    
    //Button highlighting
    if(isHovering(playButtonRect, GetMousePosition())){
        playButtonSourceRect.x = 300;
    }
    else{
        playButtonSourceRect.x = 0;
    }

    if(isHovering(exitButtonRect, GetMousePosition())){
        exitButtonSourceRect.x = 300;
    }
    else{
        exitButtonSourceRect.x = 0;
    }

    //Button functionality
    if(isHovering(playButtonRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        resetGame(true, true);
        currentState = PLAYING;
        PlaySound(click);
    }
    if(isHovering(exitButtonRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        resetGame(true, true);
        currentState = MENU;
        PlaySound(click);
    }

    EndDrawing();


}

void drawVictoryScreen(){
    BeginDrawing();

    DrawTexture(victoryScreenTexture, 0, 0, WHITE);

    DrawTexturePro(playButtonTexture, playButtonSourceRect, playButtonRect, (Vector2){0,0}, 0.0f, WHITE);
    DrawTexturePro(exitButtonTexture, exitButtonSourceRect, exitButtonRect, (Vector2){0,0}, 0.0f, WHITE);
    
    //Button highlighting
    if(isHovering(playButtonRect, GetMousePosition())){
        playButtonSourceRect.x = 300;
    }
    else{
        playButtonSourceRect.x = 0;
    }

    if(isHovering(exitButtonRect, GetMousePosition())){
        exitButtonSourceRect.x = 300;
    }
    else{
        exitButtonSourceRect.x = 0;
    }

    //Button functionality
    if(isHovering(playButtonRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        resetGame(true, true);
        currentState = PLAYING;
        PlaySound(click);
    }
    if(isHovering(exitButtonRect, GetMousePosition()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        resetGame(true, true);
        currentState = MENU;
        PlaySound(click);
    }

    EndDrawing();
}

void levelOneUpdate(){
        SetMusicVolume(levelOneMusic, 0.20);
        UpdateMusicStream(levelOneMusic);

        currentLevelTimer -= GetFrameTime();
        drawGame(1);
        playerMovement();
        updateBullet();

        if(currentLevelTimer > 0){
            if(GetTime() - timer >= enemyDelay){
                if(player.currentPower != 1){
                    enemyInit(1);
                    enemyInit(1);
                    timer = GetTime();
                }
            }
        }
        updateEnemy();
        
        playerCollisions(&currentState);
}

void levelTwoUpdate(){
        SetMusicVolume(levelOneMusic, 0.20);
        UpdateMusicStream(levelOneMusic);

        currentLevelTimer -= GetFrameTime();
        drawGame(2);
        playerMovement();
        updateBullet();

        if(currentLevelTimer > 0){
            if(GetTime() - timer >= enemyDelay){
                if(player.currentPower != 1){
                    enemyInit(2);
                    timer = GetTime();
                }
            }

        }
        updateEnemy();
        
        playerCollisions(&currentState);
}

void levelThreeUpdate(){
        SetMusicVolume(levelOneMusic, 0.20);
        UpdateMusicStream(levelOneMusic);

        currentLevelTimer -= GetFrameTime();
        drawGame(3);
        playerMovement();
        updateBullet();

        if(currentLevelTimer > 0){
            if(GetTime() - timer >= enemyDelay){
                if(player.currentPower != 1){
                    enemyInit(2);
                    enemyInit(1);
                    timer = GetTime();
                }
            }

        }
        updateEnemy();
        
        playerCollisions(&currentState);
}

void update(){
    if(currentState == MENU){
        drawMainMenu();
        SetMusicVolume(mainMenuMusic, 0.5f);
        UpdateMusicStream(mainMenuMusic);
    } 
    else if(currentState == PLAYING){
        if(currentLevelTimer <= 0 && getActiveEnemies() == 0){
            SetSoundVolume(levelCompleteSound, 0.75);
            PlaySound(levelCompleteSound);
            WaitTime(2);
            resetGame(true, false);
            currentLevel++;
            currentState = PLAYING;
        }

        if(currentLevel == 1){
            levelOneUpdate();
        }
        else if(currentLevel == 2){
            levelTwoUpdate();
        }
        else if(currentLevel == 3){
            levelThreeUpdate();
        }
        else if(currentLevel == 4){
            drawVictoryScreen();
        }
        
    }
    else if(currentState == PLAYERDIED){
        
        if(player.lives != 0){
            SetSoundVolume(death, 0.5);
            PlaySound(death);
            WaitTime(1.5);
        }
        
        resetGame(false, false);
        currentState = PLAYING;
    }
    else if(currentState == GAMEOVER){
        drawGameOverScreen();
    }
        
    if(currentLevel == 1){
        enemyDelay = 1.25f;
    }
    else if(currentLevel == 2){
        enemyDelay = 1.0f;
    }
    else if(currentLevel == 3){
        enemyDelay = 1.5f;
    }
}

void init(){
    InitWindow(screenWidth, screenHeight, "Prarie King");  
    SetTargetFPS(60);

    currentState = MENU;

    levelInit();
    playerInit();
    enemyTextureInit();
    loadSounds();

    //Textures
    mainMenuTexture = LoadTexture("../Assets/mainMenu.png");
    enemyTexture = LoadTexture("../Assets/enemyOne.png");
    playButtonTexture = LoadTexture("../Assets/playButton.png");
    exitButtonTexture = LoadTexture("../Assets/exitButton.png");
    gameOverTexture = LoadTexture("../Assets/gameOver.png");
    victoryScreenTexture = LoadTexture("../Assets/victory.png");

    //Animations
    animationInit(&mainMenu, 0, mainMenuTexture, 1000, 9, 0, 0);
    animationInit(&mainMenuEnemy, 0, enemyTexture, 16, 4, 0, 0);

    //Playing Music
    PlayMusicStream(mainMenuMusic);
    PlayMusicStream(levelOneMusic);
        
}

int main(void){

    init();

    timer = GetTime();
    while (!WindowShouldClose()){
        update();

        if(currentState == CLOSEGAME){
            break;
        }
    }
    unloadSounds();
    CloseWindow();
    return 0;
}