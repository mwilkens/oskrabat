/*
 #
 #  File        : oskrabat.cpp
 #                ( C++ source file )
 #
 #  Description : Oskrabat (Scrape/Flesh) (remake of Dong Dong)
 #
 #  Copyright   : Mandy Wilkens
 #
 #  License     : CeCILL v2.0
 #                ( http://www.cecill.info/licences/Licence_CeCILL_V2-en.html )
 #
 #  This software is governed by the CeCILL  license under French law and
 #  abiding by the rules of distribution of free software.  You can  use,
 #  modify and/ or redistribute the software under the terms of the CeCILL
 #  license as circulated by CEA, CNRS and INRIA at the following URL
 #  "http://www.cecill.info".
 #
 #  As a counterpart to the access to the source code and  rights to copy,
 #  modify and redistribute granted by the license, users are provided only
 #  with a limited warranty  and the software's author,  the holder of the
 #  economic rights,  and the successive licensors  have only  limited
 #  liability.
 #
 #  In this respect, the user's attention is drawn to the risks associated
 #  with loading,  using,  modifying and/or developing or reproducing the
 #  software by the user in light of its specific status of free software,
 #  that may mean  that it is complicated to manipulate,  and  that  also
 #  therefore means  that it is reserved for developers  and  experienced
 #  professionals having in-depth computer knowledge. Users are therefore
 #  encouraged to load and test the software's suitability as regards their
 #  requirements in conditions enabling the security of their systems and/or
 #  data to be ensured and,  more generally, to use and operate it in the
 #  same conditions as regards security.
 #
 #  The fact that you are presently reading this means that you have had
 #  knowledge of the CeCILL license and that you accept its terms.
 #
*/

// Include CImg library file and use its main namespace
#include "CImg.h"
using namespace cimg_library;

#include <stdio.h>

const unsigned char pink[] {255,0,255};

// Class Sprite
class Sprite{
    public:
        Sprite();
        ~Sprite() { /*do nothing*/}

        // overload for loading up an image in the constructor
        Sprite(const char * const filename) { 
            image.assign(filename);
            mask = image.get_threshold(1).channel(1).normalize(0,255);
            w = image.width(); h = image.height();
        }

        // just a wrapper for the assign CImg function
        void assign(const char * const filename) {
            image.assign(filename);
            mask = image.get_threshold(1).channel(1).normalize(0,255);
            w = image.width(); h = image.height();
        } 

        // Function for Projecting
        void proj(CImg<unsigned char> * buff){
            buff->draw_image(_x,_y,mask,1);
            // this function is SO slow
            //buff->draw_image(_x,_y,0,0,image,mask,1,255);
        }

        // setting function for x,y
        void set_xy(signed int x, signed int y){
            _x = x; _y = y;
        }

        // Shift the sprite over a bit
        void shift(signed int x, signed int y){
            _x += x; _y += y;
        }

        void resize_h(unsigned int height){
            float r = 1.0f*w/h;
            unsigned int new_w = 1.0f*height*r;
            image.resize(new_w,height);
            mask.resize(new_w,height);
            w = new_w; h = height;
        }

        signed int get_x(){return _x;}
        signed int get_y(){return _y;}

    protected:
        CImg<unsigned char> image, mask;
        signed int _x,_y;
        signed int w, h;
};

class Character : public Sprite {
    public:
        Character() { squished = false; }
        ~Character() {/* do nothing*/}
        
        Character(const char * const fup, const char * const fdown) :
            Sprite( fup ), squished (false) {
            snprintf(_fup,20,"%s",fup);
            snprintf(_fdown,20,"%s",fdown);
        }

        Character(const char * const fup, const char * const fdown, unsigned int height) :
            Sprite( fup ), squished (false) {
            snprintf(_fup,20,"%s",fup);
            snprintf(_fdown,20,"%s",fdown);
            max_h = height;
            orig_h = h;
            resize_h(max_h);
        }

        void squish () {
            if ( squished ) return;
            assign(_fdown);
            resize_h ((int) (1.0f * h * max_h/orig_h) );
            shift(0,max_h);
            shift(0,-h);
            squished = true;
        }
        void unsquish () {
            if( !squished ) return;
            unsigned int oldH = h;
            assign(_fup);
            resize_h(max_h);
            shift(0,-max_h);
            shift(0,oldH);
            squished = false;
        }

        bool get_squish(){ return squished; }

    private:
        char _fup[20], _fdown[20];
        unsigned int max_h, orig_h;
        bool squished;
};

class Meat : public Sprite {
    public:
        Meat() {}
        ~Meat() {}

        Meat(const char * const filename) : Sprite(filename), status(0) {}

    protected:
        signed short int status;
};

class Bonewand : public Sprite {
    public:
        Bonewand() { }
        ~Bonewand() {/* do nothing*/}
        
        Bonewand(const char * const filename, unsigned int height, bool direction) :
            Sprite( filename ),dir(direction) {
                max_h = height;
                orig_h = h;
                idir = 0; stabt = 999; t = 0;
                speed = 4;
                resize_h(max_h);
        }

        void set_speed(unsigned int spd){ speed = spd; }

        void stab() {
            //if ( !(stabt + 25 > t && stabt - 25 < t) ) {
                slide = true;
                //stabt = t;
           // }
        }

        void update(){
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
                t = t % 100;

                // reset the stab time
                //if ( !(stabt + 25 > t && stabt - 25 < t) ) stabt = 999;
                
                if ( get_y() > 500 || get_y() < 10) dir = !dir; 
                
                shift(0,pow(-1,(int)!dir)*speed); 
                
                t++;
            }
        }

    protected:
        unsigned int max_h, orig_h;
        unsigned int speed, t, stabt;
        bool dir, idir, slide;
};

// Main
// -----
int main(int argc, char **argv) {
    // The -h help dialogue
    cimg_usage("OSKRABAT!\n(by Mandy).");
    cimg_help("\n"
            "** Some Help ***********************\n"
            " 1) You'll love this game\n"
            " 2) Can't get enough of it\n"
            " 3) Play with your friend\n"
            " 4) You both love it\n"
            "************************************");

    // The dimentions of the screen
    const unsigned int Wi = 800, Hi = 800;

    // Main program window
    CImgDisplay
        main_disp(Wi,Hi, "(FLESH)",0);

    CImg<unsigned char> buffer(Wi,Hi,1,4,0);
    
    Sprite background("./res/background.jpg");
    Sprite background2("./res/background2.jpg");
   
    background.set_xy(0,0); background2.set_xy(0,0);

    Character p1("./res/p1up.jpg", "./res/p1down.jpg",200); p1.set_xy(210,550);
    Character p2("./res/p2up.jpg", "./res/p2down.jpg",200); p2.set_xy(445,550);

    Meat meat1("./res/meat.jpg"); meat1.resize_h(50); meat1.set_xy(350,75);
    Meat meat2("./res/meat.jpg"); meat2.resize_h(50); meat2.set_xy(350,275);
    Meat meat3("./res/meat.jpg"); meat3.resize_h(50); meat3.set_xy(350,475);

    Bonewand bw1("./res/bonewand.jpg",100,1); bw1.set_xy(10,200);
    Bonewand bw2("./res/bonewand2.jpg",100,0); bw2.set_xy(590,200);
    
    bw1.set_speed(10);
    bw2.set_speed(10);

    unsigned int fnum = 0;

    // The main loop
    while (!main_disp.is_closed() && !main_disp.is_keyESC() && !main_disp.is_keyQ()){
    
        if(!(main_disp.mouse_x()>=0) && !(main_disp.mouse_y()>=0)){
            // if we're not in the screen wait and don't update screen
            cimg::wait(500);
            continue;
        }

        unsigned int rnd = rand() % 100;

        switch ( rnd % 8 ){
            case 1:
                if (background2.get_x() < 6)
                    background2.shift(1,0);
                break;
            case 2:
                if (background2.get_y() < 6)
                    background2.shift(0,1);
                break;
            case 3:
                if (background2.get_x() > -6)
                    background2.shift(-1,0);
                break;
            case 4:
                if (background2.get_y() > -6)
                    background2.shift(0,-1);
                break;
            default:
                break;
        }

        // main event loop
        buffer.fill(0);

        if(main_disp.is_keyX()) { bw1.stab(); }
        //else { p1.unsquish(); }


        if(main_disp.is_keyM()) { bw2.stab(); }
        //else { p2.unsquish(); }

        if(main_disp.is_keyARROWLEFT()) { p1.squish(); }
        else { p1.unsquish(); }

        if(main_disp.is_keyARROWRIGHT()) { p2.squish(); }
        else { p2.unsquish(); }

        bw1.update(); bw2.update();

        //buffer.draw_text(0,0,"P1: %u,%u",white,12,1,24,p1.get_x(),p1.get_y());
        //buffer.draw_text(0,25,"P2: %u,%u",white,12,1,24,p2.get_x(),p2.get_y());
        
        // Do projections
        background2.proj(&buffer);
        background.proj(&buffer);
        p1.proj(&buffer); p2.proj(&buffer);
        bw1.proj(&buffer); bw2.proj(&buffer);
        meat1.proj(&buffer); meat2.proj(&buffer); meat3.proj(&buffer);

        // uncomment this block if you wanna make an animated gif
        //char c[20];
        //sprintf(c, "./gif/gif%04d.jpeg", fnum);
        //fnum++;
        //buffer.save(c);

        // update at 60fps
        main_disp.display(buffer).wait(16);
    }

    // End the game
    return 0;
}
