#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>

#include "sprites.h"

#define KEY_SEEN 1
#define KEY_RELEASED 2

// Main
// -----
int main(int argc, char **argv) {

    // The dimentions of the screen
    const uint16_t Wi = 800, Hi = 800;
   
    // Main program window
    InitWindow(Wi, Hi, "Oskrabat");
    InitAudioDevice();
    SetTargetFPS(60);

    // -----------------------------
    // Loading and defining sprites
    // -----------------------------
    Texture2D t_background = LoadTexture("res/background.png");
    Sprite background;
    SpriteInit(&background, &t_background);

    Texture2D t_background2 = LoadTexture("res/background2.png"); 
    Sprite background2;
    SpriteInit(&background2, &t_background2);
    
    Texture2D t_border = LoadTexture("res/border.png");
    Sprite border;
    SpriteInit(&border, &t_border);
    
    Texture2D t_title_text = LoadTexture("res/title.png");
    Sprite title_text;
    SpriteInit(&title_text, &t_title_text);

    Texture2D t_p1_up = LoadTexture("res/p1up");
    Texture2D t_p1_down = LoadTexture("res/p1down");
    Character p1;
    CharInit(&p1, &t_p1_up, &t_p1_down, 200);

    Texture2D t_p2_up = LoadTexture("res/p2up");
    Texture2D t_p2_down = LoadTexture("res/p2down");
    Character p2;
    CharInit(&p2, &t_p2_up, &t_p2_down, 200);

    Texture2D t_meat1 = LoadTexture("res/meat.png");
    Meat meat1;
    MeatInit(&meat1, &t_meat1, 50);
    Meat meat2;
    MeatInit(&meat2, &t_meat1, 50);
    Meat meat3;
    MeatInit(&meat3, &t_meat1, 50);    

    Texture2D t_bw1 = LoadTexture("res/bonewand.png");
    Bonewand bw1;
    BoneInit(&bw1, &t_bw1, 80, true);
    
    Texture2D t_bw2 = LoadTexture("res/bonewand.png");
    Bonewand bw2;
    BoneInit(&bw2, &t_bw2, 80, false);
    
    uint8_t winner = WINNER_NONE;

    // Configuring events

    bool title = true;
    bool running = true;
    bool playing = true;
    bool redraw = true;

    // Wait for Audio to initialize
    while(!IsAudioDeviceReady()){}

    Music music = LoadMusicStream("res/music.ogg");
    Sound bone = LoadSound("res/hit.ogg");
    Sound win_sound = LoadSound("res/final.ogg");

    PlayMusicStream(music);

    // ---------------------
    // The main loop
    // ---------------------
    while (!WindowShouldClose()) {

        while(running){

            // Set Defaults
            printf("Setting Defaults...\n");
            
            SpriteSetXY(&background,0,10);
            SpriteSetXY(&background2,195,40);
            
            SpriteSetXY(&border,0,0);
            SpriteShift(&border,-5,-5);
            
            SpriteSetXY(&title_text,170,240);
            
            CharUnsquish(&p1);
            SpriteSetXY(&(p1.sprite),210,550);
            CharUnsquish(&p2);
            SpriteSetXY(&(p2.sprite),445,550);
            
            SpriteSetXY(&(meat1.sprite),360,100);
            MeatResetStatus(&meat1);
            SpriteSetXY(&(meat2.sprite),360,275); 
            MeatResetStatus(&meat2);
            SpriteSetXY(&(meat3.sprite),360,450);
            MeatResetStatus(&meat3);
            
            SpriteSetXY(&(bw1.sprite),60,200);
            BoneReset(&bw1);
            SpriteSetXY(&(bw2.sprite),580,200); 
            BoneReset(&bw2);
            BoneSetSpeed(&bw1,18);
            BoneSetSpeed(&bw2,18);
            
            title = true; running = true; playing = true; redraw = true;
            winner = WINNER_NONE;

            BeginDrawing();
            EndDrawing();
            /*

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
                                else al_play_sample(bone, 2.0, -0.8,1.0,ALLEGRO_PLAYMODE_ONCE, currSample);
                            }

                            if(key[ALLEGRO_KEY_M] & KEY_RELEASED == KEY_RELEASED) {
                                bw2.stab(&meat1,&meat2,&meat3);
                                if (currSample) al_stop_sample(currSample);
                                else al_play_sample(bone, 2.0, 0.8,1.0,ALLEGRO_PLAYMODE_ONCE, currSample);
                            }

                            if(key[ALLEGRO_KEY_LEFT]) p1.get_squish() ? p1.unsquish() : p1.squish();
                            if(key[ALLEGRO_KEY_RIGHT]) p2.get_squish() ? p2.unsquish() : p2.squish();

                            if((key[ALLEGRO_KEY_SPACE] & KEY_RELEASED == KEY_RELEASED) && winner != WINNER_NONE) { playing = false; } 
                        }

                        if (rand() % 150 == 0) {
                            unsigned int spd = 15 + 2*(rand()%10);
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
                                al_play_sample(win_sound, 1.0, 0.0,0.5,ALLEGRO_PLAYMODE_ONCE, NULL);
                                break;
                            case WINNER_P2:
                                p1.squish();
                                al_play_sample(win_sound, 1.0, 0.0,0.5,ALLEGRO_PLAYMODE_ONCE, NULL);
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
            } // While Playing
            */
        } // While Running
        // to restart
    } // While !WindowShouldClose

    CloseWindow();

    // End the game
    return 0;
}
