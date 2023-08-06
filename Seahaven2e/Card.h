// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_CARD_H_
#define SEAHAVEN2E_CARD_H_

#include <stdint.h>
#include "Rank.h"
#include "Suit.h"


/// <summary>
/// A single-byte representation of card that's handy for storage of card arrays
/// or for passing around as a function parameter.
/// </summary>
class Card {
public:
   Card() {}
   constexpr Card(Suit suit, Rank rank)
      : cardNumber((uint8_t)rank + (uint8_t)suit.numericValue) {}

   Rank GetRank() const { return (Rank)(cardNumber & 0x0F); }
   Suit GetSuit() const { return Suit::FromNumericValue((Suit::NumericValue)(cardNumber & 0xF0)); }
   bool IsNull() const { return (cardNumber & 0x0F) == 0; }
   uint8_t ToOrdinal() const;

   bool operator==(Card c) const { return cardNumber == c.cardNumber; }

   static Card FromOrdinal(uint8_t ordinal);
   static constexpr Card Null() { return Card(Suit::Clubs(), Rank::Null); }

private:
   uint8_t cardNumber;

   friend Card operator-(Card card, uint8_t diff) {
      Card result;
      result.cardNumber = card.cardNumber - diff;
      return result;
   }
   friend Card operator+(Card card, uint8_t diff) {
      Card result;
      result.cardNumber = card.cardNumber + diff;
      return result;
   }
};


#endif  // SEAHAVEN2E_CARD_H_
