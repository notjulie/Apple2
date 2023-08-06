// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Card.h"


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
