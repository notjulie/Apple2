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
  constexpr CardAnimator() {}

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
  uint8_t background1X = 0;
  uint8_t background1Y = 0;
  SavedBackground background1;

  // saved background for graphics page 2
  uint8_t background2X = 0;
  uint8_t background2Y = 0;
  SavedBackground background2;

  // the animation in progress
  CompactCard cardToMove = CompactCard::Null();
  CardLocation endLocation = CardLocation::Null();
  uint8_t currentX = 0;
  uint8_t currentY = 0;
  uint8_t targetX = 0;
  uint8_t targetY = 0;
  uint8_t distanceX = 0;
  uint8_t distanceY = 0;
  int8_t directionX = 0;
  int8_t directionY = 0;

  uint8_t numeratorX = 0;
  uint8_t numeratorY = 0;
  uint8_t lastVBLCount = 0;
  uint8_t duration = 0;
  uint8_t timeLeft = 0;
};

#endif  // SEAHAVEN2E_CARDANIMATOR_H_
