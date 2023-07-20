// Copyright 2022-2023 Randy Rasmussen

#include "CardLocation.h"
#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/ROM.h>
#include "Sprites.h"

/// <summary>
/// Creates a CardLocation representing the given column and row
/// </summary>
CardLocation CardLocation::Column(uint8_t column, uint8_t row) {
  // this only works if the maximum column cards is no greater than 16
  static_assert(CardLocations::MaxColumnCards <= 16, "column index won't fit in four bits");
  return CardLocation(((column + 1) << 4) | row);
}


/// <summary>
/// Gets the row associated with a column location
/// </summary>
uint8_t CardLocation::GetRow() const {
  // this only works if the maximum column cards is no greater than 16
  static_assert(CardLocations::MaxColumnCards <= 16, "column index won't fit in four bits");
  return locationNumber & 0x0F;
}


/// <summary>
/// Gets the column associated with a column location
/// </summary>
uint8_t CardLocation::GetColumn() const {
  // this only works if the maximum column cards is no greater than 16
  static_assert(CardLocations::MaxColumnCards <= 16, "column index won't fit in four bits");
  return (locationNumber>>4) - 1;
}


/// \brief
///   Gets the byte offset of the location within the raster
///
uint8_t CardLocation::GetX() const {
  if (IsAce()) {
    switch (GetAceSuit()) {
    case Suit::Clubs:
      return GetColumnX(0);
    case Suit::Diamonds:
      return GetColumnX(1);
    case Suit::Hearts:
      return GetColumnX(8);
    case Suit::Spades:
      return GetColumnX(9);
    }
    a2::puts("CARDLOCATION::GETX; ACEPILE");
    a2::PAGE2OFF();
    a2::MONITOR();
    return 0;
  } else if (IsTower()) {
    return GetColumnX(3 + GetTowerIndex());
  } else if (IsNull()) {
    a2::puts("CARDLOCATION::GETX; NOWHERE");
    a2::PAGE2OFF();
    a2::MONITOR();
    return 0;
  } else {
    return GetColumnX(GetColumn());
  }
}

/// \brief
///   Gets the Y coordinate of the location
///
uint8_t CardLocation::GetY() const {
  if (IsTower() || IsAce()) {
    return CardLocations::TowersTop;
  } else if (IsColumn()) {
    return columnYLookup.Y(GetRow());
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
    uint8_t column = GetColumn();
    uint8_t row = GetRow();

    switch (column)
    {
    case 0:
    case 1:
    case 2:
    case 7:
    case 8:
    case 9:
      if (row > 0)
        result = Column(column, row - 1);
      break;

    case 3:
    case 4:
    case 5:
    case 6:
      if (row > 0) {
        result = Column(column, row - 1);
      } else {
        result = Tower(column - 3);
      }
      break;

    default:
      break;
    }
  }

  return result;
}

