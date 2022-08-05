
#ifndef SPRITES_H
#define SPRITES_H

#include <stdint.h>
#include <Apple2Lib/HGRWord.h>
#include "Rank.h"

const uint8_t CardTopSpriteHeight = 9;

typedef a2::HGRWord CardTopSprite[CardTopSpriteHeight];

extern const CardTopSprite suits[4];

class Sprites {
public:
   inline static const CardTopSprite &GetRankSprite(Rank rank) {
      return ranks[(uint8_t)rank - 1];
   }

private:
   static const CardTopSprite ranks[13];
};

#endif // SPRITES_H
