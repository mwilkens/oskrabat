#include "sprites.h"
#include <stdio.h>

#define NUM_STAGES 3.0

bool Meat::inRange(unsigned int y){
    return ( y > _y-nh && y < _y);
}

void Meat::hit(signed short s){
    if (falling) return;
    status += s;
    if (status == NUM_STAGES || status == -NUM_STAGES) falling = true;
    if (status > NUM_STAGES || status < -NUM_STAGES) return;
    shift(s*(1.2*nw/NUM_STAGES), 0);
}

void Meat::fall(unsigned int * winner){
    if (_y < 550) shift(0,4 + (30.0*_y/550));
    else *winner = (status == NUM_STAGES) ? WINNER_P1 : WINNER_P2;
}