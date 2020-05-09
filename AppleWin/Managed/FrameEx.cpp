
#include <string>
#include <Windows.h>
#include "Frame.h"
#include "FrameEx.h"


bool IsFrameWindowShowing(void)
{
   return IsWindowVisible(::g_hFrameWindow);
}

void ShowFrameWindow(bool show)
{
   ShowWindow(g_hFrameWindow, show ? SW_SHOW : SW_HIDE);
}

