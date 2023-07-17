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
  constexpr uint8_t MaxColumnCards = 16;
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
  constexpr ColumnYLookup() : y() {
    for (int i=0; i < CardLocations::MaxColumnCards; ++i)
      y[i] =
        CardLocations::ColumnsTop +
        i * CardLocations::DistanceBetweenColumnCards;
  }

  inline uint8_t Y(uint8_t index) const { return y[index]; }

 private:
  uint8_t y[CardLocations::MaxColumnCards];
};

constexpr ColumnYLookup columnYLookup;

// ==========================================================
// ==========================================================
//      class CardLocation
// ==========================================================
// ==========================================================

class CardLocation {
 public:
  bool IsNull() const { return locationNumber == 0; }
  bool IsAce() const { return locationNumber < 5; }
  bool IsColumn() const { return locationNumber>=5 && locationNumber <=251; }
  bool IsTower() const { return locationNumber > 251; }

  Suit GetAceSuit() const { return (Suit)(locationNumber - 1); }
  uint8_t GetColumn() const;
  uint8_t GetRow() const;
  uint8_t GetTowerIndex() const { return locationNumber ^ 0xFF; }

  uint8_t GetX() const;
  uint8_t GetY() const;

  CardLocation Up() const;

  static inline CardLocation AcePile(Suit suit) {
    return CardLocation(1 + (uint8_t)suit); }
  static CardLocation Column(uint8_t column, uint8_t index);
  static inline CardLocation Tower(uint8_t index) {
     return CardLocation(index ^ 0xFF); }
  static constexpr CardLocation Null() {
    return CardLocation(0); }

 private:
  constexpr CardLocation(uint8_t locationNumber) : locationNumber(locationNumber) {}
  static constexpr uint8_t GetColumnX(uint8_t column) { return column << 2; }

 private:
  uint8_t locationNumber;
};


#endif  // SEAHAVEN2E_CARDLOCATION_H_
