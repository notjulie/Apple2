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

  Card &operator-=(int i);
  bool operator==(Card c) const { return c.suit==suit && c.rank==rank; }
  bool operator!=(Card c) const { return !operator==(c); }

 private:
  Suit suit;
  Rank rank;
};

Card operator-(Card card, uint8_t i);

#endif  // SEAHAVEN2E_CARD_H_
