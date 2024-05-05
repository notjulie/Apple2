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
__attribute__((noinline)) uint8_t CardLocation::GetX() const
{
   static uint8_t aceColumns[4] = {0, 1, 8, 9};
   if (IsAce())
   {
      return GetColumnX(aceColumns[GetAceSuitOrdinal()]);
   }
   else if (IsTower())
   {
      return GetColumnX(3 + GetTowerIndex());
   }
   else if (IsNull())
   {
      assert(0);
      return 0;
   }
   else
   {
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
/// Returns the height of the card top to be drawn at this location;
/// must be a column location.  For columns with lots of cards on them
/// we scrunch the cards at the bottom a little so that they can all
/// appear onscreen, thus the card tops are not constant height.
/// </summary>
uint8_t CardLocation::GetCardTopHeight() const
{
   assert(IsColumn());
   return columnYLookup.Height(GetRow());
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


/// <summary>
/// Returns a CardLocation instance given its numeric value.  This
/// is useful for enumerating all locations by iterating from 0 to 255.
/// As a result, part of our contract is to return Null if the uint8_t
/// value is not valid.
/// </summary>
__attribute__((noinline)) CardLocation CardLocation::FromUint8(uint8_t i)
{
   // create the result
   CardLocation result(i);

   // validate
   static const uint8_t IndexCount[16] = {
      // max indices for the 10 columns
      16,16,16,16,16,
      16,16,16,16,16,

      // regions 10, 11, 12, and 13 are all null areas
      0,0,0,0,

      // regions 14 & 15 (towers and aces)
      4,4
   };

   if (result.value.parts.index >= IndexCount[result.value.parts.region])
      return CardLocation::Null();
   return result;
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
