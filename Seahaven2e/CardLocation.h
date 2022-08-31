
#ifndef CARDLOCATION_H
#define CARDLOCATION_H

#include <stdint.h>

class CardLocation {
public:
   enum class Area {
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

public:
   inline CardLocation() : area(Area::Nowhere), index(0) { }

   inline Area GetArea() const { return area; }
   inline uint8_t GetIndex() const { return index; }
   inline bool IsNull() const { return area == Area::Nowhere; }

   static inline CardLocation AcePile(Suit suit) { return CardLocation(Area::AcePiles, (uint8_t)suit); }
   static inline CardLocation Tower(uint8_t index) { return CardLocation(Area::Towers, index); }

private:
   inline CardLocation(Area _area, uint8_t _index) : area(_area), index(_index) {}

private:
   Area area;
   uint8_t index;
};

#endif // CARDLOCATION_H
