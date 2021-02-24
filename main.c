#include <gb/gb.h>
#include <gb/font.h>
#include <stdio.h>
#include "BackgroundTiles.c"
#include "BackgroundMap.c"
#include "windowmap.c"
#include "Plane.c"
#include "GameCharacter.c"

struct GameCharacter plane;
struct GameCharacter ufo;
struct GameCharacter beam;
UBYTE spritesize = 8;

void init();

void interruptLCD(){
    HIDE_WIN;
}

void performdelay(UINT8 numloops){
    UINT8 i;
    for(i = 0; i < numloops; i++){
        wait_vbl_done();
    }
}

void movegamecharacter(struct GameCharacter* character, UINT8 x, UINT8 y){
    move_sprite(character->spriteids[0], x, y);
    move_sprite(character->spriteids[1], x + spritesize, y);
}

void setupplane(){
    plane.x = 55;
    plane.y = 75;
    plane.width = 16;
    plane.height = 8;

    //load sprites for ship
    set_sprite_tile(0, 0);
    plane.spriteids[0] = 0;
    set_sprite_tile(1, 1);
    plane.spriteids[1] = 1;

    movegamecharacter(&plane, plane.x, plane.y);

}

void setupufo(){
    ufo.x = 165;
    ufo.y = 65;
    ufo.width = 16;
    ufo.height = 8;

    //load sprites for ufo
    set_sprite_tile(2, 2);
    ufo.spriteids[0] = 2;
    set_sprite_tile(3, 3);
    ufo.spriteids[1] = 3;

    movegamecharacter(&ufo, ufo.x, ufo.y);
}

void setupbeam(){
    beam.x = plane.x + 12;
    beam.y = plane.y;
    ufo.width = 8;
    ufo.height = 8;

    //load sprites for beam
    set_sprite_tile(4, 4);
    beam.spriteids[0] = 4;

    move_sprite(beam.spriteids[0], beam.x, beam.y);
}

void main(){

    font_t min_font;

    STAT_REG = 0x45;
    LYC_REG = 0x08; //Fire LCD Interrupt on the 8th scan line (just first row)

    disable_interrupts();

    font_init();
    min_font = font_load(font_min); //36 tile
    font_set(min_font);

    set_bkg_data(37, 20, BackgroundTiles);
    set_bkg_tiles(0, 0, BackgroundMapWidth, BackgroundMapHeight, BackgroundMap);

    set_win_tiles(0, 0, 20, 1, windowmap);
    move_win(7, 0);

    set_sprite_data(0, 8, Plane);
    setupplane();
    setupufo();

    init();

    while(1){
        switch (joypad())
        {
        case J_UP:
            plane.y--;
            break;
        case J_RIGHT:
            plane.x++;
            break;
        case J_DOWN:
            plane.y++;
            break;
        case J_LEFT:
            plane.x--;
            break;
        case J_A:
            setupbeam();
            break;
        default:
            break;
        }
        movegamecharacter(&plane, plane.x, plane.y);
        ufo.x--;
        movegamecharacter(&ufo, ufo.x, ufo.y);
        SHOW_WIN;
        scroll_sprite(4, 1, 0);
        scroll_bkg(1, 0);
        //performdelay(5);
        wait_vbl_done();
    }
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