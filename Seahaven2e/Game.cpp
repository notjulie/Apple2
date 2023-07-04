// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Game.h"

#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include <C6502/Memory.h>
#include "CardLocation.h"

Game Game::instance;

/** \brief
 * Shuffles and deals out a new game
 */
void Game::Shuffle16(uint16_t instruction) {
  // create unshuffled deck
  for (uint8_t i=0; i < 52; ++i)
    deck[i] = i;

  // shuffle 8 times according to high byte
  Shuffle8(instruction >> 8);

  // shuffle 8 times according to low byte
  Shuffle8((uint8_t)instruction);

  // deal
  uint8_t cardIndex = 0;
  for (uint8_t column=0; column < 10; ++column) {
    columns[column].Clear();
    for (uint8_t row=0; row < 5; ++row) {
      columns[column].Append(Card::FromOrdinal(deck[cardIndex++]));
    }
  }

  towers[0] = Card();
  towers[1] = Card::FromOrdinal(deck[cardIndex++]);
  towers[2] = Card::FromOrdinal(deck[cardIndex++]);
  towers[3] = Card();

  acePiles[0] = Card();
  acePiles[1] = Card();
  acePiles[2] = Card();
  acePiles[3] = Card();
}

void Game::Shuffle8(uint8_t instruction) {
  uint8_t deckCopy[52];
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

  return CardLocation();
}

Card Game::GetCard(CardLocation location) const {
  CardArea area = location.GetArea();
  switch (area) {
  case CardArea::AcePiles:
    return acePiles[location.GetIndex()];

  case CardArea::Towers:
    return towers[location.GetIndex()];

  case CardArea::Nowhere:
    return Card();

  default:
    return columns[(uint8_t)area - (uint8_t)CardArea::Column1].GetCard(
      location.GetIndex());
  }
}


/// <summary>
/// Gets the location of the given card
/// </summary>
CardLocation Game::GetCardLocation(CompactCard card) {
  if (card.IsNull())
    return CardLocation();

  for (uint8_t i=0; i<4; ++i)
    if (card == towers[i])
      return CardLocation::Tower(i);

  for (uint8_t i=0; i<10; ++i) {
    int8_t index = columns[i].GetCardIndex(card);
    if (index >= 0)
      return CardLocation::Column(i, index);
  }

  return CardLocation();
}


/// <summary>
/// Returns true if the given location is the bottom card on a column
/// <summary>
bool Game::IsBottomOfColumn(CardLocation location) const {
  uint8_t columnIndex = (uint8_t)(location.GetArea() - CardArea::Column1);
  if (columnIndex >= 10)
    return false;
  else
    return columns[columnIndex].GetCount() == location.GetIndex() + 1;
}

void Game::SetCard(CardLocation location, Card card) {
  CardArea area = location.GetArea();
  switch (area) {
  case CardArea::AcePiles:
    acePiles[location.GetIndex()] = card;
    break;

  case CardArea::Towers:
    towers[location.GetIndex()] = card;
    break;

  case CardArea::Nowhere:
    break;

  default:
    columns[area - CardArea::Column1].SetCard(location.GetIndex(), card);
    break;
  }
}


CardLocation Game::GetBottomColumnCardLocation(uint8_t column) {
  int8_t row = columns[column].GetCount();
  if (row > 0)
    return CardLocation::Column(column, row - 1);
  else
    return CardLocation();
}


