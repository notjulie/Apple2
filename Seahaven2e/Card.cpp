// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Card.h"


// ======================================================================
//          class Card
// ======================================================================


Card &Card::operator-=(int i) {
  rank = rank - i;
  return *this;
}

Card &Card::operator+=(int i) {
  rank = rank + i;
  return *this;
}

Card Card::FromOrdinal(uint8_t cardNumber) {
  Card result;
  result.suit = (Suit)(cardNumber & 3);
  result.rank = (Rank)(1 + (cardNumber >> 2));
  return result;
}

Card operator-(Card card, uint8_t i) {
  Card result = card;
  card -= i;
  return result;
}

Card operator+(Card card, uint8_t i) {
  Card result = card;
  card += i;
  return result;
}

// ======================================================================
//          class CompactCard
// ======================================================================

CompactCard::CompactCard(Card _card) {
  card.asInt = 0;
  card.parts.rank = (uint8_t)_card.rank;
  card.parts.suit = (uint8_t)_card.suit;
}

CompactCard::operator Card() const
{
  Card result;
  result.rank = (Rank)card.parts.rank;
  result.suit = (Suit)card.parts.suit;
  return result;
}
