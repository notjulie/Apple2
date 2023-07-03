// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Card.h"


// ======================================================================
//          class Card
// ======================================================================


Card &Card::operator-=(int i) {
  --rank;
  return *this;
}

Card operator-(Card card, uint8_t i) {
  Card result = card;
  card -= i;
  return result;
}

Card Card::FromOrdinal(uint8_t cardNumber) {
  Card result;
  result.suit = (Suit)(cardNumber & 3);
  result.rank = (Rank)(1 + (cardNumber >> 2));
  return result;
}

// ======================================================================
//          class CompactCard
// ======================================================================

CompactCard::CompactCard(Card card) {
  rank = (uint8_t)card.rank;
  suit = (uint8_t)card.suit;
}

CompactCard::operator Card() const
{
  Card result;
  result.rank = (Rank)rank;
  result.suit = (Suit)suit;
  return result;
}
