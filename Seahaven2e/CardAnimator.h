
#ifndef CARDANIMATOR_h
#define CARDANIMATOR_h

#include "Card.h"
#include "CardLocation.h"
#include "SavedBackground.h"

class CardAnimator {
public:
   bool IsAnimating() { return false; }
   void StartAnimation(Card card, CardLocation start, CardLocation end);

public:
   static CardAnimator instance;

private:
   SavedBackground background2;
};

#endif // CARDANIMATOR_h
