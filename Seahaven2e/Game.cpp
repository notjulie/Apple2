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


/// <summary>
/// look up table for avoiding multiplying by 10
/// </summary>
static constexpr uint8_t rowOffset[5] = {0, 10, 20, 30, 40};


/// <summary>
/// Shuffles and deals out a new game
/// </summary>
__attribute__((noinline)) void Game::Shuffle16(uint16_t instruction)
{
   // create unshuffled deck
   for (uint8_t i=0; i < 52; ++i)
    deck[i] = CompactCard::FromOrdinal(i);

   // shuffle 8 times according to high byte
   Shuffle8(instruction >> 8);

   // shuffle 8 times according to low byte
   Shuffle8((uint8_t)instruction);

   // deal
   uint8_t cardIndex = 0;
   for (uint8_t column=0; column < 10; ++column)
   {
      for (uint8_t index=column; index < 50; index += 10)
      {
         columnCards[index] = deck[cardIndex++];
      }
      columnCounts[column] = 5;
   }

   towers[0] = CompactCard::Null();
   towers[1] = deck[cardIndex++];
   towers[2] = deck[cardIndex++];
   towers[3] = CompactCard::Null();

   acePiles[0] = Rank::Null;
   acePiles[1] = Rank::Null;
   acePiles[2] = Rank::Null;
   acePiles[3] = Rank::Null;
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


/// <summary>
/// Gets the location of the first card that needs to move to the aces.
/// </summary>
CardLocation Game::GetCardToMoveToAce() const {
   // look at the towers
   for (int i=0; i < 4; ++i)
   {
      if (CanMoveToAce(towers[i]))
         return CardLocation::Tower(i);
   }

   // look at the columns
   for (int i=0; i<10; ++i)
   {
      CardLocation location = GetBottomColumnCardLocation(i);
      CompactCard card = GetCard(location);
      if (CanMoveToAce(card))
         return location;
   }

   return CardLocation::Null();
}


/// <summary>
/// Gets the card at the given location
/// </summary>
CompactCard Game::GetCard(CardLocation location) const
{
   if (location.IsAce()) {
      Suit suit = location.GetAceSuit();
      return CompactCard(suit, acePiles[(uint8_t)suit]);
   } else if (location.IsTower()) {
      return towers[location.GetTowerIndex()];
   } else if (location.IsColumn()) {
      return GetColumnCard(location.GetColumn(), location.GetRow());
   } else {
      return CompactCard::Null();
   }
}


/// <summary>
/// Gets the location of the given card
/// </summary>
CardLocation Game::GetCardLocation(CompactCard card)
{
   // null cards are nowhere
   if (card.IsNull())
      return CardLocation::Null();

   // see if it's on a tower
   for (uint8_t i=0; i<4; ++i)
      if (card == towers[i])
         return CardLocation::Tower(i);

   // see if it's on column
   for (uint8_t column=0; column<10; ++column)
   {
      int8_t index = GetColumnCardIndex(column, card);
      if (index >= 0)
         return CardLocation::Column(column, index);
   }

   // see if it's on an ace pile
   Rank rank = card.GetRank();
   for (uint8_t i=0; i<4; ++i)
      if (acePiles[i] >= rank)
         return CardLocation::AcePile((Suit)i);

   // else it's nowhere
   return CardLocation::Null();
}


/// <summary>
/// Returns true if the given location is the bottom card on a column
/// <summary>
bool Game::IsBottomOfColumn(CardLocation location) const
{
   if (!location.IsColumn())
      return false;

   uint8_t columnIndex = location.GetColumn();
   if (columnIndex >= 10)
      return false;
   else
      return columnCounts[location.GetColumn()] == location.GetRow() + 1;
}


/// <summary>
/// Sets the card at the given location
/// </summary>
__attribute__((noinline)) void Game::SetCard(CardLocation location, CompactCard card)
{
   // the card can't be null, that's what remove card is for
   assert(!card.IsNull());

   if (location.IsAce())
   {
      acePiles[(uint8_t)location.GetAceSuit()] = card.GetRank();
   }
   else if (location.IsTower())
   {
      towers[location.GetTowerIndex()] = card;
   }
   else if (location.IsColumn())
   {
      SetColumnCard(location.GetColumn(), location.GetRow(), card);
   }
}


/// <summary>
/// Removes the card from the given location
/// </summary>
__attribute__((noinline)) void Game::RemoveCard(CardLocation location)
{
   if (location.IsAce())
   {
      acePiles[(uint8_t)location.GetAceSuit()] = acePiles[(uint8_t)location.GetAceSuit()] - 1;
   }
   else if (location.IsTower())
   {
      towers[location.GetTowerIndex()] = CompactCard::Null();
   }
   else if (location.IsColumn())
   {
      RemoveColumnCard(location.GetColumn(), location.GetRow());
   }
}


/// <summary>
/// Gets the card location of the open column closest to the given
/// column
/// </summary>
__attribute__((noinline)) CardLocation Game::GetClosestOpenColumnToColumn(uint8_t column) const
{
   // expand our search radius until we find something
   for (int i=0; i<10; ++i)
   {
      int8_t c = (int8_t)column - i;
      if (c>=0 && c<10 && columnCounts[c]==0)
         return CardLocation::Column(c, 0);

      c = column + i;
      if (c>=0 && c<10 && columnCounts[c]==0)
         return CardLocation::Column(c, 0);
   }

   return CardLocation::Null();
}


/// <summary>
/// Gets the card location of the open column closest to the given
/// tower
/// </summary>
CardLocation Game::GetClosestOpenColumnToTower(uint8_t tower) const
{
   return GetClosestOpenColumnToColumn(tower + 3);
}


/// <summary>
/// Gets the card location of the open tower closest to the given
/// column
/// </summary>
__attribute__((noinline)) CardLocation Game::GetClosestOpenTowerToColumn(uint8_t column) const
{
   // start from the tower nearest the column
   int8_t tower = (int8_t)column - 3;
   if (tower<0)
      tower = 0;
   else if (tower > 3)
      tower = 3;

   // expand our search radius until we find something
   for (int i=0; i<4; ++i)
   {
      int8_t t = tower - i;
      if (t>=0 && t<4 && towers[t].IsNull())
         return CardLocation::Tower(t);

      t = tower + i;
      if (t>=0 && t<4 && towers[t].IsNull())
         return CardLocation::Tower(t);
   }

   return CardLocation::Null();
}


/// <summary>
/// Returns true if the given card can be moved to an ace
/// </summary>
bool Game::CanMoveToAce(CompactCard card) const
{
   if (card.IsNull())
      return false;
   else
      return card.GetRank() - acePiles[(uint8_t)card.GetSuit()] == 1;
}


CardLocation Game::GetBottomColumnCardLocation(uint8_t column) const
{
   int8_t row = columnCounts[column];
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


uint8_t Game::GetNumberOfCardsOnColumn(uint8_t column) const
{
   return columnCounts[column];
}


CompactCard Game::GetColumnCard(uint8_t column, uint8_t row) const
{
   assert(row <= columnCounts[column]);

   // if it's in our array of cards return what's in the array
   if (row < 5)
      return columnCards[column + rowOffset[row]];

   // anything beyond the array is a card stacked on the last card of the array
   Card result = columnCards[40 + column];
   result -= (row - 4);
   return CompactCard(result);
}

/// <summary>
/// Removes the card at the given row
/// </summary>
void Game::RemoveColumnCard(uint8_t column, uint8_t row)
{
   // it has to be the bottom card
   assert(row == columnCounts[column] - 1);
   columnCounts[column]--;
}


/// <summary>
/// Gets the index of the card within this column; returns -1 if we don't
/// have that card
/// </summary>
int8_t Game::GetColumnCardIndex(uint8_t column, CompactCard card)
{
   uint8_t count = columnCounts[column];

   // first check to see if we have it stored as-is
   {
      uint8_t index = column;
      for (int i=0; i<count; ++i)
      {
         if (columnCards[index] == card)
            return i;
         index += 10;
      }
   }

   // if we don't have more than 5 cards then it's not here
   if (count <= 5)
      return -1;

   // a count of >5 means that the 5th card has cards on top of it, which
   // must be of the same suit as the 5th card, and of descending rank
   Card fifthCard = GetColumnCard(column, 4);
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


/// <summary>
/// Sets the card at the given location
/// </summary>
void Game::SetColumnCard(uint8_t column, uint8_t row, CompactCard card)
{
   // the card can't be null... that's what remove card is for
   assert(!card.IsNull());

   // if the row is 5 or greater the card must be one less than
   // the card above it
   if (row >= 5)
   {
      #ifdef DEBUG
         CompactCard bottomCard = GetColumnCard(column, row - 1);
         assert(bottomCard.GetSuit() == card.GetSuit());
         assert(bottomCard.GetRank() == card.GetRank() + (uint8_t)1);
      #endif

      ++columnCounts[column];
      return;
   }

   // else just set it
   columnCards[column + rowOffset[row]] = card;
   if (row >= columnCounts[column])
      columnCounts[column] = row + 1;
}


/// <summary>
/// Looks at the column location and determines if it is the top
/// of a group of cards that can be moved together and if so how
/// many are in the group.
/// </summary>
__attribute__((noinline)) uint8_t Game::GetSizeOfMoveToColumnGroup(CardLocation location) const
{
   assert(location.IsColumn());

   uint8_t column = location.GetColumn();
   uint8_t row = location.GetRow();
   uint8_t count = columnCounts[column];
   Card topCard = GetColumnCard(column, row);

   // confirm that all cards below form a straight flush
   for (uint8_t i=row + 1; i<count; ++i)
   {
      Card card = GetColumnCard(column, i);
      if (card.GetSuit() != topCard.GetSuit())
         return 0;
      if (topCard.GetRank() - card.GetRank() != i - row)
         return 0;
   }

   // if so we're good
   return count - row;
}


/// <summary>
/// Get the number of available towers
/// </summary>
uint8_t Game::GetNumberOfAvailableTowers() const
{
   uint8_t towerCount = 0;
   for (int i=0; i<4; ++i)
      if (towers[i].IsNull())
         ++towerCount;


   return towerCount;
}
