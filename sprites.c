#include "sprites.h"
#include <raylib.h>

void SpriteInit(Sprite * sprite, Texture2D * t)
{
    sprite->image = t; 
    sprite->w = t->width;
    sprite->h = t->height;
    sprite->nw = t->width;
    sprite->nh = t->height;
}

void SpriteProj(Sprite * sprite)
{
    //al_draw_scaled_bitmap(image, 0, 0, al_get_bitmap_width(image) , al_get_bitmap_height(image) ,
    //                             _x+sx, _y+sy, nw, nh, 0);
    float scale = (float) sprite->nw / sprite->w;
    Vector2 pos = {
        sprite->_x+sprite->sx,
        sprite->_y+sprite->sy
    };
    DrawTextureEx( *(sprite->image), pos, 0, scale, WHITE);
}

// setting function for x,y
void SpriteSetXY(Sprite * sprite, int16_t x, int16_t y)
{
    sprite->_x = x;
    sprite->_y = y;
}

// Shift the sprite over a bit
void SpriteShift(Sprite * sprite, int16_t x, int16_t y)
{
    sprite->_x += x;
    sprite->_y += y;
}

void SpriteResizeH(Sprite * sprite, uint16_t h)
{
    float r = 1.0f * sprite->w / sprite->h;
    sprite->nw = 1.0f*h*r;
    sprite->nh = h;
}

void SpriteRandomShift(Sprite * sprite, uint8_t amt, int8_t lmt)
{
    uint16_t rnd = GetRandomValue(0,100);

    switch ( rnd % 8 ){
        case 1:
            if ( sprite->sx < lmt)
                sprite->sx += amt;
            break;
        case 2:
            if ( sprite->sy < lmt)
                sprite->sy += amt;
            break;
        case 3:
            if ( sprite->sx > -lmt)
                sprite->sx -= amt;
            break;
        case 4:
            if ( sprite->sy > -lmt)
                sprite->sy -= amt;
            break;
        default:
            break;
    }
}
