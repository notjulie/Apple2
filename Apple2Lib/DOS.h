// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef APPLE2_DOS_H
#define APPLE2_DOS_H

namespace a2 {

   /// <summary>
   /// DOS warm start... basically exits to BASIC
   /// </summary>
   inline void DOSWRM()
   {
      asm volatile ("JMP\t$03D0" :::);
   }

   typedef void DOSReturn();

   /// <summary>
   /// Sends the given command to DOS as though typed from the command
   /// prompt; on completion re-enters the program at the given return
   /// address.
   /// </summary>
   void ExecuteDOSCommand(const char *command, DOSReturn *returnAddress);

}


#endif // APPLE2_DOS_H
