
#ifndef CARDANIMATOR_h
#define CARDANIMATOR_h

#include "Card.h"
#include "CardLocation.h"
#include "SavedBackground.h"

class CardAnimator {
public:
   bool IsAnimating() { return state != State::Idle; }
   void StartAnimation(Card card, CardLocation start, CardLocation end);
   void Service();

public:
   static CardAnimator instance;

private:
   enum class State {
      Idle,
      Page2Initialized,
      Page1Visible,
      Page2Visible
   };

private:
   void UpdatePosition();

private:
   State state = State::Idle;

   uint8_t background1X;
   uint8_t background1Y;
   SavedBackground background1;

   uint8_t background2X;
   uint8_t background2Y;
   SavedBackground background2;

   uint8_t currentX, currentY;
   uint8_t targetX, targetY;
   Card cardToMove;
};

#endif // CARDANIMATOR_h
