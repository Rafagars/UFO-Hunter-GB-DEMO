#include <gb/gb.h>
#include <gb/font.h>
#include <stdio.h>
#include "setups.h"
#include "GameOverBackground.c"

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

    UINT8 lives = 3;
    UINT8 newlife; 

    while(lives > 0){
        
        SHOW_WIN;

        if(checkcollision(&plane, &ufo)){
            lives--;
            // Since windowmap is a array of characters, we transform the value we want to change to a integer
            newlife = (int) windowmap[19] - 1; //Subtract one to the character in the window layer that represent the number of lives
            windowmap[19] = (char) newlife; //Changed it back to a character 

            /*if(lives < 1){
                printf("\n \n \n \n \n \n \n \n === GAME OVER ===");
                lives = 3; 
                windowmap[19] = 0x04; 
            } */

            waitpad(J_A | J_START | J_B); // Press any of this buttons to continue
            set_win_tiles(0, 0, 20, 1, windowmap);
            setupBackground(); //Restart background
            //Restart sprites
            setupplane();
            setupufo();
        } else {
            switch (joypad())
            {
            case J_UP:
                if(plane.y > 30){
                    plane.y--;
                }
                break;
            case J_RIGHT:
                if(plane.x < 155){
                    plane.x++;
                }
                break;
            case J_DOWN:
                if(plane.y < 120){
                    plane.y++;
                }
                break;
            case J_LEFT:
                if(plane.x > 5){
                    plane.x--;
                }
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
            scroll_bkg(1, 0);
            //performdelay(5);
            wait_vbl_done();
            }
    }
    move_bkg(0, 0); //Move the background to its default position
    HIDE_SPRITES;
    set_bkg_data(37, 20, BackgroundTiles);
    set_bkg_tiles(0, 0, GameOverWidth, GameOverHeight, GameOver);
}

