
#ifndef CARDANIMATOR_h
#define CARDANIMATOR_h

#include "Card.h"
#include "CardLocation.h"

class CardAnimator {
public:
   bool IsAnimating() { return false; }
   void StartAnimation(Card card, CardLocation start, CardLocation end);

public:
   static CardAnimator instance;
};

#endif // CARDANIMATOR_h
