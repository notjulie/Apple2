// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Column.h"
#include <Apple2Lib/ROM.h>


void Column::SetCard(uint8_t row, Card card)
{
  // the argument shouldn't be null, RemoveCard is for that
  if (card.IsNull()) {
    a2::puts("SETCARD is null");
    return;
  }

  // if the row is 5 or greater the card must be one less than
  // the card above it
  if (row >= 5) {
    if (card != (GetCard(row - 1) - 1)) {
      a2::puts("SETCARD is wrong");
      return;
    }

    if (row >= count)
      count = row + 1;
    return;
  }

  // else just set it
  cards[row] = card;
  if (row >= count)
    count = row + 1;
}
