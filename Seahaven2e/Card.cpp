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


/// <summary>
/// Initializes an instance of CompactCard from a Card
/// </summary>
CompactCard::CompactCard(Card _card)
{
   cardNumber = (uint8_t)_card.rank + (uint8_t)_card.suit.numericValue;
}


/// <summary>
/// Conversion operator from CompactCard to Card
/// </summary>
CompactCard::operator Card() const
{
  Card result;
  result.rank = GetRank();
  result.suit = GetSuit();
  return result;
}


/// <summary>
/// Creates a CompactCard instance from a number from 0 to 51; how we
/// map the cards to those numbers is arbitrary as long as it's consistent
/// with ToOrdinal()
/// </summary>
CompactCard CompactCard::FromOrdinal(uint8_t cardNumber)
{
   return CompactCard(
      Suit::FromOrdinal(cardNumber & 3),
      (Rank)(1 + (cardNumber >> 2))
      );
}


/// <summary>
/// Maps a CompactCard instance to a number from 0 to 51; how we
/// map the cards to those numbers is arbitrary as long as it's consistent
/// with FromOrdinal()
/// </summary>
uint8_t CompactCard::ToOrdinal() const
{
   return
      (((uint8_t)GetRank() - 1) << 2) +
      GetSuit().GetOrdinal();
}
