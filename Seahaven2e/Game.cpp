// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Game.h"

#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/ROM.h>
#include <C6502/Memory.h>
#include "CardLocation.h"
#include "SHAssert.h"


/// <summary>
/// Our global instance
/// </summary>
Game Game::instance;


/** \brief
 * Shuffles and deals out a new game
 */
void Game::Shuffle16(uint16_t instruction) {
  // create unshuffled deck
  for (uint8_t i=0; i < 52; ++i)
    deck[i] = CompactCard::FromOrdinal(i);

  // shuffle 8 times according to high byte
  Shuffle8(instruction >> 8);

  // shuffle 8 times according to low byte
  Shuffle8((uint8_t)instruction);

  // deal
  uint8_t cardIndex = 0;
  for (uint8_t column=0; column < 10; ++column) {
    columns[column].Clear();
    for (uint8_t row=0; row < 5; ++row) {
      columns[column].Append(deck[cardIndex++]);
    }
  }

  towers[0] = CompactCard::Null();
  towers[1] = deck[cardIndex++];
  towers[2] = deck[cardIndex++];
  towers[3] = CompactCard::Null();

  acePiles[0] = CompactCard::Null();
  acePiles[1] = CompactCard::Null();
  acePiles[2] = CompactCard::Null();
  acePiles[3] = CompactCard::Null();
}

void Game::Shuffle8(uint8_t instruction) {
  CompactCard deckCopy[52];
  uint8_t index;

  // we have two types of shuffling, and we choose one each time through
  // based on the bits in the instruction
  for (int i=0; i < 8; ++i) {
    c6502::memcpy8(deckCopy, deck, 52);

    if (instruction & 1) {
      index = 0;
      for (int j=0; j < 26; ++j) {
        deck[index++] = deckCopy[25 - j];
        deck[index++] = deckCopy[j + 26];
      }
    } else {
      index = 23;
      for (int j=51; j >= 0; --j) {
        deck[j] = deckCopy[index];
        index += 7;
        if (index >= 52)
          index -= 52;
      }
    }

    instruction >>= 1;
  }
}

/** \brief
 * Gets the location of the first card that needs to move to the
 * aces.
 */
CardLocation Game::GetCardToMoveToAce() const {
  // TODO(RER): for the moment all I do is look for an ace on the towers
  // just for a starting point
  for (int i=0; i < 4; ++i) {
    if (towers[i].GetRank() == Rank::Ace)
      return CardLocation::Tower(i);
  }

  return CardLocation::Null();
}

CompactCard Game::GetCard(CardLocation location) const {
  if (location.IsAce()) {
    return acePiles[(uint8_t)location.GetAceSuit()];
  } else if (location.IsTower()) {
    return towers[location.GetTowerIndex()];
  } else if (location.IsColumn()) {
    return columns[location.GetColumn()].GetCard(location.GetRow());
  } else {
    return CompactCard::Null();
  }
}


/// <summary>
/// Gets the location of the given card
/// </summary>
CardLocation Game::GetCardLocation(CompactCard card) {
  if (card.IsNull())
    return CardLocation::Null();

  for (uint8_t i=0; i<4; ++i)
    if (card == towers[i])
      return CardLocation::Tower(i);

  for (uint8_t i=0; i<10; ++i) {
    int8_t index = columns[i].GetCardIndex(card);
    if (index >= 0)
      return CardLocation::Column(i, index);
  }

  return CardLocation::Null();
}


/// <summary>
/// Returns true if the given location is the bottom card on a column
/// <summary>
bool Game::IsBottomOfColumn(CardLocation location) const {
  if (!location.IsColumn())
    return false;

  uint8_t columnIndex = location.GetColumn();
  if (columnIndex >= 10)
    return false;
  else
    return columns[columnIndex].GetCount() == location.GetRow() + 1;
}


/// <summary>
/// Sets the card at the given location
/// </summary>
void Game::SetCard(CardLocation location, CompactCard card)
{
   // the card can't be null, that's what remove card is for
   assert(!card.IsNull());

   if (location.IsAce())
   {
      acePiles[(uint8_t)location.GetAceSuit()] = card;
   }
   else if (location.IsTower())
   {
      towers[location.GetTowerIndex()] = card;
   }
   else if (location.IsColumn())
   {
      columns[location.GetColumn()].SetCard(location.GetRow(), card);
   }
}


/// <summary>
/// Removes the card from the given location
/// </summary>
void Game::RemoveCard(CardLocation location)
{
   if (location.IsAce())
   {
      acePiles[(uint8_t)location.GetAceSuit()] = CompactCard::Null();
   }
   else if (location.IsTower())
   {
      towers[location.GetTowerIndex()] = CompactCard::Null();
   }
   else if (location.IsColumn())
   {
      columns[location.GetColumn()].RemoveCard(location.GetRow());
   }
}


CardLocation Game::GetBottomColumnCardLocation(uint8_t column) {
  int8_t row = columns[column].GetCount();
  if (row > 0)
    return CardLocation::Column(column, row - 1);
  else
    return CardLocation::Null();
}


CompactCard Game::GetTowerCard(uint8_t tower) {
  if (tower < 4)
    return towers[tower];
  else
    return CompactCard::Null();
}

