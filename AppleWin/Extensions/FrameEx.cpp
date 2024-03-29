
#include <stdexcept>
#include <string>
#include <vector>
#include <Windows.h>

#include "Applewin.h"
#include "CardManager.h"
#include "Disk.h"
#include "Video.h"

#include "Frame.h"
#include "FrameEx.h"

// from Frame.cpp
extern void DrawStatusArea(HDC passdc, BOOL drawflags);


bool IsFrameWindowShowing(void)
{
   return IsWindowVisible(::g_hFrameWindow);
}

/// <summary>
/// Shows the frame window
/// </summary>
/// <param name="show"></param>
void ShowFrameWindow(bool show)
{
   // deal with the potential race condition where we get called while the
   // thread is still creating the window
   while (g_hFrameWindow == NULL)
      Sleep(1);
   ShowWindow(g_hFrameWindow, show ? SW_SHOW : SW_HIDE);
}

void ActivateFrameWindow()
{
   SetForegroundWindow(g_hFrameWindow);
}

void StartRunning()
{
	//if (g_CardMgr.QuerySlot(SLOT6) == CT_Disk2)
		//dynamic_cast<Disk2InterfaceCard&>(g_CardMgr.GetRef(SLOT6)).Boot();

	LogFileTimeUntilFirstKeyReadReset();
	g_nAppMode = MODE_RUNNING;

   //DrawStatusArea((HDC)0, DRAW_TITLE);
   //VideoRedrawScreen();
}
