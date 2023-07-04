// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Column.h"
#include <Apple2Lib/ROM.h>


/// <summary>
/// Gets the card at the given location
/// </summary>
Card Column::GetCard(uint8_t index) const {
  Card result;

  // return null for a bad index
  if (index >= count)
    return result;

  // if it's in our array of cards return what's in the array
  if (index < 5)
    return cards[index];

  // anything beyond the array is a card stacked on the last card of the array
  result = cards[4];
  result -= (index - 4);
  return result;
}


/// <summary>
/// Sets the card at the given location
/// </summary>
void Column::SetCard(uint8_t row, Card card)
{
  // the argument shouldn't be null, RemoveCard is for that
  if (card.IsNull()) {
    a2::puts("SETCARD is null");
    return;
  }

  // if the row is 5 or greater the card must be one less than
  // the card above it
  if (row >= 5) {
    if (card != (GetCard(row - 1) - 1)) {
      a2::puts("SETCARD is wrong");
      return;
    }

    if (row >= count)
      count = row + 1;
    return;
  }

  // else just set it
  cards[row] = card;
  if (row >= count)
    count = row + 1;
}

/// <summary>
/// Gets the index of the card within this column; returns -1 if we don't
/// have that card
/// </summary>
int8_t Column::GetCardIndex(CompactCard card)
{
  // first check to see if we have it stored as-is
  for (int i=0; i<count; ++i)
    if (cards[i] == card)
      return i;

  // if we don't have more than 5 cards then it's not here
  if (count <= 5)
    return -1;

  // a count of >5 means that the 5th card has cards on top of it, which
  // must be of the same suit as the 5th card, and of descending rank
  Card fifthCard = cards[4];
  if (card.GetSuit() != fifthCard.GetSuit())
    return -1;
  int8_t offsetToCard = fifthCard.GetRank() - card.GetRank();
  if (offsetToCard < 0)
    return -1;

  int8_t index = 4 + offsetToCard;
  if (index >= count)
    return -1;
  else
    return index;
}
