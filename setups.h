#include <gb/gb.h>
#include <stdlib.h>
#include <time.h>
#include "BackgroundTiles.c"
#include "BackgroundMap.c"
#include "BackgroundMap2.c"
#include "BackgroundMap3.c"
#include "windowmap.c"
#include "Plane2.c"
#include "GameCharacter.c"
#include "GameOverBackground.c"
#include "Pause.c"

GameCharacter plane;
GameCharacter ufo;
GameCharacter beam;
GameCharacter asteroid;
UBYTE spritesize = 8;

void interruptLCD();

void turnOnSound();

void turnOffSound();

// Initialize all the default variables needed to start the game
void init();

void performdelay(UINT8 numloops);

UBYTE checkcollision(GameCharacter* one, GameCharacter* two);

void movegamecharacter(GameCharacter* character, UINT8 x, UINT8 y);

void setupplane();

void setupufo(UINT8 x, UINT8 y);

void setupbeam(UINT8 x, UINT8 y);

void setupAsteroid(UINT8 x, UINT8 y);

void setupBackground(UINT8 level);

//Creates a random number between 0 - 30
UINT8 randomize();

//Creates a fade out effect
void fadeout();

//Creates a fade in effect
void fadein();

void pauseScreen();

void gameOverScreen();
