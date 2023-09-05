
#include "TextScreen.h"

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
}

