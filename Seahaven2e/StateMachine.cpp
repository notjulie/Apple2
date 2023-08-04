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
__attribute__((noinline)) void StateMachine::Service() {
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

   case State::Undoing:
      CardAnimator::instance.Service();
      if (!CardAnimator::instance.IsAnimating())
         UndoNext();
      break;

   case State::Redoing:
      CardAnimator::instance.Service();
      if (!CardAnimator::instance.IsAnimating())
         RedoNext();
      break;
   }
}


/// <summary>
/// Performs periodic actions during Idle state
/// </summary>
__attribute__((noinline)) void StateMachine::ServiceIdle()
{
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
      BeginUndo();
      break;

   case (KeyCode)'Y':
      BeginRedo();
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
__attribute__((noinline)) void StateMachine::MoveToColumn()
{
   // grab the cursor location
   CardLocation location = Cursor::instance.GetLocation();
   assert(!location.IsNull());

   // get the card
   Card card = Game::instance.GetCard(location);
   if (card.IsNull())
      return;

   // get the location of the card one rank higher and verify that it's the bottom
   // of a column
   CardLocation locationAboveTarget = Game::instance.GetCardLocation(CompactCard(card + 1));
   if (!Game::instance.IsBottomOfColumn(locationAboveTarget))
      return;

   // the target location is one below that
   CardLocation targetLocation = CardLocation::Column(locationAboveTarget.GetColumn(), locationAboveTarget.GetRow() + 1);

   // start a new undo group
   currentUndoGroup = PersistentState::instance.UndoJournal.StartNewUndo();

   // if we are moving column to column it gets slightly complicated if multiple
   // cards are moving, so pop off to our handling for that
   if (location.IsColumn())
   {
      PersistentState::instance.UndoJournal.LogMove(
               currentUndoGroup,
               CompactCard(card),
               location,
               targetLocation
               );

      CardAnimator::instance.StartMoveColumnToColumn(location, targetLocation);
      state = State::Animating;
      return;
   }

   // start the animation
   MoveCard(CompactCard(card), targetLocation);
}


/// <summary>
/// Moves the currently selected card to a tower
/// </summary>
__attribute__((noinline)) void StateMachine::MoveToTower()
{
   CardLocation location = Cursor::instance.GetLocation();
   assert(!location.IsNull());

   // start the animation
   currentUndoGroup = PersistentState::instance.UndoJournal.StartNewUndo();
   MoveCard(Game::instance.GetCard(location), CardLocation::Tower(0));
}


/// <summary>
/// Moves the given card, adding it to the undo journal
/// </summary>
void StateMachine::MoveCard(CompactCard card, CardLocation location)
{
   // log
   PersistentState::instance.UndoJournal.LogMove(currentUndoGroup, card, Game::instance.GetCardLocation(card), location);

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
void StateMachine::BeginUndo()
{
   // see if there's something to undo
   UndoInstruction undo = PersistentState::instance.UndoJournal.PeekUndo();
   if (undo.IsNull())
      return;

   // tell the journal that we're actually undoing it
   PersistentState::instance.UndoJournal.PopUndo();

   // save the group... we keep undoing until we hit something that
   // belongs to a different group
   currentUndoGroup = undo.GetGroup();

   // start it a-mmoving
   CardAnimator::instance.StartAnimation(undo.GetCard(), undo.location);
   state = State::Undoing;
}


/// <summary>
/// Redoes a move if possible
/// </summary>
void StateMachine::BeginRedo()
{
   UndoInstruction redo = PersistentState::instance.UndoJournal.PeekRedo();
   if (!redo.IsNull())
   {
      PersistentState::instance.UndoJournal.PopRedo();

      // save the group... we keep undoing until we hit something that
      // belongs to a different group
      currentUndoGroup = redo.GetGroup();

      CardAnimator::instance.StartAnimation(redo.GetCard(), redo.location);
      state = State::Redoing;
   }
}


/// <summary>
/// Starts moving the next card to redo if there are more in the current group
/// </summary>
void StateMachine::RedoNext()
{
   UndoInstruction redo = PersistentState::instance.UndoJournal.PeekRedo();
   if (!redo.IsNull() && redo.GetGroup() == currentUndoGroup)
   {
      PersistentState::instance.UndoJournal.PopRedo();
      CardAnimator::instance.StartAnimation(redo.GetCard(), redo.location);
   }
   else
   {
      state = State::Idle;
   }
}


/// <summary>
/// Starts moving the next card to undo if there are more in the current group
/// </summary>
void StateMachine::UndoNext()
{
   UndoInstruction undo = PersistentState::instance.UndoJournal.PeekUndo();
   if (!undo.IsNull() && undo.GetGroup() == currentUndoGroup)
   {
      PersistentState::instance.UndoJournal.PopUndo();
      CardAnimator::instance.StartAnimation(undo.GetCard(), undo.location);
   }
   else
   {
      state = State::Idle;
   }
}


