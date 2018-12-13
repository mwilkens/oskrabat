#include "sprites.h"
#include <stdio.h>

bool Meat::inRange(unsigned int y){
    return ( y > _y-nh && y < _y);
}

void Meat::hit(signed short s){
    printf("hit!!! %d -> %d\n", _y, s);
    status += s;
    if (status == 3 || status == -3) falling = true;
    if (status > 3 || status < -3) return;
    shift(s*(1.2*nw/3.0), 0);
}

void Meat::fall(unsigned int * winner){
    if (_y < 550) shift(0,4);
    else *winner = (status == 3) ? WINNER_P1 : WINNER_P2;
}