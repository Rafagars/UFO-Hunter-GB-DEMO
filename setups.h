#include <gb/gb.h>
#include <stdlib.h>
#include <time.h>
#include "BackgroundTiles.c"
#include "BackgroundMap.c"
#include "windowmap.c"
#include "Plane.c"
#include "GameCharacter.c"

GameCharacter plane;
GameCharacter ufo;
GameCharacter beam;
GameCharacter explosion;
UBYTE spritesize = 8;

void interruptLCD(){
    HIDE_WIN;
}

void init(){

    SHOW_WIN; //Show the window
    SHOW_BKG; //Show the background
    SHOW_SPRITES; //Show the sprites
    DISPLAY_ON; //Turn on the display
    NR52_REG = 0x8F; //Turn on the sound
    NR51_REG = 0x11; //Enable the sound channels
    NR50_REG = 0x77; //Increase the volume to its max()

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
}

void setupplane(){
    plane.x = 50;
    plane.y = 75;
    plane.width = 13;
    plane.height = 5;

    //load sprites for ship
    set_sprite_tile(0, 0);
    plane.spriteids[0] = 0;
    set_sprite_tile(1, 1);
    plane.spriteids[1] = 1;

    movegamecharacter(&plane, plane.x, plane.y);

}

void setupufo(UINT8 x, UINT8 y){
    ufo.x = x;
    ufo.y = y;
    ufo.width = 15;
    ufo.height = 5;

    //load sprites for ufo
    set_sprite_tile(2, 2);
    ufo.spriteids[0] = 2;
    set_sprite_tile(3, 3);
    ufo.spriteids[1] = 3;

    movegamecharacter(&ufo, ufo.x, ufo.y);
}

void setupbeam(UINT8 x, UINT8 y){
    beam.x = x;
    beam.y = y;
    beam.width = 8;
    beam.height = 4;

    //load sprites for beam
    set_sprite_tile(4, 4);
    beam.spriteids[0] = 4;

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

void setupBackground(){
    set_bkg_data(37, 20, BackgroundTiles);
    set_bkg_tiles(0, 0, BackgroundMapWidth, BackgroundMapHeight, BackgroundMap);
}

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