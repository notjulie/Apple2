

namespace AppleWin::Managed {
   public ref class Machine {
   public:
      static void EnterMonitor() {
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
