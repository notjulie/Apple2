// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#include "DOS.h"
#include <stdint.h>


namespace a2 {

   /// <summary>
   /// Sends the given command to DOS as though typed from the command
   /// prompt; on completion re-enters the program at the given return
   /// address.
   /// </summary>
   __attribute__((noinline)) void ExecuteDOSCommand(const char *command, DOSReturn *_returnAddress)
   {
      // save the return address somewhere safe... our locals could get
      // stomped by DOS
      static DOSReturn *returnAddress = _returnAddress;

      // make the command string easy to deal with in asm
      uint8_t commandLo = (uint8_t)(uint16_t)command;
      uint8_t commandHi = (uint8_t)(((uint16_t)command) >> 8);

      asm volatile (
         // save our command pointer... we only use it once, and our
         // hook doesn't want to make any assumptions about using zero
         // page data, so we just hack it right into the load instruction
         "STA\tldChar + 1\n"
         "STX\tldChar + 2\n"

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

         // exit to DOS
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

         // send the callaer a CR
         "LDA\t#$0D\n"

      "hExit:\n"
         "INC\tindex\n"
         "ORA\t#$80\n"
         "LDX\txSave\n"
         "RTS\n"

      "xSave:\n"
         "NOP\n"
      "index:\n"
         "NOP\n"

      "done:\n"
         // restore the original hook and exit the asm block
         "LDY\toldHook\n"
         "STY\t$38\n"
         "LDY\toldHook+1\n"
         "STY\t$39\n"

      : // outputs
      : "a"(commandLo), "x"(commandHi) // inputs
      : // clobbers
      );

      // jump to the return entry point
      (*returnAddress)();
   }

}
