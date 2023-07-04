// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Card.h"


// ======================================================================
//          class Card
// ======================================================================


/// <summary>
/// Subtracts the given value from the rank of the card; on overflow the
/// result will be a null card
/// </summary>
Card &Card::operator-=(int8_t i) {
  rank = rank - i;
  return *this;
}

/// <summary>
/// Adds the given value to the rank of the card; on overflow the
/// result will be a null card
/// </summary>
Card &Card::operator+=(int8_t i) {
  rank = rank + i;
  return *this;
}

Card Card::FromOrdinal(uint8_t cardNumber) {
  Card result;
  result.suit = (Suit)(cardNumber & 3);
  result.rank = (Rank)(1 + (cardNumber >> 2));
  return result;
}

/// <summary>
/// Subtracts the given value from the rank of the card; on overflow the
/// result will be a null card
/// </summary>
Card operator-(Card card, int8_t i) {
  Card result = card;
  result -= i;
  return result;
}

/// <summary>
/// Adds the given value to the rank of the card; on overflow the
/// result will be a null card
/// </summary>
Card operator+(Card card, int8_t i) {
  Card result = card;
  result += i;
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
