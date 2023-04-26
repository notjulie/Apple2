// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_CARD_H_
#define SEAHAVEN2E_CARD_H_

#include <stdint.h>
#include "Rank.h"
#include "Suit.h"


/** \brief
 * Represents a card in our game table
 */
class Card {
 public:
  Card() : rank(Rank::Null) {}
  explicit Card(uint8_t cardNumber);

  inline Rank GetRank() const { return rank; }
  inline Suit GetSuit() const { return suit; }
  inline bool IsNull() const { return rank == Rank::Null; }

 private:
  Suit suit;
  Rank rank;
};


#endif  // SEAHAVEN2E_CARD_H_
