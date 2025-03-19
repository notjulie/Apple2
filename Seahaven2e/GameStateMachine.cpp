// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include <Apple2Lib/MMIO.h>
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

#include "GameStateMachine.h"

using ::a2::KeyCode;


/// <summary>
/// Performs periodic action
/// </summary>
__attribute__((noinline)) void GameStateMachine::Service() {
   // service animations if we have any
   ServiceAnimations();

   // take other actions according to state
   switch (state) {
   case State::Uninitialized:
      // new game
      StartCurrentGame();
      break;

   case State::Idle:
      ServiceIdle();
      break;

   case State::Screensave:
      if (a2::Keyboard::GetKey() != a2::KeyCode::None)
         ExitScreensave();
      else
         ScreensaveService();
      break;

   default:
      break;
   }
}


/// <summary>
/// Performs periodic actions during animating
/// </summary>
__attribute__((noinline)) void GameStateMachine::ServiceAnimations()
{
   // give the card animator its timeslice... if it says that it's
   // still animating we just exit and let it keep doing its thing
   if (CardAnimator::instance.Service())
      return;

   // not animating... see what we need to do next
   switch (state) {
   case State::Animating:
      if (!CheckAcesToMove())
         EnterIdle();
      break;

   case State::Undoing:
      BeginUndo(false);
      break;

   case State::Redoing:
      BeginRedo(false);
      break;

   case State::MovingToTower:
      StartNextMoveToTower();
      break;

   default:
      break;
   }
}


/// <summary>
/// Performs periodic actions during Idle state
/// </summary>
__attribute__((noinline)) void GameStateMachine::ServiceIdle()
{
   // give the cursor its timeslice
   Cursor::instance.Service();

   // update our timer
   a2::VBLCounter::Update();

   // if we've been in idle too long switch to screensave
   uint8_t ticksInIdle = a2::VBLCounter::GetCounter().hi - idleEntryTime;
   if (ticksInIdle > 3600 / 256)
   {
      EnterScreensave();
      return;
   }

   // check for user input
   KeyCode key = a2::Keyboard::GetKey();
   if (key != KeyCode::None)
   {
      idleEntryTime = a2::VBLCounter::GetCounter().hi;

      // force uppercase
      if ((char)key>='a' && (char)key<='z')
         key = (KeyCode)((char)key - ('a' - 'A'));

      // process
      ProcessInputKey(key);
   }
}


__attribute__((noinline)) void GameStateMachine::ProcessInputKey(KeyCode key)
{
   #pragma GCC diagnostic push
   #pragma GCC diagnostic ignored "-Wswitch"
   switch (key) {
   case (KeyCode)'N':
      // new game...
      NewGame();
      break;

#ifdef DEBUG
   case (KeyCode)'1':
      a2::PAGE2OFF();
      a2::TEXTON();
      break;
#endif // DEBUG

#ifdef DEBUG
   case (KeyCode)'2':
      CardAnimator::instance.SetGraphicsMode();
      break;
#endif // DEBUG

#ifdef DEBUG
   case (KeyCode)'3':
      NewGameWithFullColumn();
      break;
#endif // DEBUG

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
      BeginUndo(true);
      break;

   case (KeyCode)'Y':
      BeginRedo(true);
      break;

   default:
      #ifdef DEBUG
         a2::PRBYTE((uint8_t)key);
      #endif
      break;
   }
   #pragma GCC diagnostic pop
}


/// <summary>
/// Moves the currently selected card to a column
/// </summary>
__attribute__((noinline)) void GameStateMachine::MoveToColumn()
{
   // start a new undo group... we can do this whether we move or not
   // so just get it out of the way
   UndoJournal::instance.StartNewUndo();

   // grab the cursor location
   CardLocation startLocation = Cursor::instance.GetLocation();
   assert(!startLocation.IsNull());

   // locate the target location
   CardLocation targetLocation = Game::GetMoveToColumnDestination(startLocation);

   // complain if we don't have a landing spot
   if (targetLocation.IsNull())
   {
      Audio::ErrorBeep();
      return;
   }

   // if the start location is a column location then we need to carry
   // all the cards below the selected card along with it
   if (startLocation.IsColumn())
      MoveMultipleCards(startLocation, targetLocation, Game::GetSizeOfMoveToColumnGroup(startLocation));
   else
      MoveSingleCard(startLocation, targetLocation);
}


/// <summary>
/// Moves the currently selected card to a tower
/// </summary>
__attribute__((noinline)) void GameStateMachine::MoveToTower()
{
   CardLocation moveToTowerEnd = Cursor::instance.GetLocation();
   assert(!moveToTowerEnd.IsNull());

   // if this is not a column location do an error beep and exit
   if (!moveToTowerEnd.IsColumn())
   {
      Audio::ErrorBeep();
      return;
   }

   // figure out just how many cards we're dealing with
   CardLocation moveToTowerStart = Game::GetBottomColumnCardLocation(moveToTowerEnd.GetColumn());
   uint8_t numberOfCardsToMove = 1 + moveToTowerStart.GetRow() - moveToTowerEnd.GetRow();

   // see if we have enough towers for them
   if (Game::GetNumberOfAvailableTowers() < numberOfCardsToMove)
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
__attribute__((noinline)) void GameStateMachine::StartNextMoveToTower()
{
   // never mind if we're done
   if (moveToTowerCurrentRow < moveToTowerEndRow)
   {
      if (!CheckAcesToMove())
         EnterIdle();
      return;
   }

   // grab the vitals of the start position
   CardLocation start = CardLocation::Column(moveToTowerColumn, moveToTowerCurrentRow);

   // get the end location
   CardLocation end = Game::GetClosestOpenTowerToColumn(moveToTowerColumn);
   assert(!end.IsNull());

   // log
   UndoJournal::instance.LogMove(start, end);

   // start animating
   CardAnimator::instance.StartAnimation(Game::GetCard(start), end);
   --moveToTowerCurrentRow;
   state = State::MovingToTower;
}


/// <summary>
/// Moves a group of cards that starts from the given card and extends down
/// the column the given number of cards, adding them to the current undo group
/// </summary>
__attribute__((noinline)) void GameStateMachine::MoveMultipleCards(CardLocation startLocation, CardLocation targetLocation, uint8_t count)
{
   // check for no-op
   if (count == 0)
       return;

   // make sure we have the required amount of tower space
   uint8_t requiredTowers = count;
   if (!targetLocation.IsTower())
      --requiredTowers;
   if (requiredTowers > Game::GetNumberOfAvailableTowers())
      return;

   // log it
   UndoJournal::instance.LogMove(
            startLocation,
            targetLocation
            );

   // start the animation
   CardAnimator::instance.StartMoveColumnToColumn(startLocation, targetLocation);
   state = State::Animating;
}


/// <summary>
/// Moves a single card, adding it to the current undo group
/// </summary>
__attribute__((noinline)) void GameStateMachine::MoveSingleCard(CardLocation startLocation, CardLocation targetLocation)
{
   // log it
   UndoJournal::instance.LogMove(
            startLocation,
            targetLocation
            );

   // start the animation
   CardAnimator::instance.StartAnimation(Game::GetCard(startLocation), targetLocation);
   state = State::Animating;
}


/// <summary>
///   Starts a new game
/// </summary>
__attribute__((noinline)) void GameStateMachine::NewGame()
{
   // clear the undo journal
   UndoJournal::instance.Clear();

   // shuffle
   PersistentState::instance.NextGameSeed();
   Game::Shuffle16(PersistentState::instance.GetCurrentGameSeed());

   // start the game
   StartCurrentGame();
}


void GameStateMachine::StartCurrentGame()
{
   // have the animator draw
   CardAnimator::instance.DrawGame();

   // reset the cursor
   Cursor::instance.SetCursorLocationToDefault();

   // check for auto moves
   if (!CheckAcesToMove())
      EnterIdle();
}


/// <summary>
/// Restarts the game, resetting the undo journal to the beginning
/// </summary>
__attribute__((noinline)) void GameStateMachine::Restart()
{
   // redeal
   Game::Shuffle16(PersistentState::instance.GetCurrentGameSeed());

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


void GameStateMachine::ExitScreensave()
{
  // have the animator draw
  CardAnimator::instance.DrawGame();

  // check for auto moves
  EnterIdle();
}


/// <summary>
/// Enters idle state
/// </summary>
void GameStateMachine::EnterIdle() {
  state = State::Idle;
  idleEntryTime = a2::VBLCounter::GetCounter().hi;
  Cursor::instance.Show();
}


/// <summary>
///   Checks if there are any aces to move and starts the movement if so
/// </summary>
/// <return>
///   true if a card is being moved to the aces
/// </return>
bool GameStateMachine::CheckAcesToMove()
{
   // find the location of the card to move
   CardLocation startLocation = Game::GetCardToMoveToAce();
   if (startLocation.IsNull())
      return false;

   // get the card
   Card card = Game::GetCard(startLocation);

   // start the animation
   MoveSingleCard(startLocation, CardLocation::AcePile(card.GetSuit()));
   return true;
}


/// <summary>
/// Undoes a move if possible
/// </summary>
__attribute__((noinline)) void GameStateMachine::BeginUndo(bool firstInGroup)
{
   // see if there's something to undo
   UndoInstruction undo =
      firstInGroup ? UndoJournal::instance.GetFirstUndo() : UndoJournal::instance.GetNextUndo();
   if (undo.IsNull())
   {
      EnterIdle();
      return;
   }

   // if this is a column-to-column move treat it as such
   if (undo.location.IsColumn())
   {
      CardLocation startLocation = Game::GetCardLocation(undo.card);
      if (startLocation.IsColumn())
      {
         CardAnimator::instance.StartMoveColumnToColumn(startLocation, undo.location);
         state = State::Undoing;
         return;
      }
   }

   // start it a-moving
   CardAnimator::instance.StartAnimation(undo.card, undo.location);
   state = State::Undoing;
}


/// <summary>
/// Redoes a move if possible
/// </summary>
__attribute__((noinline)) void GameStateMachine::BeginRedo(bool firstInGroup)
{
   UndoInstruction redo =
      firstInGroup ? UndoJournal::instance.GetFirstRedo() : UndoJournal::instance.GetNextRedo();
   if (redo.IsNull())
   {
      EnterIdle();
      return;
   }

   // if this is a column-to-column move treat it as such
   if (redo.location.IsColumn())
   {
      CardLocation startLocation = Game::GetCardLocation(redo.card);
      if (startLocation.IsColumn())
      {
         CardAnimator::instance.StartMoveColumnToColumn(startLocation, redo.location);
         state = State::Redoing;
         return;
      }
   }

   CardAnimator::instance.StartAnimation(redo.card, redo.location);
   state = State::Redoing;
}


__attribute__((noinline)) void GameStateMachine::EnterScreensave()
{
   ScreensaveStart();
   state = State::Screensave;
}

#ifdef DEBUG
void StateMachine::NewGameWithFullColumn()
{
   auto &game = PersistentState::instance.Game;

   // clear the undo journal
   UndoJournal::instance.Clear();

   // shuffle
   PersistentState::instance.NextGameSeed();
   game.NewGameWithFullColumn();

   // start the game
   StartCurrentGame();
}
#endif // DEBUG
