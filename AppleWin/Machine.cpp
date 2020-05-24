
#include "ApplewinEx.h"
#include "CPUEx.h"

namespace AppleWin::Managed {
   public ref class Machine {
   public:
      /// <summary>
      /// Installs the program at the given address and jumps to it
      /// </summary>
      /// <param name="_memory"></param>
      /// <param name="startAddress"></param>
      static void ExecuteProgram(array<System::Byte>^ _memory, System::UInt16 startAddress) {
         // copy the memory array to a handy vector
         std::vector<uint8_t> memory;
         for (int i = 0; i < _memory->Length; ++i)
            memory.push_back(_memory[i]);

         ApplewinInvoke([=]() {
            // copy the memory
            CPUEx::CopyToAppleMemory(&memory[0], startAddress, (uint16_t)memory.size());

            // jump to the monitor so we have a safe place to return to
            CPUEx::Jump(0xFF69);

            // then JSR to our target address
            CPUEx::Jsr(startAddress);
            });
      }
   };
}
