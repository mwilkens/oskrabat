#include "sprites.h"
#include <math.h>

void BoneInit(Bonewand * b, Texture2D * t, uint16_t height, bool direction)
{
    SpriteInit( &(b->sprite), t );
    b->dir = direction;
    b->idir = 0;
    b->t = 0;
    b->speed = 4;
    b->slide = false;
    b->target = NULL;
    SpriteResizeH( &(b->sprite), height);
}

void BoneReset(Bonewand * b)
{
    b->slide = false; b->idir = false;
}

void BoneSetSpeed(Bonewand * b, uint16_t spd)
{
    b->speed = spd;
}

void BoneStab(Bonewand * b, Meat * m1, Meat * m2, Meat * m3)
{
    if ( b->t >= 50) {
        b->slide = true;

        if      (MeatInRange(m1,b->sprite._y)) b->target = m1;
        else if (MeatInRange(m2,b->sprite._y)) b->target = m2;
        else if (MeatInRange(m3,b->sprite._y)) b->target = m3;
        else b->target = NULL;

        b->t = 0;
    }
}

void BoneUpdate(Bonewand * b)
{
    if (b->slide){ // this is for if we're selected to slide
        if ( b->sprite._x < 360)
        {
            if ( b->sprite._x > 229)
            {
                b->idir = 1;
                BoneHitTarget(b, 1);
            }
            if (b->idir == 1 && b->sprite._x <= 50)
            {
                b->sprite._x = 50;
                b->idir = 0;
                b->slide = false;
                return;
            }
            SpriteShift( &(b->sprite), pow(-1,(int)b->idir)*b->speed,0);

        } else
        {
            if (b->sprite._x < 391)
            {
                b->idir = 1;
                BoneHitTarget(b,-1);
            }
            
            if (b->idir == 1 && b->sprite._x >= 580)
            {
                b->sprite._x = 580;
                b->idir = 0;
                b->slide = false;
                return;
            }
            
            SpriteShift( &(b->sprite), pow(-1, (int) !(b->idir)) * b->speed, 0);
        }

    }
    else
    { // this is for normal up/down movement
        if ( b->sprite._y > 500 || b->sprite._y < 30)
        {
            b->dir = !b->dir;
        }
        
        SpriteShift( &(b->sprite), 0, pow(-1, (int)!(b->dir) ) * b->speed); 
        
        if (b->t < 50)
        {
            b->t += b->speed;
        }
    }
}

void BoneHitTarget(Bonewand * b, int8_t dir)
{
    if (b->target != NULL){
        MeatHit(b->target, dir);
    }
}
