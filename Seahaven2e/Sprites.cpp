
#include "Sprites.h"


const CardTopSprite suits[4] {
   // club
   {
      "WWWWWWWWWWWWW ",
      "WWWWWW  WWWWWW",
      "WWWWW    WWWWW",
      "WWWWWW  WWWWWW",
      "WWW        WWW",
      "WWW        WWW",
      "WWWWWW  WWWWWW",
      "WWWWW    WWWWW",
      "WWWWWWWWWWWWWW",
   },

   // diamond
   {
      "WWWWWWWWWWWWW ",
      "WWWWWRRRWWWWWW",
      "WWWWRRRRRWWWWW",
      "WWWRRRRRRRWWWW",
      "WWRRRRRRRRRWWW",
      "WWWRRRRRRRWWWW",
      "WWWWRRRRRWWWWW",
      "WWWWWRRRWWWWWW",
      "WWWWWWWWWWWWWW",
   },

   // heart
   {
      "WWWWWWWWWWWWW ",
      "WWWRRRWWRRWWWW",
      "WRRRRRRRRRRRWW",
      "WWRRRRRRRRRWWW",
      "WWWRRRRRRRWWWW",
      "WWWWRRRRRWWWWW",
      "WWWWWRRRWWWWWW",
      "WWWWWWWWWWWWWW",
   },

   // spade
   {
      "WWWWWWWWWWWWW ",
      "WWWWWW  WWWWWW",
      "WWWWW    WWWWW",
      "WWW        WWW",
      "WW          WW",
      "WWW        WWW",
      "WWWWWW  WWWWWW",
      "WWWWW    WWWWW",
      "WWWWWWWWWWWWWW",
   }
};

const CardTopSprite ranks[13] {
   // ace
   {
      " WWWWWWWWWWWWW",
      "WWWWW  WWWWWWW",
      "WWWW WW WWWWWW",
      "WWW WWWW WWWWW",
      "WW        WWWW",
      "WW WWWWWW WWWW",
      "WW WWWWWW WWWW",
      "WW WWWWWW WWWW",
      "WWWWWWWWWWWWWW",
   },
   // two
   {
      " WWWWWWWWWWWWW",
      "WWW     WWWWWW",
      "WW WWWWW WWWWW",
      "WWWWWWWWW WWWW",
      "WWWWWWWW WWWWW",
      "WWWWW   WWWWWW",
      "WWW WWWWWWWWWW",
      "WW        WWWW",
      "WWWWWWWWWWWWWW",
   },
   // three
   {
      " WWWWWWWWWWWWW",
      "WWW     WWWWWW",
      "WW WWWWW WWWWW",
      "WWWWWWWWW WWWW",
      "WWWWWW   WWWWW",
      "WWWWWWWWW WWWW",
      "WW WWWWW WWWWW",
      "WWW     WWWWWW",
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
      "WW        WWWW",
      "WW WWWWWWWWWWW",
      "WW      WWWWWW",
      "WWWWWWWW WWWWW",
      "WWWWWWWWW WWWW",
      "WWW WWWW WWWWW",
      "WWWW    WWWWWW",
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
      "WW        WWWW",
      "WWWWWWWWW WWWW",
      "WWWWWWWW WWWWW",
      "WWWWWWW WWWWWW",
      "WWWWWW WWWWWWW",
      "WWWWW WWWWWWWW",
      "WWWWW WWWWWWWW",
      "WWWWWWWWWWWWWW",
   },
   // eight
   {
      " WWWWWWWWWWWWW",
      "WWW      WWWWW",
      "WW WWWWWW WWWW",
      "WW WWWWWW WWWW",
      "WWW      WWWWW",
      "WW WWWWWW WWWW",
      "WW WWWWWW WWWW",
      "WWW      WWWWW",
      "WWWWWWWWWWWWWW",
   },
   // nine
   {
      " WWWWWWWWWWWWW",
      "WWW      WWWWW",
      "WW WWWWWW WWWW",
      "WW WWWWWW WWWW",
      "WWW       WWWW",
      "WWWWWWWWW WWWW",
      "WWWWWWWWW WWWW",
      "WWW      WWWWW",
      "WWWWWWWWWWWWWW",
   },
   // ten
   {
      " WWWWWWWWWWWWW",
      "WWW WWW   WWWW",
      "WW  WW WWW WWW",
      "WWW WW WWW WWW",
      "WWW WW WWW WWW",
      "WWW WW WWW WWW",
      "WWW WW WWW WWW",
      "WWW WWW   WWWW",
      "WWWWWWWWWWWWWW",
   },
   // jack
   {
      " WWWWWWWWWWWWW",
      "WWWWWW   WWWWW",
      "WWWWWWW WWWWWW",
      "WWWWWWW WWWWWW",
      "WWWWWWW WWWWWW",
      "WWWWWWW WWWWWW",
      "WWW WWW WWWWWW",
      "WWWW   WWWWWWW",
      "WWWWWWWWWWWWWW",
   },
   // queen
   {
      " WWWWWWWWWWWWW",
      "WWWW    WWWWWW",
      "WW WWWWW WWWWW",
      "WW WWWWW WWWWW",
      "WW WWWWW WWWWW",
      "WW WWW W WWWWW",
      "WW WWWW  WWWWW",
      "WWW    WW WWWW",
      "WWWWWWWWWWWWWW",
   },
   // king
   {
      " WWWWWWWWWWWWW",
      "WWW WWWW WWWWW",
      "WWW WWW WWWWWW",
      "WWW WW WWWWWWW",
      "WWW   WWWWWWWW",
      "WWW  WW WWWWWW",
      "WWW WWWW WWWWW",
      "WWW WWWWW WWWW",
      "WWWWWWWWWWWWWW",
   },
};
