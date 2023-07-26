// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "StateMachine.h"

#include <Apple2Lib/MMIO.h>
#include <Apple2Lib/ROM.h>
#include "CardAnimator.h"
#include "Cursor.h"
#include "Drawing.h"
#include "Game.h"
#include "PersistentState.h"
#include "SHAssert.h"

using ::a2::KeyCode;

/// <summary>
/// Performs periodic action
/// </summary>
void StateMachine::Service() {
   switch (state) {
   case State::Uninitialized:
      // set HGR
      a2::HIRESON();
      a2::TEXTOFF();

      // new game
      NewGame();
      break;

   case State::Idle:
      ServiceIdle();
      break;

   case State::Animating:
      CardAnimator::instance.Service();
      if (!CardAnimator::instance.IsAnimating())
         if (!CheckAcesToMove())
            EnterIdle();
      break;
   }
}


/// <summary>
/// Performs periodic actions during Idle state
/// </summary>
void StateMachine::ServiceIdle() {
  // give the cursor its timeslice
  Cursor::instance.Service();

  // check for user input
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
   KeyCode key = a2::getKey();
   switch (key) {
   case (KeyCode)'N':
      // new game...
      NewGame();
      break;

   case KeyCode::Up:
      Cursor::instance.Up();
      break;

   case KeyCode::Down:
      Cursor::instance.Down();
      break;

   case KeyCode::Left:
      Cursor::instance.Left();
      break;

   case KeyCode::Right:
      Cursor::instance.Right();
      break;

   case (KeyCode)'C':
      MoveToColumn();
      break;

   case (KeyCode)'T':
      MoveToTower();
      break;

   case (KeyCode)'Z':
      Undo();
      break;

   case (KeyCode)'Y':
      Redo();
      break;

   case KeyCode::None:
      break;

   default:
      a2::PRBYTE((uint8_t)key);
      break;
   }
#pragma GCC diagnostic pop
}


/// <summary>
/// Moves the currently selected card to a column
/// </summary>
void StateMachine::MoveToColumn()
{
   CardLocation location = Cursor::instance.GetLocation();
   assert(!location.IsNull());

   // get the card
   Card card = Game::instance.GetCard(location);
   if (card.IsNull())
      return;

   // get the location of the card above it
   CardLocation locationAboveTarget = Game::instance.GetCardLocation(CompactCard(card + 1));
   if (!Game::instance.IsBottomOfColumn(locationAboveTarget))
      return;

   // the target location is one below that
   CardLocation targetLocation = CardLocation::Column(locationAboveTarget.GetColumn(), locationAboveTarget.GetRow() + 1);

   // start the animation
   MoveCard(CompactCard(card), targetLocation);
}


/// <summary>
/// Moves the currently selected card to a tower
/// </summary>
void StateMachine::MoveToTower()
{
   CardLocation location = Cursor::instance.GetLocation();
   assert(!location.IsNull());

   // start the animation
   MoveCard(Game::instance.GetCard(location), CardLocation::Tower(0));
}


/// <summary>
/// Moves the given card, adding it to the undo journal
/// </summary>
void StateMachine::MoveCard(CompactCard card, CardLocation location)
{
   // log
   PersistentState::instance.UndoJournal.LogMove(card, Game::instance.GetCardLocation(card), location);

   // start animating
   CardAnimator::instance.StartAnimation(card, location);
   state = State::Animating;
}

/// <summary>
///   Starts a new game
/// </summary>
void StateMachine::NewGame() {
  // shuffle
  Game::instance.Shuffle16(PersistentState::instance.GetNextGameSeed());

  // have the animator draw
  CardAnimator::instance.DrawGame();

  // reset the cursor
  Cursor::instance.SetCursorLocationToDefault();

  // check for auto moves
  if (!CheckAcesToMove())
    EnterIdle();
}


/// <summary>
/// Enters idle state
/// </summary>
void StateMachine::EnterIdle() {
  state = State::Idle;
  Cursor::instance.Show();
}


/// <summary>
///   Checks if there are any aces to move and starts the movement if so
/// </summary>
/// <return>
///   true if a card is being moved to the aces
/// </return>
bool StateMachine::CheckAcesToMove() {
  // find the location of the card to move
  CardLocation startLocation = Game::instance.GetCardToMoveToAce();
  if (startLocation.IsNull())
    return false;

  // get the card
  CompactCard card = Game::instance.GetCard(startLocation);

  // start the animation
  MoveCard(card, CardLocation::AcePile(card.GetSuit()));
  return true;
}


/// <summary>
/// Undoes a move if possible
/// </summary>
void StateMachine::Undo()
{
   CompactCard card;
   CardLocation location;
   if (PersistentState::instance.UndoJournal.PopUndo(card, location))
   {
      CardAnimator::instance.StartAnimation(card, location);
      state = State::Animating;
   }
}


/// <summary>
/// Redoes a move if possible
/// </summary>
void StateMachine::Redo()
{
   CompactCard card;
   CardLocation location;
   if (PersistentState::instance.UndoJournal.PopRedo(card, location))
   {
      CardAnimator::instance.StartAnimation(card, location);
      state = State::Animating;
   }
}
