// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef APPLE2_DOS_H
#define APPLE2_DOS_H

namespace a2 {

   /// <summary>
   /// DOS warm start... basically exits to BASIC
   /// </summary>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-noreturn"
   [[noreturn]] inline void DOSWRM()
   {
      asm volatile ("JMP\t$03D0" :::);
   }
#pragma GCC diagnostic pop

   /// <summary>
   /// Sends the given command to DOS as though typed from the command
   /// prompt.
   ///
   /// NOTE: this will likely have side effects to zero page and
   /// other things... it is probably only safe to call this from low
   /// in the call stack, such as from main
   /// </summary>
   void ExecuteDOSCommand(const char *command);
}


#endif // APPLE2_DOS_H
