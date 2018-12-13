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
#include <stdio.h>

#include "sprites.h"

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
    
    ALLEGRO_TIMER *timer = al_create_timer(1.0/60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(queue,"queue");

    // Main program window
    ALLEGRO_DISPLAY *main_disp = al_create_display(Wi,Hi);
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

    // -----------------------------
    // Loading and defining sprites
    // -----------------------------
    Sprite background("./res/background.png");
    Sprite background2("./res/background2.png");
   
    background.set_xy(0,0); background2.set_xy(0,0);

    Character p1("./res/p1up.png", "./res/p1down.png",200); p1.set_xy(210,550);
    Character p2("./res/p2up.png", "./res/p2down.png",200); p2.set_xy(445,550);

    Meat meat1("./res/meat.png"); meat1.resize_h(50); meat1.set_xy(350,75);
    Meat meat2("./res/meat.png"); meat2.resize_h(50); meat2.set_xy(350,275);
    Meat meat3("./res/meat.png"); meat3.resize_h(50); meat3.set_xy(350,475);

    Bonewand bw1("./res/bonewand.png",100,1); bw1.set_xy(10,200);
    Bonewand bw2("./res/bonewand2.png",100,0); bw2.set_xy(590,200);
    
    bw1.set_speed(5);
    bw2.set_speed(5);

    // Configuring events

    bool running = true;
    bool redraw = true;

    ALLEGRO_EVENT event;
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key,0,sizeof(key));

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(main_disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    // ---------------------
    // The main loop
    // ---------------------
    al_start_timer(timer);
    while (running){
    
        al_wait_for_event(queue,&event);

        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                if(key[ALLEGRO_KEY_ESCAPE] || key[ALLEGRO_KEY_Q]) running = false; // quit the game
                if(key[ALLEGRO_KEY_X]) bw1.stab();
                if(key[ALLEGRO_KEY_M]) bw2.stab();
                if(key[ALLEGRO_KEY_LEFT]) p1.get_squish() ? p1.unsquish() : p1.squish();
                if(key[ALLEGRO_KEY_RIGHT]) p2.get_squish() ? p2.unsquish() : p2.squish();

                for( int i = 0; i < ALLEGRO_KEY_MAX; i++) key[i] &= 1;

                redraw = true;
                break;
            // dont worry about what this means lol
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = 1|2;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= 2;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
        }

        if (redraw && al_is_event_queue_empty(queue)){
            // main event loop
            al_clear_to_color(al_map_rgb(0,0,0));

            // shift the eye guy around randomly
            background2.randomshift(1,6);
            
            // update the movement of the bone wands
            bw1.update(); bw2.update();

            // Do projections
            background2.proj();
            background.proj();
            p1.proj(); p2.proj();
            bw1.proj(); bw2.proj();
            meat1.proj(); meat2.proj(); meat3.proj();

            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_display(main_disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    // End the game
    return 0;
}
