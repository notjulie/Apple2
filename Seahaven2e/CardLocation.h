
#ifndef CARDLOCATION_H
#define CARDLOCATION_H

#include <stdint.h>
#include "Sprites.h"
#include "Suit.h"


const uint8_t DistanceBetweenColumnCards = CardTopSpriteHeight + 1;

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

class CardLocation {
public:
   static const uint8_t TowersTop = 3;
   static const uint8_t TowersBottom = CardLocation::TowersTop + CardHeight;
   static const uint8_t ColumnsTop = TowersBottom + 4;

public:
   inline CardLocation() : area(CardArea::Nowhere), index(0) { }

   inline CardArea GetArea() const { return area; }
   inline uint8_t GetIndex() const { return index; }
   inline bool IsNull() const { return area == CardArea::Nowhere; }

   uint8_t GetX() const;
   uint8_t GetY() const;

   static inline CardLocation AcePile(Suit suit) { return CardLocation(CardArea::AcePiles, (uint8_t)suit); }
   static inline CardLocation Column(uint8_t column, uint8_t index) { return CardLocation(CardArea::Column1 + column, index); }
   static inline CardLocation Tower(uint8_t index) { return CardLocation(CardArea::Towers, index); }

private:
   inline CardLocation(CardArea _area, uint8_t _index) : area(_area), index(_index) {}
   static constexpr uint8_t GetColumnX(uint8_t column) { return column << 2; }

private:
   CardArea area;
   uint8_t index;
};

#endif // CARDLOCATION_H
