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
//    raw definitions of the rank sprites
// ==================================================
constexpr std::initializer_list<const char *> aceBits = {
    " WWWWWWWWWWWWW",
    "WWWWW WWWWWWWW",
    "WWWW W WWWWWWW",
    "WWW WWW WWWWWW",
    "WW       WWWWW",
    "WW WWWWW WWWWW",
    "WW WWWWW WWWWW",
    "WW WWWWW WWWWW",
    "WWWWWWWWWWWWWW",
};
constexpr std::initializer_list<const char *> twoBits = {
    " WWWWWWWWWWWWW",
    "WWW    WWWWWWW",
    "WW WWWW WWWWWW",
    "WWWWWWWW WWWWW",
    "WWWWWWW WWWWWW",
    "WWWW   WWWWWWW",
    "WW WWWWWWWWWWW",
    "WW       WWWWW",
    "WWWWWWWWWWWWWW",
};
constexpr std::initializer_list<const char *> threeBits = {
    " WWWWWWWWWWWWW",
    "WWW    WWWWWWW",
    "WW WWWW WWWWWW",
    "WWWWWWWW WWWWW",
    "WWWWW   WWWWWW",
    "WWWWWWWW WWWWW",
    "WW WWWW WWWWWW",
    "WWW    WWWWWWW",
    "WWWWWWWWWWWWWW",
};
constexpr std::initializer_list<const char *> fourBits = {
    " WWWWWWWWWWWWW",
    "WWWWWW  WWWWWW",
    "WWWWW W WWWWWW",
    "WWWW WW WWWWWW",
    "WWW WWW WWWWWW",
    "WW       WWWWW",
    "WWWWWWW WWWWWW",
    "WWWWWWW WWWWWW",
    "WWWWWWWWWWWWWW",
};
constexpr std::initializer_list<const char *> fiveBits = {
    " WWWWWWWWWWWWW",
    "WW      WWWWWW",
    "WW WWWWWWWWWWW",
    "WW     WWWWWWW",
    "WWWWWWW WWWWWW",
    "WWWWWWWW WWWWW",
    "WWW WWW WWWWWW",
    "WWWW   WWWWWWW",
    "WWWWWWWWWWWWWW",
};
constexpr std::initializer_list<const char *> sixBits = {
    " WWWWWWWWWWWWW",
    "WWW     WWWWWW",
    "WW WWWWWWWWWWW",
    "WW WWWWWWWWWWW",
    "WW      WWWWWW",
    "WW WWWWW WWWWW",
    "WWW WWWW WWWWW",
    "WWWW    WWWWWW",
    "WWWWWWWWWWWWWW",
};
constexpr std::initializer_list<const char *> sevenBits = {
    " WWWWWWWWWWWWW",
    "WW       WWWWW",
    "WWWWWWWW WWWWW",
    "WWWWWWW WWWWWW",
    "WWWWWW WWWWWWW",
    "WWWWW WWWWWWWW",
    "WWWW WWWWWWWWW",
    "WWWW WWWWWWWWW",
    "WWWWWWWWWWWWWW",
};
constexpr std::initializer_list<const char *> eightBits = {
    " WWWWWWWWWWWWW",
    "WWW     WWWWWW",
    "WW WWWWW WWWWW",
    "WW WWWWW WWWWW",
    "WWW     WWWWWW",
    "WW WWWWW WWWWW",
    "WW WWWWW WWWWW",
    "WWW     WWWWWW",
    "WWWWWWWWWWWWWW",
};
constexpr std::initializer_list<const char *> nineBits = {
    " WWWWWWWWWWWWW",
    "WWW     WWWWWW",
    "WW WWWWW WWWWW",
    "WW WWWWW WWWWW",
    "WWW      WWWWW",
    "WWWWWWWW WWWWW",
    "WWWWWWWW WWWWW",
    "WWW     WWWWWW",
    "WWWWWWWWWWWWWW",
};
constexpr std::initializer_list<const char *> tenBits = {
    " WWWWWWWWWWWWW",
    "WWW WWW  WWWWW",
    "WW  WW WW WWWW",
    "WWW WW WW WWWW",
    "WWW WW WW WWWW",
    "WWW WW WW WWWW",
    "WWW WW WW WWWW",
    "WWW WWW  WWWWW",
    "WWWWWWWWWWWWWW",
};
constexpr std::initializer_list<const char *> jackBits = {
    " WWWWWWWWWWWWW",
    "WWWWW   WWWWWW",
    "WWWWWW WWWWWWW",
    "WWWWWW WWWWWWW",
    "WWWWWW WWWWWWW",
    "WWWWWW WWWWWWW",
    "WW WWW WWWWWWW",
    "WWW   WWWWWWWW",
    "WWWWWWWWWWWWWW",
};
constexpr std::initializer_list<const char *> queenBits = {
    " WWWWWWWWWWWWW",
    "WWWW   WWWWWWW",
    "WW WWWW WWWWWW",
    "WW WWWW WWWWWW",
    "WW WWWW WWWWWW",
    "WW WW W WWWWWW",
    "WW WWW  WWWWWW",
    "WWW   WW WWWWW",
    "WWWWWWWWWWWWWW",
};
constexpr std::initializer_list<const char *> kingBits = {
    " WWWWWWWWWWWWW",
    "WWW WWW WWWWWW",
    "WWW WW WWWWWWW",
    "WWW W WWWWWWWW",
    "WWW  WWWWWWWWW",
    "WWW  W WWWWWWW",
    "WWW WWW WWWWWW",
    "WWW WWWW WWWWW",
    "WWWWWWWWWWWWWW",
};

// ==================================================
//    cardTopSprites array
// ==================================================
const CardTopSprite Sprites::cardTopSprites[19] {
   CardTopSprite(aceBits),
   CardTopSprite(twoBits),
   CardTopSprite(threeBits),
   CardTopSprite(fourBits),
   CardTopSprite(fiveBits),
   CardTopSprite(sixBits),
   CardTopSprite(sevenBits),
   CardTopSprite(eightBits),
   CardTopSprite(nineBits),
   CardTopSprite(tenBits),
   CardTopSprite(jackBits),
   CardTopSprite(queenBits),
   CardTopSprite(kingBits),
   CardTopSprite(clubsBits),
   CardTopDualSprite(diamondsBits).even,
   CardTopDualSprite(heartsBits).even,
   CardTopSprite(spadesBits),
   CardTopDualSprite(diamondsBits).odd,
   CardTopDualSprite(heartsBits).odd
};


// ==================================================
//    cursor sprites
// ==================================================
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
   const CardTopSprite *sprite = &cardTopSprites[0];
   for (uint8_t i=0; i < 19; ++i)
      spritesLookup.Set(i, sprite++);
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

