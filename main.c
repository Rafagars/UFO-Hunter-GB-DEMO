#include <gb/gb.h>
#include <gb/font.h>
#include <stdio.h>
#include "setups.h"

void main(){

    font_t min_font;

    STAT_REG = 0x45;
    LYC_REG = 0x08; //Fire LCD Interrupt on the 8th scan line (just first row)

    disable_interrupts();

    font_init();
    min_font = font_load(font_min); //36 tile
    font_set(min_font);

    setupBackground();

    set_win_tiles(0, 0, 20, 1, windowmap);
    move_win(7, 0);

    set_sprite_data(0, 8, Plane);
    setupplane();
    setupufo();

    init();

    while(1){
        
        SHOW_WIN;
        
        if(checkcollision(&plane, &ufo)){
            printf("\n \n \n \n \n \n \n \n === GAME OVER ===");
            waitpad(J_A | J_START | J_B);
            setupBackground();
            setupplane();
            setupufo();
        } else {
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

            if(checkcollision(&beam, &ufo)){
                setupExplosion();
            }
            scroll_sprite(4, 1, 0);
            //scroll_bkg(1, 0);
            //performdelay(5);
            wait_vbl_done();
            }
    }
}

