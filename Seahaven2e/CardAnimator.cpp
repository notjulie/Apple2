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

/// <summary>
/// Draws the game as it currently sits; we are responsible for
/// this since we maintain the states of the HGR pages
/// </summary>
void CardAnimator::DrawGame() {
  // draw to page 1
  drawing1.DrawBackground();
  drawing1.DrawGame();

  // show page 1
  a2::PAGE2OFF();

  // copy to the offscreen buffer
  drawing1.CopyTo(&drawing2);

  // both pages are the same and up to date
  state = State::Idle;
}


/// <summary>
///   Starts an animation of a card from one position to another
/// </summary>
void CardAnimator::StartAnimation(
      CompactCard card,
      CardLocation end)
{
  // save parameters
  endLocation = end;

  // step 1: remove the card from its current position
  CardLocation start = Game::instance.GetCardLocation(card);
  Game::instance.SetCard(start, Card());

  // set the bounds of the animation
  currentX = start.GetX();
  currentY = start.GetY() - CardLocations::CardShadowHeight;
  targetX = end.GetX();
  targetY = end.GetY() - CardLocations::CardShadowHeight;
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

  // calculate the duration
  uint8_t pixelDistance = CalculatePixelDistance(distanceX, distanceY);
  duration = pixelDistance >> 4;

  timeLeft = duration;
  numeratorX = numeratorY = 0;
  lastVBLCount = a2::VBLCounter::GetCounter();
  cardToMove = card;

  // draw the game without the card on page 2
  drawing2.EraseCard(start);

  // draw the card at its original position, saving the background
  background2X = currentX;
  background2Y = currentY;
  drawing2.SaveCardBackground(currentX, currentY, &background2);
  drawing2.DrawCardWithShadow(card, currentX, currentY);

  // switch to page 2
  a2::PAGE2ON();

  // draw the game without the card on page 1
  drawing1.EraseCard(start);

  // set the state
  state = State::Page2Visible;
}


/// <summary>
/// Calculates an approximate hypotenuse
/// </summary>
uint8_t CardAnimator::CalculatePixelDistance(uint8_t dx, uint8_t dy) {
  // 3.5x + y
  return
    (dx >> 1) +
    dx +
    dx +
    dx +
    (dy >> 1);
}


/// \brief
///   Updates the state of the animation
///
void CardAnimator::Service() {
  switch (state) {
  case State::Idle:
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
    drawing2.DrawCardWithShadow(cardToMove, currentX, currentY);
    a2::PAGE2ON();
    state = State::Page2Visible;
    break;

  case State::Page2Visible:
    drawing1.RestoreBackground(&background1, background1X, background1Y);
    UpdatePosition();
    background1X = currentX;
    background1Y = currentY;
    drawing1.SaveCardBackground(currentX, currentY, &background1);
    drawing1.DrawCardWithShadow(cardToMove, currentX, currentY);
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
