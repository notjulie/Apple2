// Copyright 2022 Randy Rasmussen

#include "CardLocation.h"
#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include "Sprites.h"



/// \brief
///   Gets the byte offset of the location within the raster
///
uint8_t CardLocation::GetX() const
{
   switch (area)
   {
   case Area::Nowhere:
      a2::puts("CARDLOCATION::GETX; NOWHERE");
      a2::PAGE2OFF();
      a2::MONITOR();
      return 0;

   case Area::AcePiles:
      switch (index)
      {
      case 0:
         return GetColumnX(0);
      case 1:
         return GetColumnX(1);
      case 2:
         return GetColumnX(8);
      case 3:
         return GetColumnX(9);
      }
      a2::puts("CARDLOCATION::GETX; ACEPILE");
      a2::PAGE2OFF();
      a2::MONITOR();
      return 0;

   case Area::Towers:
      return GetColumnX(3 + index);

   default:
      return GetColumnX((uint8_t)area - (uint8_t)Area::Column1);
   }
}

/// \brief
///   Gets the Y coordinate of the location
///
uint8_t CardLocation::GetY() const
{
   switch (area)
   {
   case Area::Nowhere:
      a2::puts("CARDLOCATION::GETY; NOWHERE");
      a2::PAGE2OFF();
      a2::MONITOR();
      return 0;

   case Area::AcePiles:
   case Area::Towers:
      return TowersTop;

   default:
      a2::puts("CARDLOCATION::GETY; DEFAULT");
      a2::PAGE2OFF();
      a2::MONITOR();
      return 0;
   }
}


