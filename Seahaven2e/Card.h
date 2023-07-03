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
friend class CompactCard;

 public:
  Card() : rank(Rank::Null) {}

  inline Rank GetRank() const { return rank; }
  inline Suit GetSuit() const { return suit; }
  inline bool IsNull() const { return rank == Rank::Null; }

  Card &operator-=(int i);
  bool operator==(Card c) const { return c.suit==suit && c.rank==rank; }
  bool operator!=(Card c) const { return !operator==(c); }

  static Card FromOrdinal(uint8_t ordinal);

 private:
  Suit suit;
  Rank rank;
};

Card operator-(Card card, uint8_t i);


class CompactCard {
public:
  CompactCard() = default;
  CompactCard(Card card);
  operator Card() const;

private:
  uint8_t rank : 4;
  uint8_t suit : 2;
};

#endif  // SEAHAVEN2E_CARD_H_
