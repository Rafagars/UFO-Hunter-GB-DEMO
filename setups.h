#ifndef _SETUPS_H
#define _SETUPS_H

#include <gb/gb.h>
#include <stdlib.h>
#include <time.h>
#include "backgrounds/BackgroundTiles.c"
#include "backgrounds/BackgroundMap.c"
#include "backgrounds/BackgroundMap2.c"
#include "backgrounds/BackgroundMap3.c"
#include "windowmap.c"
#include "sprites/Plane.c"
#include "GameCharacter.c"
#include "backgrounds/GameOverBackground.c"
#include "backgrounds/Pause.c"

GameCharacter plane;
GameCharacter ufo;
GameCharacter beam;
GameCharacter asteroid;
extern UBYTE spritesize;

extern void interruptLCD();

extern void turnOnSound();

extern void turnOffSound();

// Initialize all the default variables needed to start the game
extern void init();

extern void performdelay(UINT8 numloops);

extern UBYTE checkcollision(GameCharacter* one, GameCharacter* two);

extern void movegamecharacter(GameCharacter* character, UINT8 x, UINT8 y);

extern void setupplane();

extern void setupufo(UINT8 x, UINT8 y);

extern void setupbeam(UINT8 x, UINT8 y);

extern void setupAsteroid(UINT8 x, UINT8 y);

extern void setupBackground(UINT8 level);

//Creates a random number between 0 - 30
extern UINT8 randomize();

//Creates a fade out effect
extern void fadeout();

//Creates a fade in effect
extern void fadein();

extern void pauseScreen();

extern void gameOverScreen();

#endif