// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "StateMachine.h"

#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include "CardAnimator.h"
#include "CardLocation.h"
#include "Drawing.h"
#include "Game.h"
#include "PersistentState.h"


/// <summary>
/// Performs periodic action
/// </summary>
void StateMachine::Service() {
  switch (state) {
  case State::Uninitialized:
    // set HGR
    a2::HIRESON();
    a2::TEXTOFF();
    a2::MIXEDON();

    // new game
    NewGame();
    break;

  case State::Idle:
    ServiceIdle();
    break;

  case State::MoveToAces:
    CardAnimator::instance.Service();
    if (!CardAnimator::instance.IsAnimating())
      EnterIdle();
    break;
  }
}


void StateMachine::ServiceIdle() {
  if (CheckAcesToMove())
    return;

  switch (a2::getchar()) {
  case 'N':
    // new game...
    NewGame();
    break;

  default:
    break;
  }
}


/// \brief
///   Starts a new game
///
void StateMachine::NewGame() {
  Game::instance.Shuffle16(PersistentState::instance.GetNextGameSeed());
  drawing1.DrawBackground();
  drawing1.DrawGame();
  a2::PAGE2OFF();
  if (!CheckAcesToMove())
    EnterIdle();
}


/// <summary>
/// Enters idle state
/// </summary>
void StateMachine::EnterIdle() {
  state = State::Idle;

  CardLocation location = CardLocation::Tower(1);
  drawing1.ToggleCursor(location.GetX(), location.GetY());
}


/// \brief
///   Checks if there are any aces to move and starts the movement if so
/// \return
///   true if a card is being moved to the aces
///
bool StateMachine::CheckAcesToMove() {
  // find the location of the card to move
  CardLocation startLocation = Game::instance.GetCardToMoveToAce();
  if (startLocation.IsNull())
    return false;

  // get the card
  Card card = Game::instance.GetCard(startLocation);

  // start the animation
  CardAnimator::instance.StartAnimation(
      card,
      startLocation,
      CardLocation::AcePile(card.GetSuit()));
  state = State::MoveToAces;
  return true;
}
