
#include "Sprites.h"


const CardTopSprite suits[4] {
   // club
   {
      "WWWWWWWWWWWW  ",
      "WWWWW  WWWWWW ",
      "WWWW    WWWWW ",
      "WWWWW  WWWWWW ",
      "WW        WWW ",
      "WW        WWW ",
      "WWWWW  WWWWWW ",
      "WWWW    WWWWW ",
      "WWWWWWWWWWWWW ",
   },

   // diamond
   {
      "WWWWWWWWWWWW  ",
      "WWWWWRRRWWWWW ",
      "WWWWRRRRRWWWW ",
      "WWWRRRRRRRWWW ",
      "WWRRRRRRRRRWW ",
      "WWWRRRRRRRWWW ",
      "WWWWRRRRRWWWW ",
      "WWWWWRRRWWWWW ",
      "WWWWWWWWWWWWW ",
   },

   // heart
   {
      "WWWWWWWWWWWW  ",
      "WWWRRRWWRRWWW ",
      "WRRRRRRRRRRRW ",
      "WWRRRRRRRRRWW ",
      "WWWRRRRRRRWWW ",
      "WWWWRRRRRWWWW ",
      "WWWWWRRRWWWWW ",
      "WWWWWWWWWWWWW ",
      "WWWWWWWWWWWWW ",
   },

   // spade
   {
      "WWWWWWWWWWWW  ",
      "WWWWW  WWWWWW ",
      "WWWW    WWWWW ",
      "WW        WWW ",
      "W          WW ",
      "WW        WWW ",
      "WWWWW  WWWWWW ",
      "WWWW    WWWWW ",
      "WWWWWWWWWWWWW ",
   }
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
