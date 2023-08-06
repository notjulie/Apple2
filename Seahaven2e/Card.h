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

  Card &operator-=(int8_t i);
  Card &operator+=(int8_t i);
  bool operator==(Card c) const { return c.suit==suit && c.rank==rank; }
  bool operator!=(Card c) const { return !operator==(c); }

 private:
  Suit suit;
  Rank rank;
};

Card operator-(Card card, int8_t i);
Card operator+(Card card, int8_t i);


/// <summary>
/// A single-byte representation of card that's handy for storage of card arrays
/// or for passing around as a function parameter.
/// </summary>
class CompactCard {
public:
   CompactCard() {}
   constexpr CompactCard(Suit suit, Rank rank)
      : cardNumber((uint8_t)rank + (uint8_t)suit.numericValue) {}
   explicit CompactCard(Card card);

   operator Card() const;

   Rank GetRank() const { return (Rank)(cardNumber & 0x0F); }
   Suit GetSuit() const { return Suit::FromNumericValue((Suit::NumericValue)(cardNumber & 0xF0)); }
   bool IsNull() const { return (cardNumber & 0x0F) == 0; }
   uint8_t ToOrdinal() const;

   bool operator==(CompactCard c) { return cardNumber == c.cardNumber; }

   static CompactCard FromOrdinal(uint8_t ordinal);
   static constexpr CompactCard Null() { return CompactCard(Suit::Clubs(), Rank::Null); }

private:
   uint8_t cardNumber;
};

#endif  // SEAHAVEN2E_CARD_H_
