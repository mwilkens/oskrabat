/*
 #
 #  File        : oskrabat.cpp
 #                ( C++ source file )
 #
 #  Description : Oskrabat (Scrape/Flesh) (remake of Dong Dong)
 #
 #  Copyright   : Mandy Wilkens
 #
 #  License     : CeCILL v2.0
 #                ( http://www.cecill.info/licences/Licence_CeCILL_V2-en.html )
 #
 #  This software is governed by the CeCILL  license under French law and
 #  abiding by the rules of distribution of free software.  You can  use,
 #  modify and/ or redistribute the software under the terms of the CeCILL
 #  license as circulated by CEA, CNRS and INRIA at the following URL
 #  "http://www.cecill.info".
 #
 #  As a counterpart to the access to the source code and  rights to copy,
 #  modify and redistribute granted by the license, users are provided only
 #  with a limited warranty  and the software's author,  the holder of the
 #  economic rights,  and the successive licensors  have only  limited
 #  liability.
 #
 #  In this respect, the user's attention is drawn to the risks associated
 #  with loading,  using,  modifying and/or developing or reproducing the
 #  software by the user in light of its specific status of free software,
 #  that may mean  that it is complicated to manipulate,  and  that  also
 #  therefore means  that it is reserved for developers  and  experienced
 #  professionals having in-depth computer knowledge. Users are therefore
 #  encouraged to load and test the software's suitability as regards their
 #  requirements in conditions enabling the security of their systems and/or
 #  data to be ensured and,  more generally, to use and operate it in the
 #  same conditions as regards security.
 #
 #  The fact that you are presently reading this means that you have had
 #  knowledge of the CeCILL license and that you accept its terms.
 #
*/

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>

#include "sprites.h"

#define KEY_SEEN 1
#define KEY_RELEASED 2

void must_init (bool test, const char *description){
    if(test)return;
    printf("Couldn't Initialize %s\n", description);
    exit(1);
}

// Main
// -----
int main(int argc, char **argv) {

    // The dimentions of the screen
    const unsigned int Wi = 800, Hi = 800;
   
    must_init(al_init(),"allegro");
    must_init(al_install_keyboard(),"keyboard");
    must_init(al_init_image_addon(), "image");
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "acodec");
    must_init(al_reserve_samples(6), "samples"); // only 3 audio files
    
    ALLEGRO_TIMER *timer = al_create_timer(1.0/30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(queue,"queue");

    ALLEGRO_SAMPLE * music = NULL;
    ALLEGRO_SAMPLE * bone = NULL;
    ALLEGRO_SAMPLE * win_sound = NULL;

    // Main program window
    ALLEGRO_DISPLAY *main_disp = al_create_display(Wi,Hi);
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    // -----------------------------
    // Loading and defining sprites
    // -----------------------------
    Sprite background("./res/background.png");
    Sprite background2("./res/background2.png");
    Sprite border("./res/border.png");
    Sprite title_text("./res/title.png");

    Character p1("./res/p1up.png", "./res/p1down.png",200); 
    Character p2("./res/p2up.png", "./res/p2down.png",200); 

    Meat meat1("./res/meat.png"); meat1.resize_h(50); 
    Meat meat2("./res/meat.png"); meat2.resize_h(50); 
    Meat meat3("./res/meat.png"); meat3.resize_h(50); 

    Bonewand bw1("./res/bonewand.png",80,1);
    Bonewand bw2("./res/bonewand2.png",80,0); 
    
    unsigned int winner = WINNER_NONE;

    // Configuring events

    bool title = true;
    bool running = true;
    bool playing = true;
    bool redraw = true;

    ALLEGRO_EVENT event;
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key,0,sizeof(key));

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(main_disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // play our audio file
    music = al_load_sample("./res/music.ogg");
    bone  = al_load_sample("./res/hit.ogg");
    win_sound = al_load_sample("./res/final.ogg");
    ALLEGRO_SAMPLE_ID * currSample = NULL;
    al_play_sample(music, 0.8, 0.0,0.8,ALLEGRO_PLAYMODE_LOOP,NULL);

    // ---------------------
    // The main loop
    // ---------------------
    al_start_timer(timer);
    while (running) {

        // Set Defaults
        printf("Setting Defaults...\n");
        background.set_xy(0,10); background2.set_xy(195,40);
        border.set_xy(0,0); border.shift(-5,-5);
        title_text.set_xy(170,240);
        p1.unsquish(); p1.set_xy(210,550);
        p2.unsquish(); p2.set_xy(445,550);
        meat1.set_xy(360,100); meat1.resetStatus();
        meat2.set_xy(360,275); meat2.resetStatus();
        meat3.set_xy(360,450); meat3.resetStatus();
        bw1.set_xy(60,200);
        bw2.set_xy(580,200);
        bw1.set_speed(15);
        bw2.set_speed(15);
        title = true; running = true; playing = true; redraw = true;
        winner = WINNER_NONE;

        while (playing){
        
            al_wait_for_event(queue,&event);

            switch(event.type){
                case ALLEGRO_EVENT_TIMER:

                    // Key events
                    if(key[ALLEGRO_KEY_ESCAPE] || key[ALLEGRO_KEY_Q]){
                        running = false; // quit the game
                        playing = false;
                    }

                    if (title){
                        if(key[ALLEGRO_KEY_SPACE] & KEY_RELEASED == KEY_RELEASED) title = false;
                    } else {
                        if(key[ALLEGRO_KEY_X] & KEY_RELEASED == KEY_RELEASED) {
                            bw1.stab(&meat1,&meat2,&meat3);
                            if (currSample) {al_stop_sample(currSample); printf("is this happening?\n");} 
                            else al_play_sample(bone, 2.0, -0.5,1.0,ALLEGRO_PLAYMODE_ONCE, currSample);
                        }

                        if(key[ALLEGRO_KEY_M] & KEY_RELEASED == KEY_RELEASED) {
                            bw2.stab(&meat1,&meat2,&meat3);
                            if (currSample) al_stop_sample(currSample);
                            else al_play_sample(bone, 2.0, 0.5,1.0,ALLEGRO_PLAYMODE_ONCE, currSample);
                        }

                        if(key[ALLEGRO_KEY_LEFT]) p1.get_squish() ? p1.unsquish() : p1.squish();
                        if(key[ALLEGRO_KEY_RIGHT]) p2.get_squish() ? p2.unsquish() : p2.squish();

                        if((key[ALLEGRO_KEY_SPACE] & KEY_RELEASED == KEY_RELEASED) && winner != WINNER_NONE) { playing = false; } 
                    }

                    if (rand() % 600 == 0) {
                        unsigned int spd = 10 + rand()%10;
                        bw1.set_speed(spd);
                        bw2.set_speed(spd);
                    }

                    for( int i = 0; i < ALLEGRO_KEY_MAX; i++) key[i] &= 0;

                    redraw = true;
                    break;

                // dont worry about what this means lol
                case ALLEGRO_EVENT_KEY_DOWN:
                    key[event.keyboard.keycode] = KEY_SEEN;
                    break;

                case ALLEGRO_EVENT_KEY_UP:
                    key[event.keyboard.keycode] |= KEY_RELEASED;
                    break;

                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    running = false;
                    break;
            }

            if (redraw && al_is_event_queue_empty(queue)){
                // main event loop
                al_clear_to_color(al_map_rgb(0,0,0));

                // shift the eye guy around randomly
                background2.randomshift(1,20);
                border.randomshift(1,2);
                p1.randomshift(1,1); p2.randomshift(1,1);

                // Do background projections
                background2.proj();
                background.proj();
                border.proj();
                p1.proj(); p2.proj();

                if(!title){

                    // this is literally all the game logic lmao

                    meat1.randomshift(1,1); meat2.randomshift(1,1); meat3.randomshift(1,1);
                    
                    // update the movement of the bone wands
                    if      (meat1.get_falling()) meat1.fall(&winner);
                    else if (meat2.get_falling()) meat2.fall(&winner);
                    else if (meat3.get_falling()) meat3.fall(&winner);
                    else {bw1.update(); bw2.update();}

                    switch(winner){
                        case WINNER_P1:
                            p2.squish();
                            al_play_sample(win_sound, 2.0, 0.0,0.5,ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;
                        case WINNER_P2:
                            p1.squish();
                            al_play_sample(win_sound, 2.0, 0.0,0.5,ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;
                        case WINNER_NONE:
                            break;
                    }

                    bw1.proj(); bw2.proj();
                    meat1.proj(); meat2.proj(); meat3.proj();
                } else {
                    title_text.randomshift(1,3);
                    title_text.proj();
                }

                al_flip_display();
                redraw = false;
            }
        }
        // to restart
    }

    al_destroy_display(main_disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_sample(music);

    // End the game
    return 0;
}
