// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "TextScreenManager.h"


TextScreenManager TextScreenManager::instance;


void TextScreenManager::Initialize()
{
   // we use text page 2... page 1 gets written on by some of our
   // DOS commands and we j=use it for debugging, too
   textPage = a2::TextPage::TEXT2();
}

void TextScreenManager::ShowScreen(TextScreenID screen)
{
   textPage.Clear();
   textPage.Show();
   textPage.WriteAt(6, 9, "SEA][HAVEN TOWERS V1.3.DEV");
}

