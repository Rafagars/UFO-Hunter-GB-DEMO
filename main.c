#include <gb/gb.h>
#include <gb/font.h>
#include <stdio.h>
#include "BackgroundTiles.c"
#include "BackgroundMap.c"
#include "windowmap.c"
#include "Plane.c"

void init();

void interruptLCD(){
    HIDE_WIN;
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

    int x = 55;
    int y = 75;

    SPRITES_8x8;

    set_sprite_data(0, 8, Plane);
    set_sprite_tile(0, 0);
    move_sprite(0, x, y);
    set_sprite_tile(1, 1);
    move_sprite(1, x + 8, y);

    init();

    while(1){
        switch (joypad())
        {
        case J_UP:
            y--;
            break;
        case J_RIGHT:
            x++;
            break;
        case J_DOWN:
            y++;
            break;
        case J_LEFT:
            x--;
            break;
        default:
            break;
        }
        move_sprite(0, x, y);
        move_sprite(1, x + 8, y);
        SHOW_WIN;
        scroll_bkg(1, 0);
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