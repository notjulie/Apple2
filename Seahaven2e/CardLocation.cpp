// Copyright 2022 Randy Rasmussen

#include "CardLocation.h"
#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include "Sprites.h"



/// \brief
///   Gets the byte offset of the location within the raster
///
uint8_t CardLocation::GetX() const {
  if (IsAce()) {
    switch (index) {
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
  } else if (IsTower()) {
    return GetColumnX(3 + index);
  } else if (IsNull()) {
    a2::puts("CARDLOCATION::GETX; NOWHERE");
    a2::PAGE2OFF();
    a2::MONITOR();
    return 0;
  } else {
    return GetColumnX(area.GetColumn());
  }
}

/// \brief
///   Gets the Y coordinate of the location
///
uint8_t CardLocation::GetY() const {
  if (IsTower() || IsAce()) {
    return CardLocations::TowersTop;
  } else if (IsColumn()) {
    return columnYLookup.Y(index);
  } else {
    a2::puts("CARDLOCATION::GETY; NOWHERE");
    a2::PAGE2OFF();
    a2::MONITOR();
    return 0;
  }
}


CardLocation CardLocation::Up() const
{
  CardLocation result = *this;

  if (IsColumn())
  {
    switch (area.GetColumn())
    {
    case 0:
    case 1:
    case 2:
    case 7:
    case 8:
    case 9:
      if (index > 0)
        --result.index;
      break;

    case 3:
    case 4:
    case 5:
    case 6:
      if (index > 0) {
        --result.index;
      } else {
        result.area = CardArea::Towers();
        result.index = area.GetColumn() - 3;
      }
      break;

    default:
      break;
    }
  }

  return result;
}

