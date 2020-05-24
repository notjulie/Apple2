
#include "ApplewinEx.h"

namespace AppleWin::Managed {
   public ref class Machine {
   public:
      static void EnterMonitor() {
         ApplewinInvoke([]() {
            int x = 1;
            });
         throw gcnew System::NotImplementedException();
      }

      static void CopyMemory(array<System::Byte>^ memory, System::UInt16 startAddress) {
         throw gcnew System::NotImplementedException();
      }

      static void InjectJSR(System::UInt16 startAddress) {
         throw gcnew System::NotImplementedException();
      }
   };
}
