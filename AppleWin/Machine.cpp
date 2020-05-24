
#include "ApplewinEx.h"
#include "CPUEx.h"

namespace AppleWin::Managed {
   public ref class Machine {
   public:
      static void EnterMonitor() {
         ApplewinInvoke([]() {
            CPUEx::Jump(0xFF69);
            });
      }

      static void CopyMemory(array<System::Byte>^ memory, System::UInt16 startAddress) {
         throw gcnew System::NotImplementedException();
      }

      static void InjectJSR(System::UInt16 startAddress) {
         throw gcnew System::NotImplementedException();
      }
   };
}
