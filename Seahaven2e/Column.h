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
  inline Card GetCard(uint8_t index) const { return cards[index]; }
  void SetCard(uint8_t index, Card card) { cards[index] = card; }
  int8_t GetBottomCardRow();

 private:
  Card  cards[5];
};


#endif
