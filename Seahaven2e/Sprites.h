// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_SPRITES_H_
#define SEAHAVEN2E_SPRITES_H_

#include <stdint.h>
#include <initializer_list>
#include <C6502/LookupTable.h>
#include <Apple2Lib/HGRWord.h>
#include "Rank.h"
#include "Suit.h"

const uint8_t CardTopSpriteHeight = 9;
const uint8_t CardHeight = 33;


class CardTopSprite {
public:
   constexpr CardTopSprite(const std::initializer_list<const char *> &rowStrings, uint8_t alignOffset = 0) {
      int i=0;
      for (auto str : rowStrings)
         rows[i++] = a2::HGRWord(str, alignOffset);
   }

public:
   a2::HGRWord rows[CardTopSpriteHeight];
};

class CardTopDualSprite {
public:
   constexpr CardTopDualSprite(const std::initializer_list<const char *> &rowStrings)
   : even(rowStrings, 0), odd(rowStrings, 1)
   {
   }

   const CardTopSprite &GetSprite(bool oddColors) const {
      return oddColors ? odd : even;
   }

public:
   CardTopSprite even;
   CardTopSprite odd;
};

/// <summary>
/// Sprite ID... easier to pass around than a pointer
/// </summary>
class SpriteID {
public:
   SpriteID() {}

   static SpriteID FromRank(Rank rank);
   static SpriteID FromSuit(SuitOrdinal suitOrdinal, bool oddColors);

   uint8_t GetIndex() const { return index; }

private:
   SpriteID(uint8_t index) : index(index) {}

private:
   uint8_t index;
};

class SpriteOffsetLookup {
public:
   constexpr SpriteOffsetLookup() : lookup() {
      uint16_t offset = 0;
      for (uint8_t i=0; i < 19; ++i)
      {
         lookup.Set(i, offset);
         offset += sizeof(CardTopSprite);
      }
   }

   uint16_t Get(uint8_t index) const { return lookup.Get(index); }

private:
   c6502::Lookup16Bit<uint16_t, 19> lookup;
};

/// <summary>
/// Our registry of sprites
/// </summary>
class Sprites {
public:
   static const CardTopSprite *GetSprite(SpriteID spriteID);

public:
   static const CardTopSprite cursorLeft;
   static const CardTopSprite cursorRight;

private:
   static const CardTopSprite cardTopSprites[19];
   static const SpriteOffsetLookup spritesOffsetLookup;
};

#endif  // SEAHAVEN2E_SPRITES_H_
