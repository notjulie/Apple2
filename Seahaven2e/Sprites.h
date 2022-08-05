
#ifndef SPRITES_H
#define SPRITES_H

#include <stdint.h>
#include <Apple2Lib/HGRWord.h>

const uint8_t CardTopSpriteHeight = 9;

typedef a2::HGRWord CardTopSprite[CardTopSpriteHeight];

extern const CardTopSprite suits[4];

#endif // SPRITES_H
