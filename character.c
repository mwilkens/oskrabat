#include "sprites.h"

void CharInit(Character * c, Texture2D * fup, Texture2D * fdown, uint16_t height)
{
    c->_fup = fup;
    c->_fdown = fdown;
    SpriteInit( &(c->sprite), fup );
    c->squished = false;
    c->max_h = height;
    c->orig_h = c->sprite.h;
    SpriteResizeH( &(c->sprite), c->max_h);
}

void CharSquish(Character * c)
{
    if ( c->squished ) return;
    
    c->sprite.image = c->_fdown;
    SpriteInit( &(c->sprite), c->_fdown );
    
    SpriteResizeH( &(c->sprite), (uint16_t) (1.0f * c->sprite.h * c->max_h/c->orig_h) );
    SpriteShift( &(c->sprite), 0, c->max_h);
    SpriteShift( &(c->sprite), 0, -(c->sprite.nh) );
    c->squished = true;
}

void CharUnsquish(Character * c)
{
    if( !c->squished ) return;
    uint16_t oldH = c->sprite.nh;
    
    c->sprite.image = c->_fup;
    SpriteInit( &(c->sprite), c->_fup );

    SpriteResizeH( &(c->sprite), c->max_h);
    SpriteShift( &(c->sprite), 0, -(c->max_h) );
    SpriteShift( &(c->sprite), 0, oldH);
    c->squished = false;
}
