// Copyright 2022-2023 Randy Rasmussen

#include "CardLocation.h"
#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/ROM.h>
#include "SHAssert.h"
#include "Sprites.h"


/// <summary>
/// Gets the row associated with a column location
/// </summary>
uint8_t CardLocation::GetRow() const
{
   // this only works if the maximum column cards is no greater than 16
   static_assert(CardLocations::MaxColumnCards <= 16, "column index won't fit in four bits");
   return value.parts.index;
}


bool CardLocation::operator==(CardLocation location) const
{
   return value.location_number == location.value.location_number;
}


bool CardLocation::operator!=(CardLocation location) const
{
   return value.location_number != location.value.location_number;
}


uint8_t CardLocation::GetGridRow() const
{
   if (IsColumn())
      return GetRow() + 1;
   else
      return 0;
}


/// <summary>
/// Gets the column associated with a column location
/// </summary>
uint8_t CardLocation::GetColumn() const
{
   // this only works if the maximum column cards is no greater than 16
   static_assert(CardLocations::MaxColumnCards <= 16, "column index won't fit in four bits");
   return value.parts.region;
}


/// \brief
///   Gets the byte offset of the location within the raster
///
__attribute__((noinline)) uint8_t CardLocation::GetX() const {
   if (IsAce()) {
      switch (GetAceSuitOrdinal()) {
      case Suit::Clubs().GetOrdinal():
         return GetColumnX(0);
      case Suit::Diamonds().GetOrdinal():
         return GetColumnX(1);
      case Suit::Hearts().GetOrdinal():
         return GetColumnX(8);
      case Suit::Spades().GetOrdinal():
         return GetColumnX(9);
      default:
         assert(0);
         return 0;
      }
   } else if (IsTower()) {
      return GetColumnX(3 + GetTowerIndex());
   } else if (IsNull()) {
      assert(0);
      return 0;
   } else {
      return GetColumnX(GetColumn());
   }
}

/// \brief
///   Gets the Y coordinate of the location
///
__attribute__((noinline)) uint8_t CardLocation::GetY() const {
   if (IsTower() || IsAce())
   {
      return CardLocations::TowersTop;
   } else if (IsColumn())
   {
      return columnYLookup.Y(GetRow());
   } else
   {
      assert(0);
      return 0;
   }
}


/// <summary>
/// Returns the location up from this location (as if the user had hit the up arrow)
/// </summary>
__attribute__((noinline)) CardLocation CardLocation::Up() const
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


/// <summary>
/// Returns the location down from this location (as if the user had hit the down arrow)
/// </summary>
__attribute__((noinline)) CardLocation CardLocation::Down() const
{
   if (IsColumn())
   {
      uint8_t column = GetColumn();
      uint8_t row = GetRow();
      if (row < CardLocations::MaxColumnCards - 1)
         ++row;
      return Column(column, row);
   }
   else if (IsTower())
   {
      return Column(GetTowerIndex() + 3, 0);
   }
   else
   {
      return CardLocation::Null();
   }
}


/// <summary>
/// Returns the location left from this location (as if the user had hit the left arrow)
/// </summary>
__attribute__((noinline)) CardLocation CardLocation::Left() const
{
   if (IsColumn())
   {
      uint8_t column = GetColumn();
      uint8_t row = GetRow();
      if (column > 0)
         return Column(column - 1, row);
   }
   else if (IsTower())
   {
      uint8_t towerIndex;
      if ((towerIndex = GetTowerIndex()) > 0)
         return Tower(towerIndex - 1);
   }

   return *this;
}


/// <summary>
/// Returns the location right from this location (as if the user had hit the right arrow)
/// </summary>
__attribute__((noinline)) CardLocation CardLocation::Right() const
{
   if (IsColumn())
   {
      uint8_t column = GetColumn();
      uint8_t row = GetRow();
      if (column < 9)
         return Column(column + 1, row);
   }
   else if (IsTower())
   {
      uint8_t towerIndex;
      if ((towerIndex = GetTowerIndex()) < 3)
         return Tower(towerIndex + 1);
   }

   return *this;
}


void CardLocation::Print() const
{
   if (IsNull())
   {
      a2::COUT('X');
   }
   else if (IsTower())
   {
      a2::COUT('T');
      a2::COUT('0' + GetTowerIndex());
   }
   else if (IsColumn())
   {
      a2::COUT('C');
      a2::COUT('0' + GetColumn());
      a2::COUT('0' + GetRow());
   }
   else if (IsAce())
   {
      a2::COUT('A');
      a2::COUT('0' + GetAceSuitOrdinal());
   }
}
