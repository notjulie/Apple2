// Copyright 2022 Randy Rasmussen

#include "CardLocation.h"
#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include "Sprites.h"



/// \brief
///   Gets the byte offset of the location within the raster
///
uint8_t CardLocation::GetX() const {
  switch (area) {
  case CardArea::Nowhere:
    a2::puts("CARDLOCATION::GETX; NOWHERE");
    a2::PAGE2OFF();
    a2::MONITOR();
    return 0;

  case CardArea::AcePiles:
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

  case CardArea::Towers:
    return GetColumnX(3 + index);

  default:
    return GetColumnX((uint8_t)area - (uint8_t)CardArea::Column1);
  }
}

/// \brief
///   Gets the Y coordinate of the location
///
uint8_t CardLocation::GetY() const {
  switch (area) {
  case CardArea::Nowhere:
    a2::puts("CARDLOCATION::GETY; NOWHERE");
    a2::PAGE2OFF();
    a2::MONITOR();
    return 0;

  case CardArea::AcePiles:
  case CardArea::Towers:
    return CardLocations::TowersTop;

  case CardArea::Column1:
  case CardArea::Column2:
  case CardArea::Column3:
  case CardArea::Column4:
  case CardArea::Column5:
  case CardArea::Column6:
  case CardArea::Column7:
  case CardArea::Column8:
  case CardArea::Column9:
  case CardArea::Column10:
    return columnYLookup.Y(index);

  default:
    a2::puts("CARDLOCATION::GETY; DEFAULT");
    a2::PAGE2OFF();
    a2::MONITOR();
    return 0;
  }
}


CardLocation CardLocation::Up() const
{
  CardLocation result = *this;

  switch (area)
  {
  case CardArea::Column1:
  case CardArea::Column2:
  case CardArea::Column3:
  case CardArea::Column8:
  case CardArea::Column9:
  case CardArea::Column10:
    if (index > 0)
      --result.index;
    break;

  case CardArea::Column4:
  case CardArea::Column5:
  case CardArea::Column6:
  case CardArea::Column7:
    if (index > 0) {
      --result.index;
    } else {
      result.area = CardArea::Towers;
      result.index = area - CardArea::Column4;
    }
    break;

  default:
    break;
  }

  return result;
}
