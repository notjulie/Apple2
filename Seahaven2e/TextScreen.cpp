
#include "TextScreen.h"
#include <Apple2Lib/VBLCounter.h>

TextScreen TextScreen::instance;


void TextScreen::Initialize()
{
   // we use text page 2... page 1 gets written on by some of our
   // DOS commands and we j=use it for debugging, too
   textPage = a2::TextPage::TEXT2();
}

void TextScreen::ShowSplash()
{
   textPage.Clear();
   textPage.Show();
   textPage.WriteAt(6, 9, "SEA][HAVEN TOWERS V1.3.DEV");

   a2::VBLCounter::Update();
   uint8_t startTime = a2::VBLCounter::GetCounter().lo;
   for (;;)
   {
      a2::VBLCounter::Update();
      uint8_t elapsed = a2::VBLCounter::GetCounter().lo - startTime;
      if (elapsed > 200)
         break;
   }
}

