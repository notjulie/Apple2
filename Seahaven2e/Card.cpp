// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Card.h"
#include <Apple2Lib/ROM.h>


/// <summary>
/// Creates a Card instance from a number from 0 to 51; how we
/// map the cards to those numbers is arbitrary as long as it's consistent
/// with ToOrdinal()
/// </summary>
__attribute__((noinline)) Card Card::FromOrdinal(uint8_t cardNumber)
{
   return Card(
      (SuitOrdinal)(cardNumber & 3),
      (Rank)(1 + (cardNumber >> 2))
      );
}


/// <summary>
/// Creates a card from our internal 6-bit card number; note that this
/// is different from our ordinal which is guaranteed to be in the range
/// 0 to 51... this value is in the range 0 to 63.
/// </summary>
Card Card::FromCardNumber(uint8_t _cardNumber) {
   Card result;
   result.cardNumber = _cardNumber;
   return result;
}


/// <summary>
/// Maps a Card instance to a number from 0 to 51; how we
/// map the cards to those numbers is arbitrary as long as it's consistent
/// with FromOrdinal()
/// </summary>
uint8_t Card::ToOrdinal() const
{
   return
      (((uint8_t)GetRank() - 1) << 2) +
      GetSuit().GetOrdinal();
}

