#include "sprites.h"
#include <math.h>

Bonewand::Bonewand() { }
Bonewand::~Bonewand() {/* do nothing*/}

Bonewand::Bonewand(const char * const filename, unsigned int height, bool direction) :
    Sprite( filename ),dir(direction) {
        idir = 0; t = 0;
        speed = 4;
        slide = false;
        resize_h(height);
}

void Bonewand::set_speed(unsigned int spd){ speed = spd; }

void Bonewand::stab(Meat * m1, Meat * m2, Meat * m3) {
    if ( t >= 50) {
        slide = true;

        if      (m1->inRange(_y)) target = m1;
        else if (m2->inRange(_y)) target = m2;
        else if (m3->inRange(_y)) target = m3;
        else target = NULL;

        t = 0;
    }
}

unsigned int Bonewand::getT(){return t;}

void Bonewand::update(){
    if (slide){ // this is for if we're selected to slide
        if ( get_x() < 360){
            if (get_x() > 229) { idir = 1; hitTarget(1);}
            if (idir == 1 && get_x() <= 50) {
                _x = 50;
                idir = 0;
                slide = false;
                return;
            }
                shift( pow(-1,(int)idir)*speed,0);

        } else {
            if (get_x() < 391) {idir = 1; hitTarget(-1);}
            if (idir == 1 && get_x() >= 580) {
                _x = 580;
                idir = 0;
                slide = false;
                return;
            }
                shift(pow(-1,(int)!idir)*speed,0);
        }

    } else { // this is for normal up/down movement
        if ( get_y() > 500 || get_y() < 30) dir = !dir; 
        
        shift(0,pow(-1,(int)!dir)*speed); 
        
        if (t < 50)
            t+=speed;
    }
}

void Bonewand::hitTarget(short dir){
    if (target != NULL){
        target->hit(dir);
    }
}