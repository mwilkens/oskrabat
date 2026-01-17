#include "sprites.h"
#include <raylib.h>

#define NUM_STAGES 3.0

void MeatInit(Meat * m, Texture2D * t, uint16_t height)
{
    SpriteInit( &(m->sprite), t);
    SpriteResizeH( &(m->sprite), height);
}

void MeatResetStatus(Meat * m)
{
    m->status = 0;
    m->falling = false;
}

bool MeatInRange(Meat * m, uint16_t y)
{
    return ( y > m->sprite._y - m->sprite.nh
                 && y < m->sprite._y);
}

void MeatHit(Meat * m, int8_t s)
{
    if (m->falling) return;
    m->status += s;
    if (m->status == NUM_STAGES || m->status == -NUM_STAGES)
    {
        m->falling = true;
    }
    if (m->status > NUM_STAGES || m->status < -NUM_STAGES) return;
    
    SpriteShift( &(m->sprite), s * (1.2 * m->sprite.nw / NUM_STAGES) , 0);
}

void MeatFall(Meat * m, uint16_t * winner)
{
    if (m->sprite._y < 550)
    {
        SpriteShift(&(m->sprite), 0, 4 + (30.0*m->sprite._y/550));
    }
    else
    {
        *winner = (m->status == NUM_STAGES) ? WINNER_P1 : WINNER_P2;
    }
}
