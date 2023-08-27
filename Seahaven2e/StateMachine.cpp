// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "StateMachine.h"

#include <Apple2Lib/Keyboard.h>
#include <Apple2Lib/ROM.h>
#include <Apple2Lib/VBLCounter.h>
#include "Audio.h"
#include "CardAnimator.h"
#include "Cursor.h"
#include "Drawing.h"
#include "Game.h"
#include "PersistentState.h"
#include "Screensave.h"
#include "SHAssert.h"

using ::a2::KeyCode;

/// <summary>
/// Performs periodic action
/// </summary>
__attribute__((noinline)) void StateMachine::Service() {
   switch (state) {
   case State::Uninitialized:
      // new game
      StartCurrentGame();
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

   case State::MovingToTower:
      CardAnimator::instance.Service();
      if (!CardAnimator::instance.IsAnimating())
         StartNextMoveToTower();
      break;

   case State::Screensave:
      if (a2::Keyboard::GetKey() != a2::KeyCode::None)
         ExitScreensave();
      else
         Screensave::instance.Service();
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

  // update our timer
  a2::VBLCounter::Update();
  uint8_t now = a2::VBLCounter::GetCounter();
  uint8_t elapsed = now - lastVBLCount;
  timeInIdle += elapsed;
  lastVBLCount = now;

  // if we've been in idle too long switch to screensave
  if (timeInIdle > 3600)
  {
     EnterScreensave();
     return;
  }

  // check for user input
   KeyCode key = a2::Keyboard::GetKey();
   if (key != KeyCode::None)
   {
      timeInIdle = 0;

      // force uppercase
      if ((char)key>='a' && (char)key<='z')
         key = (KeyCode)((char)key - ('a' - 'A'));

      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wswitch"
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

      case (KeyCode)'R':
         // restart
         Restart();
         break;

      case (KeyCode)'S':
         // force screensave
         EnterScreensave();
         break;

      case (KeyCode)'T':
         MoveToTower();
         break;

      case (KeyCode)'W':
         // write to disk requested by user; for safety we let main handle that
         // since we are exiting to DOS when we do that
         writeRequested = true;
         break;

      case (KeyCode)'Z':
         BeginUndo();
         break;

      case (KeyCode)'Y':
         BeginRedo();
         break;

      default:
         #ifdef DEBUG
            a2::PRBYTE((uint8_t)key);
         #endif
         break;
      }
      #pragma GCC diagnostic pop
   }
}


/// <summary>
/// Moves the currently selected card to a column
/// </summary>
__attribute__((noinline)) void StateMachine::MoveToColumn()
{
   auto &game = PersistentState::instance.Game;

   // grab the cursor location
   CardLocation location = Cursor::instance.GetLocation();
   assert(!location.IsNull());

   // get the card
   Card card = game.GetCard(location);
   assert(!card.IsNull());

   // locate the target location
   CardLocation targetLocation = CardLocation::Null();
   if (card.GetRank() == Rank::King)
   {
      if (location.IsTower())
         targetLocation = game.GetClosestOpenColumnToTower(location.GetTowerIndex());
      else if (location.IsColumn())
         targetLocation = game.GetClosestOpenColumnToColumn(location.GetColumn());
      else
         assert(0);
   }
   else
   {
      // get the location of the card one rank higher and verify that it's the bottom
      // of a column
      CardLocation locationAboveTarget = game.GetCardLocation(card + 1);
      if (game.IsBottomOfColumn(locationAboveTarget))
      {
         // the target location is one below that
         targetLocation = CardLocation::Column(locationAboveTarget.GetColumn(), locationAboveTarget.GetRow() + 1);
      }
   }

   // complain if we don't have a landing spot
   if (targetLocation.IsNull())
   {
      Audio::ErrorBeep();
      return;
   }

   // start a new undo group
   UndoJournal::instance.StartNewUndo();

   // if we are moving column to column it gets slightly complicated if multiple
   // cards are moving, so pop off to our handling for that
   if (location.IsColumn())
   {
      UndoJournal::instance.LogMove(
               card,
               location,
               targetLocation
               );

      CardAnimator::instance.StartMoveColumnToColumn(location, targetLocation);
      state = State::Animating;
      return;
   }

   // start the animation
   MoveCard(card, targetLocation);
}


/// <summary>
/// Moves the currently selected card to a tower
/// </summary>
__attribute__((noinline)) void StateMachine::MoveToTower()
{
   auto &game = PersistentState::instance.Game;

   CardLocation moveToTowerEnd = Cursor::instance.GetLocation();
   assert(!moveToTowerEnd.IsNull());

   // if this is not a column location do an error beep and exit
   if (!moveToTowerEnd.IsColumn())
   {
      Audio::ErrorBeep();
      return;
   }

   // figure out just how many cards we're dealing with
   CardLocation moveToTowerStart = game.GetBottomColumnCardLocation(moveToTowerEnd.GetColumn());
   uint8_t numberOfCardsToMove = 1 + moveToTowerStart.GetRow() - moveToTowerEnd.GetRow();

   // see if we have enough towers for them
   if (game.GetNumberOfAvailableTowers() < numberOfCardsToMove)
   {
      Audio::ErrorBeep();
      return;
   }

   // start a new undo
   UndoJournal::instance.StartNewUndo();

   // save the parameters of the move
   moveToTowerColumn = moveToTowerEnd.GetColumn();
   moveToTowerCurrentRow = moveToTowerStart.GetRow();
   moveToTowerEndRow = moveToTowerEnd.GetRow();

   // go
   StartNextMoveToTower();
}


/// <summary>
/// Moves the next column to tower move
/// </summary>
void StateMachine::StartNextMoveToTower()
{
   auto &game = PersistentState::instance.Game;

   // never mind if we're done
   if (moveToTowerCurrentRow < moveToTowerEndRow)
   {
      if (!CheckAcesToMove())
         EnterIdle();
      return;
   }

   // grab the vitals of the start position
   CardLocation start = CardLocation::Column(moveToTowerColumn, moveToTowerCurrentRow);
   Card card = game.GetCard(start);

   // get the end location
   CardLocation end = game.GetClosestOpenTowerToColumn(moveToTowerColumn);
   assert(!end.IsNull());

   // log
   UndoJournal::instance.LogMove(card, start, end);

   // start animating
   CardAnimator::instance.StartAnimation(card, end);
   --moveToTowerCurrentRow;
   state = State::MovingToTower;
}


/// <summary>
/// Moves the given card, adding it to the undo journal
/// </summary>
void StateMachine::MoveCard(Card card, CardLocation location)
{
   auto &game = PersistentState::instance.Game;

   // log
   UndoJournal::instance.LogMove(card, game.GetCardLocation(card), location);

   // start animating
   CardAnimator::instance.StartAnimation(card, location);
   state = State::Animating;
}

/// <summary>
///   Starts a new game
/// </summary>
void StateMachine::NewGame()
{
   auto &game = PersistentState::instance.Game;

   // shuffle
   game.Shuffle16(PersistentState::instance.GetNextGameSeed());

   // start the game
   StartCurrentGame();
}


void StateMachine::StartCurrentGame()
{
   // have the animator draw
   CardAnimator::instance.DrawGame();

   // reset the cursor
   Cursor::instance.SetCursorLocationToDefault();

   // check for auto moves
   if (!CheckAcesToMove())
      EnterIdle();
}


void StateMachine::Restart()
{
   auto &game = PersistentState::instance.Game;

   // redeal
   game.Shuffle16(PersistentState::instance.GetCurrentGameSeed());

   // have the animator draw
   CardAnimator::instance.DrawGame();

   // reset the cursor
   Cursor::instance.SetCursorLocationToDefault();

   // reset Undo position
   UndoJournal::instance.Restart();

   // check for auto moves
   if (!CheckAcesToMove())
      EnterIdle();
}


void StateMachine::ExitScreensave()
{
  // have the animator draw
  CardAnimator::instance.DrawGame();

  // check for auto moves
  EnterIdle();
}


/// <summary>
/// Enters idle state
/// </summary>
void StateMachine::EnterIdle() {
  state = State::Idle;
  timeInIdle = 0;
  Cursor::instance.Show();
}


/// <summary>
///   Checks if there are any aces to move and starts the movement if so
/// </summary>
/// <return>
///   true if a card is being moved to the aces
/// </return>
bool StateMachine::CheckAcesToMove() {
   auto &game = PersistentState::instance.Game;

   // find the location of the card to move
   CardLocation startLocation = game.GetCardToMoveToAce();
   if (startLocation.IsNull())
      return false;

   // get the card
   Card card = game.GetCard(startLocation);

   // start the animation
   MoveCard(card, CardLocation::AcePile(card.GetSuit()));
   return true;
}


/// <summary>
/// Undoes a move if possible
/// </summary>
__attribute__((noinline)) void StateMachine::BeginUndo()
{
   auto &game = PersistentState::instance.Game;

   // see if there's something to undo
   UndoInstruction undo = UndoJournal::instance.PeekUndo();
   if (undo.IsNull())
      return;

   // tell the journal that we're actually undoing it
   UndoJournal::instance.PopUndo();

   // save the group... we keep undoing until we hit something that
   // belongs to a different group
   currentUndoGroup = undo.GetGroup();

   // if this is a column-to-column move treat it as such
   if (undo.location.IsColumn())
   {
      CardLocation startLocation = game.GetCardLocation(undo.GetCard());
      if (startLocation.IsColumn())
      {
         CardAnimator::instance.StartMoveColumnToColumn(startLocation, undo.location);
         state = State::Animating;
         return;
      }
   }

   // start it a-moving
   CardAnimator::instance.StartAnimation(undo.GetCard(), undo.location);
   state = State::Undoing;
}


/// <summary>
/// Redoes a move if possible
/// </summary>
__attribute__((noinline)) void StateMachine::BeginRedo()
{
   auto &game = PersistentState::instance.Game;

   UndoInstruction redo = UndoJournal::instance.PeekRedo();
   if (!redo.IsNull())
   {
      UndoJournal::instance.PopRedo();

      // save the group... we keep undoing until we hit something that
      // belongs to a different group
      currentUndoGroup = redo.GetGroup();

      // if this is a column-to-column move treat it as such
      if (redo.location.IsColumn())
      {
         CardLocation startLocation = game.GetCardLocation(redo.GetCard());
         if (startLocation.IsColumn())
         {
            CardAnimator::instance.StartMoveColumnToColumn(startLocation, redo.location);
            state = State::Animating;
            return;
         }
      }

      CardAnimator::instance.StartAnimation(redo.GetCard(), redo.location);
      state = State::Redoing;
   }
}


/// <summary>
/// Starts moving the next card to redo if there are more in the current group
/// </summary>
__attribute__((noinline)) void StateMachine::RedoNext()
{
   UndoInstruction redo = UndoJournal::instance.PeekRedo();
   if (!redo.IsNull() && redo.GetGroup() == currentUndoGroup)
   {
      UndoJournal::instance.PopRedo();
      CardAnimator::instance.StartAnimation(redo.GetCard(), redo.location);
   }
   else
   {
      EnterIdle();
   }
}


/// <summary>
/// Starts moving the next card to undo if there are more in the current group
/// </summary>
__attribute__((noinline)) void StateMachine::UndoNext()
{
   UndoInstruction undo = UndoJournal::instance.PeekUndo();
   if (!undo.IsNull() && undo.GetGroup() == currentUndoGroup)
   {
      UndoJournal::instance.PopUndo();
      CardAnimator::instance.StartAnimation(undo.GetCard(), undo.location);
   }
   else
   {
      EnterIdle();
   }
}


__attribute__((noinline)) void StateMachine::EnterScreensave()
{
   Screensave::instance.Start();
   state = State::Screensave;
}
