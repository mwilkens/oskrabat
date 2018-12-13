#define _SPRITES_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

// Class Sprite
class Sprite{
    public:
        Sprite() : image(NULL) {}
        ~Sprite() { al_destroy_bitmap(image); }

        // overload for loading up an image in the constructor
        Sprite(const char * const filename);

        // just a wrapper for the assign CImg function
        void assign(const char * const filename);

        // Function for Projecting
        void proj();

        // setting function for x,y
        void set_xy(signed int x, signed int y);

        // Shift the sprite over a bit
        void shift(signed int x, signed int y);

        void resize_h(unsigned int height);

        signed int get_x();
        signed int get_y();

        void randomshift(unsigned int amt, signed int lmt);

    protected:
        ALLEGRO_BITMAP *image;
        signed int _x,_y;
        signed int w, h, nw, nh;
};

class Character : public Sprite {
    public:
        Character(const char * const fup, const char * const fdown);

        Character(const char * const fup, const char * const fdown, unsigned int height);

        ~Character();

        void squish ();
        void unsquish ();

        bool get_squish();

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
        Bonewand();
        ~Bonewand();
        
        Bonewand(const char * const filename, unsigned int height, bool direction);

        void set_speed(unsigned int spd);

        void stab();

        unsigned int getT();

        void update();

    protected:
        unsigned int speed, t;
        bool dir, idir, slide;
};
