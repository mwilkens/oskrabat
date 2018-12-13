#include "sprites.h"
#include <math.h>

Bonewand::Bonewand() { }
Bonewand::~Bonewand() {/* do nothing*/}

Bonewand::Bonewand(const char * const filename, unsigned int height, bool direction) :
    Sprite( filename ),dir(direction) {
        idir = 0; t = 0;
        speed = 4;
        resize_h(height);
}

void Bonewand::set_speed(unsigned int spd){ speed = spd; }

void Bonewand::stab() {
    if ( t >= 50) {
        slide = true;
        t = 0;
    }
}

unsigned int Bonewand::getT(){return t;}

void Bonewand::update(){
    if (slide){ // this is for if we're selected to slide
        if ( get_x() < 360){
            if (get_x() > 219) idir = 1;
            if (idir == 1 && get_x() == 10) {
                idir = 0;
                slide = false;
                return;
            }
                shift( pow(-1,(int)idir)*15,0);

        } else {
            if (get_x() < 381) idir = 1;
            if (idir == 1 && get_x() == 590) {
                idir = 0;
                slide = false;
                return;
            }
                shift(pow(-1,(int)!idir)*15,0);
        }

    } else { // this is for normal up/down movement
        if ( get_y() > 500 || get_y() < 10) dir = !dir; 
        
        shift(0,pow(-1,(int)!dir)*speed); 
        
        if (t < 50)
            t+=speed;
    }
}