#include "sprites.h"
#include <stdio.h>

Character::Character(const char * const fup, const char * const fdown) :
    Sprite( fup ), squished (false) {
    snprintf(_fup,20,"%s",fup);
    snprintf(_fdown,20,"%s",fdown);
}

Character::Character(const char * const fup, const char * const fdown, unsigned int height) :
    Sprite( fup ), squished (false) {
    snprintf(_fup,20,"%s",fup);
    snprintf(_fdown,20,"%s",fdown);
    max_h = height;
    orig_h = h;
    resize_h(max_h);
}

Character::~Character() {/* do nothing*/}

void Character::squish () {
    if ( squished ) return;
    assign(_fdown);
    resize_h ( (int) (1.0f * h * max_h/orig_h) );
    shift(0,max_h);
    shift(0,-nh);
    squished = true;
}
void Character::unsquish () {
    if( !squished ) return;
    unsigned int oldH = nh;
    assign(_fup);
    resize_h(max_h);
    shift(0,-max_h);
    shift(0,oldH);
    squished = false;
}

bool Character::get_squish(){ return squished; }
