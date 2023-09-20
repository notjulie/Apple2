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
      Suit::FromOrdinal(cardNumber & 3),
      (Rank)(1 + (cardNumber >> 2))
      );
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

void Card::Print() const
{
   static const char ranks[] = "0A23456789TJQK";
   a2::COUT(ranks[(uint8_t)GetRank()]);
   a2::COUT(GetSuit().ToChar());
}
