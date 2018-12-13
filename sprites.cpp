#include "sprites.h"

// overload for loading up an image in the constructor
Sprite::Sprite(const char * const filename) { 
    image = al_load_bitmap(filename);
    w = al_get_bitmap_width(image); h = al_get_bitmap_height(image);
    nw = w; nh = h;
}

// just a wrapper for the assign CImg function
void Sprite::assign(const char * const filename) {
    image = al_load_bitmap(filename); 
    w = al_get_bitmap_width(image); h = al_get_bitmap_height(image);
} 

// Function for Projecting
void Sprite::proj(){
    al_draw_scaled_bitmap(image, 0, 0, al_get_bitmap_width(image) , al_get_bitmap_height(image) ,
                                 _x, _y, nw, nh, 0);
}

// setting function for x,y
void Sprite::set_xy(signed int x, signed int y){
    _x = x; _y = y;
}

// Shift the sprite over a bit
void Sprite::shift(signed int x, signed int y){
    _x += x; _y += y;
}

void Sprite::resize_h(unsigned int height){
    float r = 1.0f*w/h;
    nw = 1.0f*height*r;
    nh = height;
}

signed int Sprite::get_x(){return _x;}
signed int Sprite::get_y(){return _y;}

void Sprite::randomshift(unsigned int amt, signed int lmt){
        unsigned int rnd = rand() % 100;

        switch ( rnd % 8 ){
            case 1:
                if (get_x() < lmt)
                    shift(amt,0);
                break;
            case 2:
                if (get_y() < lmt)
                    shift(0,amt);
                break;
            case 3:
                if (get_x() > -lmt)
                    shift(-amt,0);
                break;
            case 4:
                if (get_y() > -lmt)
                    shift(0,-amt);
                break;
            default:
                break;
        }
}