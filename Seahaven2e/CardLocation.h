// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_CARDLOCATION_H_
#define SEAHAVEN2E_CARDLOCATION_H_

#include <stdint.h>
#include "Sprites.h"
#include "Suit.h"


// ==========================================================
// ==========================================================
//      namespace CardLocations
// ==========================================================
// ==========================================================

namespace CardLocations {
  constexpr uint8_t CardShadowHeight = 1;
  constexpr uint8_t DistanceBetweenColumnCards = CardTopSpriteHeight + CardShadowHeight;
  constexpr uint8_t TowersTop = 3;
  constexpr uint8_t TowersBottom = TowersTop + CardHeight;
  constexpr uint8_t ColumnsTop = TowersBottom + 4;

  constexpr uint8_t CardByteWidth = 4;

  /// <summary>
  /// Starting column cards... 5, of course
  /// </summary>
  constexpr uint8_t StartingColumnCards = 5;

  /// <summary>
  /// Max column cards; it would be the case where the column starts with a king at
  /// the bottom and then cards are stacked on it all the way down to two
  /// </summary>
  constexpr uint8_t MaxColumnCards = StartingColumnCards + (Rank::King - Rank::Two);
}

// ==========================================================
// ==========================================================
//      class ColumnYLookup
// ==========================================================
// ==========================================================

/// <summary>
/// Handy constexpr lookup for the top of a column card so
/// that we don't have to multiply
/// </summary>
class ColumnYLookup {
public:
   constexpr ColumnYLookup() : y(), height() {
      // start with them evenly spaced
      for (int i=0; i < CardLocations::MaxColumnCards; ++i)
         y[i] =
            CardLocations::ColumnsTop +
            i * CardLocations::DistanceBetweenColumnCards;

      // scrunch some of the bottom rows so that the last row is visible
      const int rowsToScrunch = 6;
      for (int i=0; i<rowsToScrunch; ++i) {
         y[CardLocations::MaxColumnCards - 1 - i] -= (rowsToScrunch - i);
      }

      // use the result to create a lookup for height of exposed
      // card top
      for (int i=0; i<CardLocations::MaxColumnCards - 1; ++i)
         height[i] = y[i + 1] - y[i];
      height[CardLocations::MaxColumnCards - 1] = height[CardLocations::MaxColumnCards - 2];
   }

   inline uint8_t Y(uint8_t index) const { return y[index]; }
   inline uint8_t Height(uint8_t index) const { return height[index]; }

private:
   uint8_t y[CardLocations::MaxColumnCards];
   uint8_t height[CardLocations::MaxColumnCards];
};

constexpr ColumnYLookup columnYLookup;

// ==========================================================
// ==========================================================
//      class CardLocation
// ==========================================================
// ==========================================================

class CardLocation {
private:
   enum class Region : uint8_t {
      Tower = 15,
      Ace = 14,
      Null = 13
   };

public:
   CardLocation() = default;
   static CardLocation FromUint8(uint8_t i);

   uint8_t AsUint8() const { return value.location_number; }

   bool IsNull() const { return value.location_number == Null().value.location_number; }
   bool IsAce() const { return value.parts.region == (uint8_t)Region::Ace; }
   bool IsColumn() const { return value.parts.region < 10; }
   bool IsTower() const { return value.parts.region == (uint8_t)Region::Tower; }

   uint8_t GetAceSuitOrdinal() const { return value.parts.index; }
   uint8_t GetColumn() const;
   uint8_t GetRow() const;
   uint8_t GetTowerIndex() const { return value.parts.index; }

   uint8_t GetX() const;
   uint8_t GetY() const;
   uint8_t GetCardTopHeight() const;

   uint8_t GetGridColumn() const;
   uint8_t GetGridRow() const;

   // cursor movement helpers
   CardLocation Up() const;
   CardLocation Down() const;
   CardLocation Left() const;
   CardLocation Right() const;

   void Print() const;

   bool operator==(CardLocation location) const;
   bool operator!=(CardLocation location) const;

public:
   __attribute__((noinline)) static CardLocation AcePile(SuitOrdinal suitOrdinal) {  return CardLocation(Region::Ace, (uint8_t)suitOrdinal); }
   __attribute__((noinline)) static CardLocation Column(uint8_t column, uint8_t index) { return CardLocation((Region)column, index); }
   __attribute__((noinline)) static inline CardLocation Tower(uint8_t index) { return CardLocation(Region::Tower, index); }
   static constexpr CardLocation Null() { return CardLocation(Region::Null, 0); }

private:
   static constexpr uint8_t ColumnXValues[10] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36};

private:
   constexpr CardLocation(Region region, uint8_t index) : value() { value.parts.region = (uint8_t)region; value.parts.index = index; }
   constexpr CardLocation(uint8_t locationNumber) : value() { value.location_number = locationNumber; }
   static constexpr uint8_t GetColumnX(uint8_t column) { return ColumnXValues[column]; }

private:
   union {
      struct {
         uint8_t index : 4;
         uint8_t region : 4;
      } parts;
      uint8_t location_number;
   } value;
};
static_assert(sizeof(CardLocation)==1, "CardLocation is supposed to be small");

#endif  // SEAHAVEN2E_CARDLOCATION_H_
