// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Card.h"


Card::Card(uint8_t cardNumber) {
  suit = (Suit)(cardNumber & 3);
  rank = (Rank)(1 + (cardNumber >> 2));
}
