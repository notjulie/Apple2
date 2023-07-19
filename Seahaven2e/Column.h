// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef _COLUMN_H
#define _COLUMN_H

#include "Card.h"
#include "CardLocation.h"

/** \brief
 * Represents a column on the game board
 */
class Column {
 public:
  constexpr Column() {}

  void Clear() { count = 0; }
  void Append(CompactCard card) { SetCard(count, card); }

  CompactCard GetCard(uint8_t index) const;
  uint8_t GetCount() const { return count; }

  void SetCard(uint8_t row, CompactCard card);

  int8_t GetCardIndex(CompactCard card);

 private:
  CompactCard  cards[5];
  uint8_t count = 0;
};


#endif
