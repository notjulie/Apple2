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
   Card(SuitOrdinal suitOrdinal, Rank rank)
      : cardNumber((uint8_t)rank + (((uint8_t)suitOrdinal) << 4)) {}

   Rank GetRank() const { return (Rank)(cardNumber & 0x0F); }
   SuitOrdinal GetSuit() const { return (SuitOrdinal)(cardNumber>>4); }
   bool IsNull() const { return (cardNumber & 0x0F) == 0; }
   uint8_t ToOrdinal() const;

   uint8_t GetCardNumber() const { return cardNumber; }

   bool operator==(Card c) const { return cardNumber == c.cardNumber; }
   bool operator!=(Card c) const { return cardNumber != c.cardNumber; }

   void DecrementRank() { --cardNumber; }

public:
   static Card FromOrdinal(uint8_t ordinal);
   static Card FromCardNumber(uint8_t _cardNumber);
   static constexpr Card Null() { return Card(0); }

private:
   uint8_t cardNumber;

   constexpr Card(uint8_t _cardNumber) : cardNumber(_cardNumber) {}

   // the compiler goes loony for some reason if this is inlined... it just
   // completely adds useless size to the image
   friend __attribute__((noinline)) Card operator-(Card card, uint8_t diff) {
      Card result;
      result.cardNumber = card.cardNumber - diff;
      return result;
   }
   friend __attribute__((noinline)) Card operator+(Card card, uint8_t diff) {
      Card result;
      result.cardNumber = card.cardNumber + diff;
      return result;
   }
};


#endif  // SEAHAVEN2E_CARD_H_
