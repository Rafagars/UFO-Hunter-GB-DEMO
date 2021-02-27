#include <gb/gb.h>
#include <gb/font.h>
#include <stdio.h>
#include "setups.h"
#include "GameOverBackground.c"

void main(){

    UINT8 lives = 3;
    UINT8 swap; 

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
    setupufo(randomize() + 75, randomize() + 50);

    init();

    while(1){
        
        SHOW_WIN;

        if(checkcollision(&plane, &ufo)){
            lives--;
            // Since windowmap is a array of characters, we transform the value we want to change to a integer
            swap = (int) windowmap[19] - 1; //Subtract one to the character in the window layer that represent the number of lives
            windowmap[19] = (char) swap; //Changed it back to a character 

            if(lives < 1){
                move_bkg(0, 0); //Move the background to its default position
                HIDE_SPRITES;
                set_bkg_data(37, 20, BackgroundTiles);
                set_bkg_tiles(0, 0, GameOverWidth, GameOverHeight, GameOver);
                lives = 3; 
                windowmap[19] = 0x04; 
                windowmap[8] = 0x01;
                windowmap[7] = 0x01;
                windowmap[6] = 0x01;
            }

            waitpad(J_A | J_START | J_B); // Press any of this buttons to continue
            set_win_tiles(0, 0, 20, 1, windowmap);
            setupBackground(); //Restart background
            //Restart sprites
            SHOW_SPRITES;
            setupplane();
            setupufo(randomize(), randomize() + 60);
        } else {
            switch (joypad())
            {
            case J_UP:
                if(plane.y > 25){
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
                setupbeam(plane.x + 10, plane.y);
                break;
            default:
                break;
            }
            if(checkcollision(&beam, &ufo)){
                setupufo(randomize(), randomize() + 50);
                setupbeam(0, 0);
                
                if(windowmap[8] == 0x0A){
                    swap = (int) windowmap[7] + 1;
                    windowmap[8] = 0x01;
                    windowmap[7] = (char) swap;
                } else if(windowmap[7] == 0x0A){
                    swap = (int) windowmap[6] + 1;
                    windowmap[7] = 0x01;
                    windowmap[6] = (char) swap;
                }else{
                    swap = (int) windowmap[8] + 1;
                    windowmap[8] = (char) swap;
                }
                set_win_tiles(0, 0, 20, 1, windowmap);
            }

            movegamecharacter(&plane, plane.x, plane.y);
            ufo.x -= 2;
            movegamecharacter(&ufo, ufo.x, ufo.y);
            
            beam.x += 3;
            move_sprite(beam.spriteids[0], beam.x, beam.y);
            scroll_bkg(1, 0);

            if(beam.x > 155){
                setupbeam(0, 0); //That way we avoid that beam keep scrolling after disappearing from the screen
            }

            wait_vbl_done();
            }
    }
}

