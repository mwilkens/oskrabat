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
    SetTargetFPS(30);

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

    Texture2D t_p1_up = LoadTexture("res/p1up.png");
    Texture2D t_p1_down = LoadTexture("res/p1down.png");
    Character p1;
    CharInit(&p1, &t_p1_up, &t_p1_down, 200);

    Texture2D t_p2_up = LoadTexture("res/p2up.png");
    Texture2D t_p2_down = LoadTexture("res/p2down.png");
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
    
    Texture2D t_bw2 = LoadTexture("res/bonewand2.png");
    Bonewand bw2;
    BoneInit(&bw2, &t_bw2, 80, false);
    
    uint16_t winner = WINNER_NONE;

    // Configuring events

    bool title = true;
    bool running = true;
    bool playing = false;
    bool redraw = true;

    // Wait for Audio to initialize
    while(!IsAudioDeviceReady()){}

    Music music = LoadMusicStream("res/music.ogg");
    Sound bone = LoadSound("res/hit.ogg");
    Sound win_sound = LoadSound("res/final.ogg");
    Sound dead_sound = LoadSound("res/dead.ogg");

    PlayMusicStream(music);
    SetMusicVolume(music, 0.1f);

    // ---------------------
    // The main loop
    // ---------------------
    while (!WindowShouldClose()) 
    {
        UpdateMusicStream(music);
        if(!playing)
        {
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

            title = true; playing = true; redraw = true;
            winner = WINNER_NONE;
        }
        else
        {
            // quit the game
            if( IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_Q) )
            {
                StopMusicStream(music);
                PlayMusicStream(music);
                playing = false;
            }

            // hold on title until space pressed
            if(title)
            {
                if( IsKeyPressed(KEY_SPACE) )
                {
                    title = false;
                    printf("[INFO] Starting the game!\n");
		    PlaySound(win_sound);
                }
                redraw = true;
            }
            else
            {
                if( IsKeyPressed(KEY_X) )
                {
                    BoneStab(&bw1,&meat1,&meat2,&meat3);
                    PlaySound(bone);
                }

                if( IsKeyPressed(KEY_M) )
                {
                    BoneStab(&bw2,&meat1,&meat2,&meat3);
                    PlaySound(bone);
                }

                if( IsKeyPressed(KEY_LEFT) )
                {
                    p1.squished ? CharUnsquish(&p1) : CharSquish(&p1);
                }

                if( IsKeyPressed(KEY_RIGHT) )
                {
                    p2.squished ? CharUnsquish(&p2) : CharSquish(&p2);
                }

                if( IsKeyPressed(KEY_SPACE) && winner != WINNER_NONE )
                {
                    playing = false;
                }

                if (GetRandomValue(0,150) == 0) {
                    uint16_t spd = 15 + 2*(GetRandomValue(0,10));
                    BoneSetSpeed(&bw1,spd);
                    BoneSetSpeed(&bw2,spd);
                }

                redraw = true;
            }

            if(redraw)
            {
                // main event loop
                BeginDrawing();
                ClearBackground(BLACK);

                // shift the eye guy around randomly
                SpriteRandomShift(&background2,1,20);
                SpriteRandomShift(&border,1,2);
                SpriteRandomShift(&(p1.sprite),1,1);
                SpriteRandomShift(&(p2.sprite),1,1);

                // Do background projections
                SpriteProj(&background2);
                SpriteProj(&background);
                SpriteProj(&(p1.sprite));
                SpriteProj(&(p2.sprite));

                if(!title)
                {
                    // this is literally all the game logic lmao
                    SpriteRandomShift(&(meat1.sprite),1,1);
                    SpriteRandomShift(&(meat2.sprite),1,1);
                    SpriteRandomShift(&(meat3.sprite),1,1);

                    // update the movement of the bone wands
                    if(meat1.falling)
                    {
                        MeatFall(&meat1,&winner);
                    }
                    else if(meat2.falling)
                    {
                        MeatFall(&meat2,&winner);
                    }
                    else if(meat3.falling)
                    {
                        MeatFall(&meat3,&winner);
                    }
                    else
                    {
                        BoneUpdate(&bw1);
                        BoneUpdate(&bw2);
                    }

                    switch(winner)
                    {
                        case WINNER_P1:
                            CharSquish(&p2);
                            PlaySound(dead_sound);
                            break;
                        case WINNER_P2:
                            CharSquish(&p1);
                            PlaySound(dead_sound);
                            break;
                        case WINNER_NONE:
                            break;
                    }

                    SpriteProj(&(bw1.sprite));
                    SpriteProj(&(bw2.sprite));
                    SpriteProj(&(meat1.sprite));
                    SpriteProj(&(meat2.sprite));
                    SpriteProj(&(meat3.sprite));

                } else {
                    SpriteRandomShift(&title_text,1,3);
                    SpriteProj(&title_text);
                }
                
                SpriteProj(&border);
                EndDrawing();
                redraw = false;
            } // If Redraw
        } // If Playing
    } // While !WindowShouldClose
    
    UnloadTexture(t_background);
    UnloadTexture(t_background2);
    UnloadTexture(t_border);
    UnloadTexture(t_title_text);
    UnloadTexture(t_p1_up);
    UnloadTexture(t_p1_down);
    UnloadTexture(t_p2_up);
    UnloadTexture(t_p2_down);
    UnloadTexture(t_meat1);
    UnloadTexture(t_bw1);
    UnloadTexture(t_bw2);
    UnloadMusicStream(music);
    UnloadSound(bone);
    UnloadSound(win_sound);
    UnloadSound(dead_sound);

    CloseAudioDevice();
    CloseWindow();

    // End the game
    return 0;
}
