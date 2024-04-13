// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Sprites.h"

c6502::Lookup16Bit<const CardTopSprite *, 19> Sprites::spritesLookup;

// ==================================================
//    raw definitions of the suit sprites
// ==================================================
constexpr std::initializer_list<const char *> clubsBits = {
    "WWWWWWWWWWWW  ",
    "WWWWW  WWWWWW ",
    "WWWW    WWWWW ",
    "WWWWW  WWWWWW ",
    "WW        WWW ",
    "WW        WWW ",
    "WWWWW  WWWWWW ",
    "WWWW    WWWWW ",
    "WWWWWWWWWWWWW "
};
constexpr std::initializer_list<const char *> diamondsBits = {
    "WWWWWWWWWWWW  ",
    "WWWWWRRRWWWWW ",
    "WWWWRRRRRWWWW ",
    "WWWRRRRRRRWWW ",
    "WWRRRRRRRRRWW ",
    "WWWRRRRRRRWWW ",
    "WWWWRRRRRWWWW ",
    "WWWWWRRRWWWWW ",
    "WWWWWWWWWWWWW "
};
constexpr std::initializer_list<const char *> heartsBits = {
    "WWWWWWWWWWWW  ",
    "WWWRRRWWRRWWW ",
    "WRRRRRRRRRRRW ",
    "WWRRRRRRRRRWW ",
    "WWWRRRRRRRWWW ",
    "WWWWRRRRRWWWW ",
    "WWWWWRRRWWWWW ",
    "WWWWWWWWWWWWW ",
    "WWWWWWWWWWWWW "};
constexpr std::initializer_list<const char *> spadesBits = {
    "WWWWWWWWWWWW  ",
    "WWWWW  WWWWWW ",
    "WWWW    WWWWW ",
    "WW        WWW ",
    "W          WW ",
    "WW        WWW ",
    "WWWWW  WWWWWW ",
    "WWWW    WWWWW ",
    "WWWWWWWWWWWWW ",
};

// ==================================================
//    suit sprites array
// ==================================================
const CardTopSprite Sprites::suits[6] {
   CardTopSprite(clubsBits),
   CardTopDualSprite(diamondsBits).even,
   CardTopDualSprite(heartsBits).even,
   CardTopSprite(spadesBits),
   CardTopDualSprite(diamondsBits).odd,
   CardTopDualSprite(heartsBits).odd
};

// ==================================================
//    rank sprites array
// ==================================================
const CardTopSprite Sprites::ranks[13] {
  // ace
  {
    " WWWWWWWWWWWWW",
    "WWWWW WWWWWWWW",
    "WWWW W WWWWWWW",
    "WWW WWW WWWWWW",
    "WW       WWWWW",
    "WW WWWWW WWWWW",
    "WW WWWWW WWWWW",
    "WW WWWWW WWWWW",
    "WWWWWWWWWWWWWW",
  },
  // two
  {
    " WWWWWWWWWWWWW",
    "WWW    WWWWWWW",
    "WW WWWW WWWWWW",
    "WWWWWWWW WWWWW",
    "WWWWWWW WWWWWW",
    "WWWW   WWWWWWW",
    "WW WWWWWWWWWWW",
    "WW       WWWWW",
    "WWWWWWWWWWWWWW",
  },
  // three
  {
    " WWWWWWWWWWWWW",
    "WWW    WWWWWWW",
    "WW WWWW WWWWWW",
    "WWWWWWWW WWWWW",
    "WWWWW   WWWWWW",
    "WWWWWWWW WWWWW",
    "WW WWWW WWWWWW",
    "WWW    WWWWWWW",
    "WWWWWWWWWWWWWW",
  },
  // four
  {
    " WWWWWWWWWWWWW",
    "WWWWWW  WWWWWW",
    "WWWWW W WWWWWW",
    "WWWW WW WWWWWW",
    "WWW WWW WWWWWW",
    "WW       WWWWW",
    "WWWWWWW WWWWWW",
    "WWWWWWW WWWWWW",
    "WWWWWWWWWWWWWW",
  },
  // five
  {
    " WWWWWWWWWWWWW",
    "WW      WWWWWW",
    "WW WWWWWWWWWWW",
    "WW     WWWWWWW",
    "WWWWWWW WWWWWW",
    "WWWWWWWW WWWWW",
    "WWW WWW WWWWWW",
    "WWWW   WWWWWWW",
    "WWWWWWWWWWWWWW",
  },
  // six
  {
    " WWWWWWWWWWWWW",
    "WWW     WWWWWW",
    "WW WWWWWWWWWWW",
    "WW WWWWWWWWWWW",
    "WW      WWWWWW",
    "WW WWWWW WWWWW",
    "WWW WWWW WWWWW",
    "WWWW    WWWWWW",
    "WWWWWWWWWWWWWW",
  },
  // seven
  {
    " WWWWWWWWWWWWW",
    "WW       WWWWW",
    "WWWWWWWW WWWWW",
    "WWWWWWW WWWWWW",
    "WWWWWW WWWWWWW",
    "WWWWW WWWWWWWW",
    "WWWW WWWWWWWWW",
    "WWWW WWWWWWWWW",
    "WWWWWWWWWWWWWW",
  },
  // eight
  {
    " WWWWWWWWWWWWW",
    "WWW     WWWWWW",
    "WW WWWWW WWWWW",
    "WW WWWWW WWWWW",
    "WWW     WWWWWW",
    "WW WWWWW WWWWW",
    "WW WWWWW WWWWW",
    "WWW     WWWWWW",
    "WWWWWWWWWWWWWW",
  },
  // nine
  {
    " WWWWWWWWWWWWW",
    "WWW     WWWWWW",
    "WW WWWWW WWWWW",
    "WW WWWWW WWWWW",
    "WWW      WWWWW",
    "WWWWWWWW WWWWW",
    "WWWWWWWW WWWWW",
    "WWW     WWWWWW",
    "WWWWWWWWWWWWWW",
  },
  // ten
  {
    " WWWWWWWWWWWWW",
    "WWW WWW  WWWWW",
    "WW  WW WW WWWW",
    "WWW WW WW WWWW",
    "WWW WW WW WWWW",
    "WWW WW WW WWWW",
    "WWW WW WW WWWW",
    "WWW WWW  WWWWW",
    "WWWWWWWWWWWWWW",
  },
  // jack
  {
    " WWWWWWWWWWWWW",
    "WWWWW   WWWWWW",
    "WWWWWW WWWWWWW",
    "WWWWWW WWWWWWW",
    "WWWWWW WWWWWWW",
    "WWWWWW WWWWWWW",
    "WW WWW WWWWWWW",
    "WWW   WWWWWWWW",
    "WWWWWWWWWWWWWW",
  },
  // queen
  {
    " WWWWWWWWWWWWW",
    "WWWW   WWWWWWW",
    "WW WWWW WWWWWW",
    "WW WWWW WWWWWW",
    "WW WWWW WWWWWW",
    "WW WW W WWWWWW",
    "WW WWW  WWWWWW",
    "WWW   WW WWWWW",
    "WWWWWWWWWWWWWW",
  },
  // king
  {
    " WWWWWWWWWWWWW",
    "WWW WWW WWWWWW",
    "WWW WW WWWWWWW",
    "WWW W WWWWWWWW",
    "WWW  WWWWWWWWW",
    "WWW  W WWWWWWW",
    "WWW WWW WWWWWW",
    "WWW WWWW WWWWW",
    "WWWWWWWWWWWWWW",
  },
};


const CardTopSprite Sprites::cursorLeft = {
  "              ",
  "              ",
  "              ",
  "           www",
  "           www",
  "           www",
  "              ",
  "              ",
  "              ",
};

const CardTopSprite Sprites::cursorRight = {
  "              ",
  "              ",
  "              ",
  "w             ",
  "w             ",
  "w             ",
  "              ",
  "              ",
  "              ",
};




__attribute__((noinline)) void Sprites::Initialize()
{
   const CardTopSprite *rankSprite = &ranks[0];
   for (uint8_t i=0; i < 13; ++i)
      spritesLookup.Set(i, rankSprite++);

   const CardTopSprite *suitSprite = &suits[0];
   for (uint8_t i=0; i < 6; ++i)
      spritesLookup.Set(13 + i, suitSprite++);
}


const CardTopSprite *Sprites::GetSprite(SpriteID spriteID)
{
   return spritesLookup.Get(spriteID.GetIndex());
}

// =========================================================
//    class SpriteID
// =========================================================

SpriteID SpriteID::FromRank(Rank rank)
{
   // ace is sprite zero
   return SpriteID((uint8_t)rank - (uint8_t)Rank::Ace);
}


SpriteID SpriteID::FromSuit(SuitOrdinal suitOrdinal, bool oddColors)
{
   uint8_t index = (uint8_t)suitOrdinal;
   if (oddColors)
      index += 4;

   // The basics of this lookup table... we have an array of sprites,
   // and these are the indices of the suits.  And:
   //   - the first suit in the list is 13
   //   - the normal (even byte address) colored suits are first
   //   - diamonds and hearts have different sprites for the odd colors
   static const uint8_t lookup[8] = {13, 14, 15, 16, 13, 17, 18, 16};
   return SpriteID(lookup[index]);
}

