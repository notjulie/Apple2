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
   CompactCard(Suit suit, Rank rank);
   explicit CompactCard(Card card);

   operator Card() const;

   Rank GetRank() const { return (Rank)card.parts.rank; }
   Suit GetSuit() const { return (Suit)card.parts.suit; }
   bool IsNull() const { return card.parts.rank == 0; }
   uint8_t ToOrdinal() const;

   bool operator==(CompactCard c) { return card.asInt == c.card.asInt; }

   static CompactCard FromOrdinal(uint8_t ordinal);
   static constexpr CompactCard Null() { return CardDetails(Rank::Null, Suit::Clubs); }

private:
   union CardDetails {
      CardDetails() {}
      constexpr CardDetails(Rank rank, Suit suit) : parts(rank, suit) {}
      struct Parts {
         constexpr Parts(Rank rank, Suit suit) : rank((uint8_t)rank), suit((uint8_t)suit) {}
         uint8_t rank : 4;
         uint8_t suit : 2;
      } parts;
      uint8_t asInt;
   };
   static_assert(sizeof(CardDetails)==1, "CardDetails wrong size");

private:
   constexpr CompactCard(CardDetails cardDetails) : card(cardDetails) {}

private:
   CardDetails card;
};

#endif  // SEAHAVEN2E_CARD_H_
