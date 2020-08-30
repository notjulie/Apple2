
#include <thread>
#include "ApplewinEx.h"
#include "CPUEx.h"
#include "FrameEx.h"
using namespace std::chrono_literals;

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

         // reset
         ApplewinInvokeSynchronous([=]() {
            // reset
            StartRunning();
            ResetMachineState();
            });

         // for now throw in a delay; TODO - find a less trusting way to
         // synchronize this... in the meantime it seems like the only risk
         // is we might gets the wrong number of beeps on startup
         std::this_thread::sleep_for(1000ms);

         // start the program
         ApplewinInvokeSynchronous([=]() {
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
