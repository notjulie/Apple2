// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "DOS.h"
#include <stdint.h>


namespace a2 {

   /// <summary>
   /// Sends the given command to DOS as though typed from the command
   /// prompt.
   ///
   /// NOTE: this will likely have side effects to zero page and
   /// other things... it is probably only safe to call this from low
   /// in the call stack, such as from main
   /// </summary>
   __attribute__((noinline)) void ExecuteDOSCommand(const char *command)
   {
      // make the command string easy to deal with in asm
      uint8_t commandLo = (uint8_t)(uint16_t)command;
      uint8_t commandHi = (uint8_t)(((uint16_t)command) >> 8);

      asm volatile (
         // save our command pointer... we only use it once, and our
         // hook doesn't want to make any assumptions about using zero
         // page data, so we just hack it right into the load instruction
         "STA\tldChar + 1\n"
         "STX\tldChar + 2\n"

         // save the stack pointer
         "TSX\n"
         "STX\tsSave\n"
//         "PLA\n"
//         "CLC\n"
//         "ADC\t#1\n"
//         "STA\tasmExit+1\n"
//         "PLA\n"
//         "ADC\t#0\n"
//         "STA\tasmExit+2\n"

         // clear our index
         "LDX\t#0\n"
         "STX\tindex\n"

         // save the current keyboard hook
         "LDY\t$38\n"
         "STY\toldHook\n"
         "LDY\t$39\n"
         "STY\toldHook + 1\n"

         // install our keyboard hook
         "LDY\t#mos16lo(hook)\n"
         "STY\t$38\n"
         "LDY\t#mos16hi(hook)\n"
         "STY\t$39\n"

         // exit to DOS; DOS won't return to us, so we need to
         // steal control when it asks for a character from the keyboard
         "JMP\t$03D0\n"

      "oldHook:\n"
         "NOP\n"
         "NOP\n"

      "hook:\n"
         // we return the key in the accumulator and MUST preserve the other
         // registers
         "STX\txSave\n"
         "LDX\tindex\n"
         "BMI\tdone\n"
      "ldChar:\n"
         // the address here gets modified with the address of the
         // command
         "LDA\t$8000,x\n"
         "BNE\thExit\n"

         // we're at the null character; set our index negative
         "LDA\t#$80\n"
         "STA\tindex\n"

         // send the caller a CR
         "LDA\t#$0D\n"

      "hExit:\n"
         "INC\tindex\n"
         "ORA\t#$80\n"
         "LDX\txSave\n"
         "RTS\n"

      "sSave:\n"
         "NOP\n"
      "xSave:\n"
         "NOP\n"
      "index:\n"
         "NOP\n"

      "done:\n"
         // DOS has finished executing our command and has asked us for another
         // character from the keyboard; instead, we will unroll the stack to it's
         // original location and return to our caller.

         // restore the original hook
         "LDY\toldHook\n"
         "STY\t$38\n"
         "LDY\toldHook+1\n"
         "STY\t$39\n"

         // restore stack pointer and return
         "LDX\tsSave\n"
         "TXS\n"
         "RTS\n"

      : // outputs
      : "a"(commandLo), "x"(commandHi) // inputs
      : // clobbers
      );
   }

}
