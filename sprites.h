#define _SPRITES_H

#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef signed char int8_t;
typedef signed int int16_t;

#define WINNER_NONE 0 
#define WINNER_P1   1
#define WINNER_P2   2

struct Sprite {
    int16_t _x;
    int16_t _y;
    int16_t sx;
    int16_t sy;
    int16_t w;
    int16_t h;
    int16_t nw;
    int16_t nh;
    Texture2D *image;
};
typedef struct Sprite Sprite;


void SpriteInit(Sprite * sprite, Texture2D * t);
void SpriteProj(Sprite * sprite);
void SpriteSetXY(Sprite * sprite, int16_t x, int16_t y);
void SpriteShift(Sprite * sprite, int16_t x, int16_t y);
void SpriteResizeH(Sprite * sprite, uint16_t h);
void SpriteRandomShift(Sprite * sprite, uint8_t amt, int8_t lmt);

struct Character {
    Texture2D * _fup;
    Texture2D * _fdown;
    uint16_t max_h;
    uint16_t orig_h;
    bool squished;
    Sprite sprite;
};
typedef struct Character Character;

void CharInit(Character * c, Texture2D * fup, Texture2D * fdown, uint16_t h);
void CharSquish(Character * c);
void CharUnsquish(Character * c);

struct Meat {
    int8_t status;
    bool falling;
    Sprite sprite;
};
typedef struct Meat Meat;

void MeatInit(Meat * m, Texture2D * t, uint16_t height);
bool MeatInRange(Meat * m, uint16_t y);
void MeatHit(Meat * m, int8_t s);
void MeatResetStatus(Meat * m);
void MeatFall(Meat * m, uint16_t * winner);

struct Bonewand {
    uint16_t speed;
    uint16_t t;
    bool dir;
    bool idir;
    bool slide;
    Sprite sprite;
    Meat * target;
};
typedef struct Bonewand Bonewand;

void BoneInit(Bonewand * b, Texture2D * t, uint16_t height, bool direction);
void BoneSetSpeed(Bonewand * b, uint16_t spd);
void BoneStab(Bonewand * b, Meat * m1, Meat * m2, Meat * m3);
void BoneReset(Bonewand * b);
void BoneUpdate(Bonewand * b);
void BoneHitTarget(Bonewand * b, int8_t dir);
