// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef SEAHAVEN2E_CARDANIMATOR_H_
#define SEAHAVEN2E_CARDANIMATOR_H_

#include "Card.h"
#include "CardLocation.h"
#include "SavedBackground.h"

class CardAnimator {
 public:
  void DrawGame();
  bool IsAnimating() { return state != State::Idle; }
  void StartAnimation(CompactCard card, CardLocation end);
  void Service();

 public:
  static CardAnimator instance;

 private:
  enum class State {
    /// <summary>
    /// Both HGR pages the same and up to date
    /// </summary>
    Idle,

    /// <summary>
    /// Animating, page 1 visible
    /// </summary>
    Page1Visible,

    /// <summary>
    /// Animating, page 1 visible
    /// </summary>
    Page2Visible
  };

 private:
  static uint8_t CalculatePixelDistance(uint8_t dx, uint8_t dy);
  void UpdatePosition();

 private:
  State state = State::Idle;

  // saved background for graphics page 1
  uint8_t background1X;
  uint8_t background1Y;
  SavedBackground background1;

  // saved background for graphics page 2
  uint8_t background2X;
  uint8_t background2Y;
  SavedBackground background2;

  // the animation in progress
  CompactCard cardToMove;
  CardLocation endLocation = CardLocation::Null();
  uint8_t currentX, currentY;
  uint8_t targetX, targetY;
  uint8_t distanceX, distanceY;
  int8_t directionX, directionY;

  uint8_t numeratorX, numeratorY;
  uint8_t lastVBLCount;
  uint8_t duration;
  uint8_t timeLeft;
};

#endif  // SEAHAVEN2E_CARDANIMATOR_H_
