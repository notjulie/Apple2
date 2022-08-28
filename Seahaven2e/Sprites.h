
#ifndef SPRITES_H
#define SPRITES_H

#include <stdint.h>
#include <C6502/LookupTable.h>
#include <Apple2Lib/HGRWord.h>
#include "Rank.h"
#include "Suit.h"

const uint8_t CardTopSpriteHeight = 9;

typedef a2::HGRWord CardTopSprite[CardTopSpriteHeight];


class Sprites {
public:
   static void Initialize();

   inline static const CardTopSprite &GetRankSprite(Rank rank) {
      return *ranksLookup.Get((uint8_t)rank - 1);
   }
   inline static const CardTopSprite &GetSuitSprite(Suit suit) {
      return *suitsLookup.Get((uint8_t)suit);
   }

private:
   static const CardTopSprite ranks[13];
   static c6502::Lookup16Bit<const CardTopSprite *, 13> ranksLookup;

   static const CardTopSprite suits[4];
   static c6502::Lookup16Bit<const CardTopSprite *, 4> suitsLookup;
};

#endif // SPRITES_H