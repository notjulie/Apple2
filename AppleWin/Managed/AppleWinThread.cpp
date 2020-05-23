
#include "Windows.h"
#include "ApplewinEx.h"
#include "FrameEx.h"

namespace AppleWin::Managed {
   public ref class AppleWinThread {
   public:
      static void Run(void) {
         // save the thread handle for later
         threadID = GetCurrentThreadId();

         // run WinMain on the thread
         AppleWinMain();

         // done; forget the thread ID
         threadID = 0;
      }

      static bool IsShowing() {
         return IsFrameWindowShowing();
      }

      static void Show(bool show) {
         ShowFrameWindow(show);
      }

      static void Shutdown(void) {
         if (threadID != 0)
            PostThreadMessage(threadID, WM_QUIT, 0, 0);
      }

   private:
      static DWORD threadID = 0;
   };
}
