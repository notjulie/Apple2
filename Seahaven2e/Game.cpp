// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "Game.h"

#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/ROM.h>
#include <C6502/Memory.h>
#include "CardLocation.h"
#include "SHAssert.h"


__attribute__((noinline)) void Game::Shuffle16(c6502::Int16 instruction)
{
   deck.Shuffle(instruction);
   c6502::memset8(&columnCounts[0], 5, 10);
   acePiles.Clear();
}


/// <summary>
/// Gets the location of the first card that needs to move to the aces.
/// </summary>
CardLocation Game::GetCardToMoveToAce() const
{
   // look at the towers
   for (int i=0; i < 4; ++i)
   {
      CardLocation location = CardLocation::Tower(i);
      if (CanMoveToAce(location))
         return location;
   }

   // look at the columns
   for (int i=0; i<10; ++i)
   {
      CardLocation location = GetBottomColumnCardLocation(i);
      if (CanMoveToAce(location))
         return location;
   }

   return CardLocation::Null();
}


/// <summary>
/// Gets the card at the given location
/// </summary>
Card Game::GetCard(CardLocation location) const
{
   if (location.IsAce()) {
      return acePiles.GetCard(location);
   } else if (location.IsTower()) {
      return deck.GetTower(location.GetTowerIndex());
   } else if (location.IsColumn()) {
      return GetColumnCard(location.GetColumn(), location.GetRow());
   } else {
      return Card::Null();
   }
}


/// <summary>
/// Gets the location of the given card
/// </summary>
CardLocation Game::GetCardLocation(Card card)
{
   // null cards are nowhere
   if (card.IsNull())
      return CardLocation::Null();

   // see if it's on a tower
   for (uint8_t i=0; i<4; ++i)
      if (card == deck.GetTower(i))
         return CardLocation::Tower(i);

   // see if it's on column... for the sake of frugal use of RAM I'm
   // making this simple but slow... there are some easy speed improvements
   // to make if necessary... I think they are not
   for (uint8_t column=0; column<10; ++column)
   {
      for (uint8_t row=0; row<CardLocations::MaxColumnCards; ++row)
      {
         if (GetColumnCard(column, row) == card)
            return CardLocation::Column(column, row);
      }
   }

   // else if it's not on an ace pile then it's nowhere
   return acePiles.GetCardLocation(card);
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
__attribute__((noinline)) void Game::SetCard(CardLocation location, Card card)
{
   // the card can't be null, that's what remove card is for
   assert(!card.IsNull());

   if (location.IsAce())
   {
      acePiles.Set(location, card);
   }
   else if (location.IsTower())
   {
      deck.SetTower(location.GetTowerIndex(), card);
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
      acePiles.RemoveCard(location);
   }
   else if (location.IsTower())
   {
      deck.SetTower(location.GetTowerIndex(), Card::Null());
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
/// Gets the destination column location for the card
/// </summary>
__attribute((noinline)) CardLocation Game::GetMoveToColumnDestination(CardLocation startLocation)
{
   Card card = GetCard(startLocation);

   if (card.GetRank() == Rank::King)
   {
      if (startLocation.IsTower())
         return GetClosestOpenColumnToTower(startLocation.GetTowerIndex());
      else if (startLocation.IsColumn())
         return GetClosestOpenColumnToColumn(startLocation.GetColumn());
   }
   else
   {
      // get the location of the card one rank higher and verify that it's the bottom
      // of a column
      CardLocation locationAboveTarget = GetCardLocation(card + 1);
      if (IsBottomOfColumn(locationAboveTarget))
      {
         // the target location is one below that
         return CardLocation::Column(locationAboveTarget.GetColumn(), locationAboveTarget.GetRow() + 1);
      }
   }

   return CardLocation::Null();
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
      if (t>=0 && t<4 && deck.GetTower(t).IsNull())
         return CardLocation::Tower(t);

      t = tower + i;
      if (t>=0 && t<4 && deck.GetTower(t).IsNull())
         return CardLocation::Tower(t);
   }

   return CardLocation::Null();
}


/// <summary>
/// Returns true if the given card can be moved to an ace
/// </summary>
__attribute__((noinline)) bool Game::CanMoveToAce(CardLocation location) const
{
   Card card = GetCard(location);
   if (card.IsNull())
      return false;
   else
      return card.GetRank() - acePiles.GetRank(card.GetSuit()) == 1;
}


/// <summary>
/// Gets the location of the bottom card on the given column
/// </summary>
__attribute__((noinline)) CardLocation Game::GetBottomColumnCardLocation(uint8_t column) const
{
   int8_t row = columnCounts[column];
   if (row > 0)
      return CardLocation::Column(column, row - 1);
   else
      return CardLocation::Null();
}


Card Game::GetTowerCard(uint8_t tower) {
  if (tower < 4)
    return deck.GetTower(tower);
  else
    return Card::Null();
}


uint8_t Game::GetNumberOfCardsOnColumn(uint8_t column) const
{
   return columnCounts[column];
}


Card Game::GetColumnCard(uint8_t column, uint8_t row) const
{
   // never mind if we don't have that many cards on the column
   if (row >= columnCounts[column])
      return Card::Null();

   // if it's in our array of cards return what's in the array
   if (row < 5)
      return deck.GetColumnCard(column, row);

   // anything beyond the array is a card stacked on the last card of the array
   return deck.GetColumnCard(column, 4) - (row - 4);
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
/// Sets the card at the given location
/// </summary>
__attribute((noinline)) void Game::SetColumnCard(uint8_t column, uint8_t row, Card card)
{
   // the card can't be null... that's what remove card is for
   assert(!card.IsNull());

   // if the row is 5 or greater the card must be one less than
   // the card above it
   if (row >= 5)
   {
      #ifdef DEBUG
         Card bottomCard = GetColumnCard(column, row - 1);
         assert(bottomCard.GetSuit() == card.GetSuit());
         assert(bottomCard.GetRank() == card.GetRank() + (uint8_t)1);
      #endif

      ++columnCounts[column];
      return;
   }

   // else just set it
   deck.SetColumnCard(column, row, card);
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

   // calculate our result
   uint8_t row = location.GetRow();
   uint8_t column = location.GetColumn();
   uint8_t count = columnCounts[column];
   uint8_t result = count - row;

   // that result is correct, as long as all the cards between
   // this row and the last card on the column are a straight flush;
   // if not we return 0 to indicate that these cards can't move to
   // another column as a group
   if (count > 5)
      count = 5;
   Card topCard = deck.GetColumnCard(column, row);
   while (++row < count)
   {
      Card nextCard = deck.GetColumnCard(column, row);

      if (nextCard.GetSuit() != topCard.GetSuit())
         return 0;
      if (topCard.GetRank() - nextCard.GetRank() != 1)
         return 0;
      topCard = nextCard;
   }

   return result;
}


/// <summary>
/// Get the number of available towers
/// </summary>
__attribute((noinline)) uint8_t Game::GetNumberOfAvailableTowers() const
{
   uint8_t towerCount = 0;
   for (int i=0; i<4; ++i)
      if (deck.GetTower(i).IsNull())
         ++towerCount;


   return towerCount;
}

#ifdef DEBUG

void Game::NewGameWithFullColumn()
{
   acePiles.Clear();
   deck.SetColumnCard(0, 0, Card(Suit::Clubs(), Rank::King));
   deck.SetColumnCard(0, 1, Card(Suit::Clubs(), Rank::Queen));
   deck.SetColumnCard(0, 2, Card(Suit::Clubs(), Rank::Jack));
   deck.SetColumnCard(0, 3, Card(Suit::Clubs(), Rank::Ten));
   deck.SetColumnCard(0, 4, Card(Suit::Diamonds(), Rank::King));
   columnCounts[0] = 16;
   for (int i=1; i<10; ++i)
      columnCounts[i] = 0;
}

#endif // DEBUG


// ========================================================================
// ========================================================================
//     class AcePiles
// ========================================================================
// ========================================================================


Card AcePiles::GetCard(CardLocation location) const
{
   if (location.IsAce())
   {
      uint8_t suitOrdinal = location.GetAceSuitOrdinal();
      return Card((SuitOrdinal)suitOrdinal, ranks[suitOrdinal]);
   }

   return Card::Null();
}


__attribute__((noinline)) CardLocation AcePiles::GetCardLocation(Card card) const
{
   Rank rank = card.GetRank();
   SuitOrdinal suitOrdinal = card.GetSuit().GetOrdinal();
   if (rank <= ranks[(uint8_t)suitOrdinal])
      return CardLocation::AcePile(suitOrdinal);

   return CardLocation::Null();
}

