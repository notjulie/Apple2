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
  constexpr uint8_t DistanceBetweenColumnCards = CardTopSpriteHeight + 1;
  constexpr uint8_t TowersTop = 3;
  constexpr uint8_t TowersBottom = TowersTop + CardHeight;
  constexpr uint8_t ColumnsTop = TowersBottom + 4;
  constexpr uint8_t MaxColumnCards = 16;
}

// ==========================================================
// ==========================================================
//      enum class CardArea
// ==========================================================
// ==========================================================

enum class CardArea {
  Nowhere,
  AcePiles,
  Towers,
  Column1,
  Column2,
  Column3,
  Column4,
  Column5,
  Column6,
  Column7,
  Column8,
  Column9,
  Column10
};

inline CardArea operator+(CardArea area, uint8_t addend) {
  return (CardArea)((uint8_t)area + addend);
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
  inline CardLocation() : area(CardArea::Nowhere), index(0) { }

  inline CardArea GetArea() const { return area; }
  inline uint8_t GetIndex() const { return index; }
  inline bool IsNull() const { return area == CardArea::Nowhere; }

  uint8_t GetX() const;
  uint8_t GetY() const;

  static inline CardLocation AcePile(Suit suit) {
    return CardLocation(CardArea::AcePiles, (uint8_t)suit); }
  static inline CardLocation Column(uint8_t column, uint8_t index) {
    return CardLocation(CardArea::Column1 + column, index); }
  static inline CardLocation Tower(uint8_t index) {
     return CardLocation(CardArea::Towers, index); }

 private:
  inline CardLocation(CardArea _area, uint8_t _index)
    : area(_area), index(_index) {}
  static constexpr uint8_t GetColumnX(uint8_t column) { return column << 2; }

 private:
  CardArea area;
  uint8_t index;
};

#endif  // SEAHAVEN2E_CARDLOCATION_H_
