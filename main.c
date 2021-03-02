#include <gb/gb.h>
#include <gb/font.h>
#include <stdio.h>
#include "gbt_player.h"
#include "setups.h"
#include "GameOverBackground.c"
#include "PortadaGB_data.c"
#include "PortadaGB_map.c"

extern const unsigned char * song_Data[];

void main(){

    UINT8 lives = 3;
    UINT8 level = 1;
    UINT8 numberofUfos = 10;
    UINT8 swap; 

    font_t min_font;

    //Set up the the splashcreen
    set_bkg_data(0, 83, PortadaGB_data);
    set_bkg_tiles(0, 0, 20, 18, PortadaGB_map);

    SHOW_BKG;
    DISPLAY_ON;

    waitpad(J_START);

    fadeout();

    fadein();

    STAT_REG = 0x45;
    LYC_REG = 0x08; //Fire LCD Interrupt on the 8th scan line (just first row)

    disable_interrupts();

    //Start background music
    gbt_play(song_Data, 2, 7);
    gbt_loop(1);

    //Set fonts
    font_init();
    min_font = font_load(font_min); //36 tile
    font_set(min_font);

    setupBackground(level);

    set_win_tiles(0, 0, 20, 1, windowmap);
    move_win(7, 0);

    set_sprite_data(0, 8, Plane);
    setupplane();
    setupufo(180, randomize() + 50);
    
    // Initialize all the default variables needed to start the game
    init();

    while(1){
        wait_vbl_done();
        
        SHOW_WIN;

        if(checkcollision(&plane, &ufo)){
            
            //Collision sound
            NR10_REG = 0x1E;
            NR11_REG = 0x00;
            NR12_REG = 0xF3;
            NR13_REG = 0x00;
            NR14_REG = 0x86;

            lives--;
            // Since windowmap is a array of characters, we transform the value we want to change to a integer
            swap = (int) windowmap[18] - 1; //Subtract one to the character in the window layer that represent the number of lives
            windowmap[18] = (char) swap; //Changed it back to a character 
            turnOffSound();
            fadeout();
            if(lives < 1){
                move_bkg(0, 0); //Move the background to its default position
                HIDE_SPRITES;
                fadein();
                set_bkg_data(37, 20, BackgroundTiles);
                set_bkg_tiles(0, 0, GameOverWidth, GameOverHeight, GameOver);
                lives = 3; 
                windowmap[18] = 0x04; 
                windowmap[6] = 0x02;
                if(level == 1){
                    windowmap[5] = 0x01;
                    numberofUfos = 10;
                } else {
                    windowmap[5] = 0x06;
                    numberofUfos = 15;
                }
            }
            waitpad(J_A | J_START | J_B); // Press any of this buttons to continue
            fadein();
            turnOnSound();
            set_win_tiles(0, 0, 20, 1, windowmap);
            setupBackground(level); //Restart background
            //Restart sprites
            SHOW_SPRITES;
            setupplane();
            setupufo(180, randomize() + 50);
        } else {

            if((joypad() & J_UP) && (plane.y > 25)){
                plane.y--;
            } else if((joypad() & J_LEFT) && (plane.x > 5)){
                plane.x--;
            } else if((joypad() & J_DOWN) && (plane.y < 120)){
                plane.y++;
            } else if((joypad() & J_RIGHT) && plane.x < 155){
                plane.x++;
            } else if(joypad() & J_A){
                setupbeam(plane.x + 10, plane.y);
                //Beam sound
                NR10_REG = 0x1E;
                NR11_REG = 0x10;
                NR12_REG = 0xF3;
                NR13_REG = 0x00;
                NR14_REG = 0x86;
            } 

            if(checkcollision(&beam, &ufo)){
                //Ufo collision sound
                NR10_REG = 0x1E;
                NR11_REG = 0x00;
                NR12_REG = 0xF3;
                NR13_REG = 0x10;
                NR14_REG = 0x87;

                numberofUfos--;

                setupufo(180, randomize() + 50);
                setupbeam(0, 0);
                
                if(windowmap[5] == 0x02 && windowmap[6] == 0x01){
                    windowmap[5] = 0x01;
                    windowmap[6] = 0x0A;
                } else {
                    swap = (int) windowmap[6] - 1;
                    windowmap[6] = (char) swap;
                }

                set_win_tiles(0, 0, 20, 1, windowmap);
            }

            if(numberofUfos == 0 && level < 2){
                level++;
                if(level == 2){
                    windowmap[5] = 0x02;
                    windowmap[6] = 0x06;
                    set_win_tiles(0, 0, 20, 1, windowmap);
                    numberofUfos = 15;
                }
                fadeout();
                setupBackground(level);
                fadein();
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

            gbt_update(); //This will change to ROM bank 1

            }
    }
}
