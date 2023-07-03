// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef _COLUMN_H
#define _COLUMN_H

#include "Card.h"

/** \brief
 * Represents a column on the game board
 */
class Column {
 public:
  void Clear() { count = 0; }
  void Append(Card card) { SetCard(count, card); }

  inline Card GetCard(uint8_t index) const { return cards[index]; }
  uint8_t GetCount() const { return count; }

  void SetCard(uint8_t row, Card card);

 private:
  Card  cards[5];
  uint8_t count;
};


#endif