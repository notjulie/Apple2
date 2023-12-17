// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef TEXTSCREENMANAGER_H
#define TEXTSCREENMANAGER_H

#include <Apple2Lib/TextPage.h>

class TextScreenManager {
public:
   void Initialize();
   void ShowSplash();

public:
   static TextScreenManager instance;

private:
   a2::TextPage textPage;
};


#endif // TEXTSCREENMANAGER_H

