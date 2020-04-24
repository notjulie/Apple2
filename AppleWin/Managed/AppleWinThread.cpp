
#include "Windows.h"
#include "Applewin.h"

namespace AppleWin::Managed {
   public ref class AppleWinThread {
   public:
      static void Run(void) {
         AppleWinMain();
      }
   };
}
