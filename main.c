#include <gb/gb.h>
#include <gb/font.h>
#include <stdio.h>
#include "audio/gbt_player.h"
#include "setups.c"
#include "cover/Cover_data.c"
#include "cover/Cover_map.c"

extern const unsigned char * song_Data[];

void main(){

    UINT8 lives = 3;
    UINT8 level = 1;
    UINT8 numberofUfos = 20;
    UINT8 ufoSpeed = 2;
    UINT8 swap; 

    font_t min_font;

    //Set up the the splashcreen
    set_bkg_data(0, 83, Cover_data);
    set_bkg_tiles(0, 0, 20, 18, Cover_map);

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

    set_sprite_data(0, 10, Plane);
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
            swap = (int) windowmap[19] - 1; //Subtract one to the character in the window layer that represent the number of lives
            windowmap[19] = (char) swap; //Changed it back to a character 
            turnOffSound();
            fadeout();
            if(lives < 1){
                gameOverScreen();
                lives = 3;
                //Changes the characters on the window layer to its default
                windowmap[19] = 0x04; 
                windowmap[4] = 0x03;
                if(level == 1){
                    windowmap[5] = 0x01;
                    numberofUfos = 20;
                } else if (level == 2){
                    windowmap[5] = 0x06;
                    numberofUfos = 25;
                } else {
                    windowmap[5] = 0x0A;
                    numberofUfos = 29;
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
            } else if(joypad() & J_SELECT){
                if (ufoSpeed > 3){
                    ufoSpeed = 2;
                } else {
                    ufoSpeed++;
                }
            }else if(joypad() & J_START){
                pauseScreen();
                setupBackground(level);
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
                
                //Changes the ufos character on the window layer to current one
                if(windowmap[4] == 0x03 && windowmap[5] == 0x01){
                    windowmap[4] = 0x02;
                    windowmap[5] = 0x0A;
                } else if(windowmap[4] == 0x02 && windowmap[5] == 0x01) {
                    windowmap[4] = 0x01;
                    windowmap[5] = 0x0A;
                } else{
                    swap = (int) windowmap[5] - 1;
                    windowmap[5] = (char) swap;
                }

                set_win_tiles(0, 0, 20, 1, windowmap);
            }

            if(numberofUfos == 0 && level < 3){
                level++;
                if(level == 2){
                    windowmap[4] = 0x03;
                    windowmap[5] = 0x06;
                    windowmap[10] = 0x03; //Changes the lvl and ufos characater on the window layer
                    set_win_tiles(0, 0, 20, 1, windowmap);
                    numberofUfos = 25;
                    ufoSpeed = 3;
                } else if(level == 3){
                    windowmap[4] = 0x03;
                    windowmap[5] = 0x09;
                    windowmap[10] = 0x04;
                    set_win_tiles(0, 0, 20, 1, windowmap);
                    numberofUfos = 29;
                }
                fadeout();
                setupBackground(level);
                fadein();
            }

            movegamecharacter(&plane, plane.x, plane.y);
            ufo.x -= ufoSpeed;
            movegamecharacter(&ufo, ufo.x, ufo.y);
            beam.x += 3;
            move_sprite(beam.spriteids[0], beam.x, beam.y);
            scroll_bkg(1, 0);

            if(beam.x > 155){
                setupbeam(0, 0); //That way we avoid that beam keep scrolling after disappearing from the screen
            }

            if(ufo.x < 1){
                setupufo(180, randomize() + 50);
            }

            gbt_update(); //This will change to ROM bank 1

            }
    }
}
