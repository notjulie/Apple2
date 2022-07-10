
#include <thread>
#include <Windows.h>
#include "ApplewinEx.h"
#include "Common.h"
#include "CPU.h"
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

         // put the CPU into running state
         ApplewinInvokeSynchronous([=]() {
            // reset
            StartRunning();
            });

         // TODO...find better way to synchronize this
         std::this_thread::sleep_for(2000ms);

         // reset
         ApplewinInvokeSynchronous([=]() {
            CtrlReset();
            });

         // TODO...find better way to synchronize this
         std::this_thread::sleep_for(2000ms);

         // run
         ApplewinInvokeSynchronous([=]() {
            // jump to the monitor so that we land there when we're done
            CPUEx::Jump(0xFF69);

            // copy the program to memory
            CPUEx::CopyToAppleMemory(&memory[0], startAddress, (uint16_t)memory.size());

            // JSR to it
            CPUEx::Jsr(startAddress);
            });
      }
   };
}
