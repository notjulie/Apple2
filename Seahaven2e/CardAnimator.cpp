// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "CardAnimator.h"

#include <Apple2Lib/IO.h>
#include <Apple2Lib/ROM.h>
#include <Apple2Lib/VBLCounter.h>
#include "Drawing.h"
#include "Game.h"

CardAnimator CardAnimator::instance;

/// \brief
///   Starts an animation of a card from one position to another
///
void CardAnimator::StartAnimation(
      CompactCard card,
      CardLocation start,
      CardLocation end) {
  // save parameters
  endLocation = end;

  // step 1: remove the card from its current position
  Game::instance.SetCard(start, Card());

  // set the bounds of the animation
  currentX = start.GetX();
  currentY = start.GetY();
  targetX = end.GetX();
  targetY = end.GetY();
  if (targetX > currentX) {
    distanceX = targetX - currentX;
    directionX = 1;
  } else {
    distanceX = currentX - targetX;
    directionX = -1;
  }
  if (targetY > currentY) {
    distanceY = targetY - currentY;
    directionY = 1;
  } else {
    distanceY = currentY - targetY;
    directionY = -1;
  }
  // throw together a duration... TODO better
  duration = 30;
  timeLeft = duration;
  numeratorX = numeratorY = 0;
  lastVBLCount = a2::VBLCounter::GetCounter();
  cardToMove = card;

  // draw the game without the card on page 2
  drawing2.DrawBackground();
  drawing2.DrawGame();

  // draw the card at its original position, saving the background
  background2X = currentX;
  background2Y = currentY;
  drawing2.SaveCardBackground(currentX, currentY, &background2);
  drawing2.DrawCard(card, currentX, currentY);

  // switch to page 2
  a2::PAGE2ON();

  // set the state
  state = State::Page2Initialized;
}


/// \brief
///   Updates the state of the animation
///
void CardAnimator::Service() {
  switch (state) {
  case State::Idle:
    break;

  case State::Page2Initialized:
    // copy page 2 to page 1
    drawing2.CopyTo(&drawing1);

    // erase the card that we're moving
    drawing1.RestoreBackground(&background2, background2X, background2Y);

    // set the new position
    UpdatePosition();

    // save background, draw, switch pages, update state
    background1X = currentX;
    background1Y = currentY;
    drawing1.SaveCardBackground(currentX, currentY, &background1);
    drawing1.DrawCard(cardToMove, currentX, currentY);
    a2::PAGE2OFF();
    state = State::Page1Visible;
    break;

  case State::Page1Visible:
    if (timeLeft == 0) {
      Game::instance.SetCard(endLocation, cardToMove);
      state = State::Idle;
      break;
    }
    drawing2.RestoreBackground(&background2, background2X, background2Y);
    UpdatePosition();
    background2X = currentX;
    background2Y = currentY;
    drawing2.SaveCardBackground(currentX, currentY, &background2);
    drawing2.DrawCard(cardToMove, currentX, currentY);
    a2::PAGE2ON();
    state = State::Page2Visible;
    break;

  case State::Page2Visible:
    drawing1.RestoreBackground(&background1, background1X, background1Y);
    UpdatePosition();
    background1X = currentX;
    background1Y = currentY;
    drawing1.SaveCardBackground(currentX, currentY, &background1);
    drawing1.DrawCard(cardToMove, currentX, currentY);
    a2::PAGE2OFF();
    state = State::Page1Visible;
    break;
  }
}


/// \brief
///   Calculates the new position
///
void CardAnimator::UpdatePosition() {
  if (timeLeft == 0)
    return;

  // snapshot the time
  uint8_t now = a2::VBLCounter::GetCounter();

  // step once for each tick of the clock
  while (lastVBLCount != now) {
    numeratorX += distanceX;
    while (numeratorX >= duration) {
      currentX += directionX;
      numeratorX -= duration;
    }

    numeratorY += distanceY;
    while (numeratorY >= duration) {
      currentY += directionY;
      numeratorY -= duration;
    }

    ++lastVBLCount;
    if (--timeLeft == 0)
      break;
  }
}
