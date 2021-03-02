#include <gb/gb.h>
#include <stdlib.h>
#include <time.h>
#include "BackgroundTiles.c"
#include "BackgroundMap.c"
#include "BackgroundMap2.c"
#include "windowmap.c"
#include "Plane2.c"
#include "GameCharacter.c"


GameCharacter plane;
GameCharacter ufo;
GameCharacter beam;
GameCharacter explosion;
UBYTE spritesize = 8;

void interruptLCD(){
    HIDE_WIN;
}

void turnOnSound(){
    NR52_REG = 0x8F; //Turn on the sound
    NR51_REG = 0x11; //Enable the sound channels
    NR50_REG = 0x77; //Increase the volume to its max()
}

void turnOffSound(){
    NR52_REG = 0x00;
}

// Initialize all the default variables needed to start the game
void init(){

    SHOW_WIN; //Show the window
    SHOW_BKG; //Show the background
    SHOW_SPRITES; //Show the sprites
    DISPLAY_ON; //Turn on the display
    turnOnSound(); 


    add_LCD(interruptLCD);
    enable_interrupts();
    set_interrupts(VBL_IFLAG | LCD_IFLAG);

}

void performdelay(UINT8 numloops){
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }
}

UBYTE checkcollision(GameCharacter* one, GameCharacter* two){
    return(one->x >= two->x && one->x <= two->x + two->width) && (one->y >= two->y && one->y <= two->y + two->height) || (two->x >= one->x && two->x <= one->x + one->width) && (two->y >= one->y && two->y <= one->y + one->height);
}

void movegamecharacter(GameCharacter* character, UINT8 x, UINT8 y){
    move_sprite(character->spriteids[0], x, y);
    move_sprite(character->spriteids[1], x + spritesize, y);
    move_sprite(character->spriteids[2], x, y + spritesize);
    move_sprite(character->spriteids[3], x + spritesize, y + spritesize);
}

void setupplane(){
    plane.x = 50;
    plane.y = 75;
    plane.width = 15;
    plane.height = 11;

    //load sprites for ship
    set_sprite_tile(0, 0);
    plane.spriteids[0] = 0;
    set_sprite_tile(2, 2);
    plane.spriteids[1] = 2;
    set_sprite_tile(1, 1);
    plane.spriteids[2] = 1;
    set_sprite_tile(3, 3);
    plane.spriteids[3] = 3;

    movegamecharacter(&plane, plane.x, plane.y);

}

void setupufo(UINT8 x, UINT8 y){
    ufo.x = x;
    ufo.y = y;
    ufo.width = 16;
    ufo.height = 11;

    //load sprites for ufo
    set_sprite_tile(4, 4);
    ufo.spriteids[0] = 4;
    set_sprite_tile(6, 6);
    ufo.spriteids[1] = 6;
    set_sprite_tile(5, 5);
    ufo.spriteids[2] = 5;
    set_sprite_tile(7, 7);
    ufo.spriteids[3] = 7;


    movegamecharacter(&ufo, ufo.x, ufo.y);
}

void setupbeam(UINT8 x, UINT8 y){
    beam.x = x;
    beam.y = y;
    beam.width = 6;
    beam.height = 4;

    //load sprites for beam
    set_sprite_tile(8, 8);
    beam.spriteids[0] = 8;

    move_sprite(beam.spriteids[0], beam.x, beam.y);
}

void setupExplosion(UINT8 x, UINT8 y){
    explosion.x = x;
    explosion.y = y;
    explosion.width = 8;
    explosion.height = 8;

    //load sprites for explosion
    set_sprite_tile(5, 5);
    explosion.spriteids[0] = 5;
    set_sprite_tile(6, 6);
    explosion.spriteids[1] = 6;

    movegamecharacter(&explosion, explosion.x, explosion.y);
}

void setupBackground(UINT8 level){
    set_bkg_data(37, 48, BackgroundTiles);
    if(level == 1){
        set_bkg_tiles(0, 0, BackgroundMapWidth, BackgroundMapHeight, BackgroundMap);
    } else if(level == 2){
        set_bkg_tiles(0, 0, BackgroundMap2Width, BackgroundMap2Height, BackgroundMap2);
    }
}

//Creates a random number between 0 - 30
UINT8 randomize(){
    return rand() % 30;
}

UINT8 i;
//Creates a fade out effect
void fadeout(){
    for(i = 0; i < 4; i++){
        switch (i)
        {
        case 0:
            BGP_REG = 0xE4;
            break;
        case 1:
            BGP_REG = 0xF9;
            break;
        case 2: 
            BGP_REG = 0xFE;
            break;
        case 3:
            BGP_REG = 0xFF;
            break;
        }
        performdelay(10);
    }
}
//Creates a fade in effect
void fadein(){
    for(i = 0; i < 3; i++){
        switch (i)
        {
        case 0:
            BGP_REG = 0xFE;
            break;
        case 1:
            BGP_REG = 0xF9;
            break;
        case 2:
            BGP_REG = 0xE4;
            break;
        }
        performdelay(10);
    }
}
