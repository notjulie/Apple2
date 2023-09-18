// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Sprites.h"

c6502::Lookup16Bit<const CardTopSprite *, 13> Sprites::ranksLookup;

// club
const CardTopSprite clubsSprite = {
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

// diamond
const CardTopDualSprite diamondsSprite = {
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

// heart
const CardTopDualSprite heartsSprite = {
    "WWWWWWWWWWWW  ",
    "WWWRRRWWRRWWW ",
    "WRRRRRRRRRRRW ",
    "WWRRRRRRRRRWW ",
    "WWWRRRRRRRWWW ",
    "WWWWRRRRRWWWW ",
    "WWWWWRRRWWWWW ",
    "WWWWWWWWWWWWW ",
    "WWWWWWWWWWWWW "
  };

// spade
const CardTopSprite spadesSprite = {
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
      ranksLookup.Set(i, rankSprite++);
}


const CardTopSprite &Sprites::GetSuitSprite(Suit suit, bool oddColors)
{
   switch (suit.GetOrdinal())
   {
   case 0:
      return clubsSprite;
   case 1:
      return diamondsSprite.GetSprite(oddColors);
   case 2:
      return heartsSprite.GetSprite(oddColors);
   case 3:
   default:
      return spadesSprite;
   }
}
