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
//      enum class CardArea
// ==========================================================
// ==========================================================

/// <summary>
/// The type of area; CardArea is defined by one of these and a
/// column index in the case of a column
/// </summary>
enum class CardAreaType {
  Nowhere,
  AcePiles,
  Towers,
  Columns
};


// ==========================================================
// ==========================================================
//      enum class CardArea
// ==========================================================
// ==========================================================

class CardArea {
public:
  constexpr CardArea(CardAreaType areaType, uint8_t column) : areaType((uint8_t)areaType), column(column) {}

  uint8_t GetColumn() const { return column; }
  CardAreaType GetType() const { return (CardAreaType)areaType; }
  bool IsNowhere() const { return areaType == (uint8_t)CardAreaType::Nowhere; }

  static constexpr CardArea Nowhere() { return CardArea(CardAreaType::Nowhere, 0); }
  static constexpr CardArea AcePiles() { return CardArea(CardAreaType::AcePiles, 0); }
  static constexpr CardArea Column(uint8_t column) { return CardArea(CardAreaType::Columns, column); }
  static constexpr CardArea Towers() { return CardArea(CardAreaType::Towers, 0); }

private:
  uint8_t areaType : 3;
  uint8_t column : 4;
};

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
  constexpr CardLocation(CardArea area, uint8_t index) : area(area), index(index) { }

  inline CardArea GetArea() const { return area; }
  inline uint8_t GetIndex() const { return index; }
  inline bool IsNull() const { return area.IsNowhere(); }

  uint8_t GetX() const;
  uint8_t GetY() const;

  CardLocation Up() const;

  static inline CardLocation AcePile(Suit suit) {
    return CardLocation(CardArea::AcePiles(), (uint8_t)suit); }
  static inline CardLocation Column(uint8_t column, uint8_t index) {
    return CardLocation(CardArea::Column(column), index); }
  static inline CardLocation Tower(uint8_t index) {
     return CardLocation(CardArea::Towers(), index); }
  static constexpr CardLocation Null() {
    return CardLocation(CardArea::Nowhere(), 0);
  }

 private:
  static constexpr uint8_t GetColumnX(uint8_t column) { return column << 2; }

 private:
  CardArea area;
  uint8_t index;

//friend CardLocation operator+(CardLocation location, int8_t i);
};


#endif  // SEAHAVEN2E_CARDLOCATION_H_
