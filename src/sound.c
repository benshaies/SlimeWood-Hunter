#include "sound.h"

Sound shoot;
Sound enemyDead;
Sound click;
Sound gameOver;
Sound death;
Sound powerUpSound;
Sound levelCompleteSound;

Music mainMenuMusic;
Music levelOneMusic;

void loadSounds(){

    InitAudioDevice();
    shoot = LoadSound("../Assets/Shoot.wav");
    enemyDead = LoadSound("../Assets/enemyHit.wav");
    click = LoadSound("../Assets/click.wav");
    gameOver = LoadSound("../Assets/gameOver.mp3");
    death = LoadSound("../Assets/deathSound.wav");
    powerUpSound = LoadSound("../Assets/powerUp.wav");
    levelCompleteSound = LoadSound("../Assets/levelComplete.ogg");

    //Music
    mainMenuMusic = LoadMusicStream("../Assets/mainMenu.mp3");
    levelOneMusic = LoadMusicStream("../Assets/levelOneMusic.ogg");

}

void unloadSounds(){
    //Sound
    UnloadSound(shoot);
    UnloadSound(enemyDead);
    UnloadSound(click);
    UnloadSound(gameOver);
    UnloadSound(death);
    UnloadSound(powerUpSound);
    UnloadSound(levelCompleteSound);

    //Music
    UnloadMusicStream(mainMenuMusic);
    UnloadMusicStream(levelOneMusic);

    CloseAudioDevice();
}