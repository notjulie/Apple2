// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef APPLE2_KEYBOARD_H
#define APPLE2_KEYBOARD_H

#include <stdint.h>

namespace a2 {

   /// <summary>
   /// Apple ][ keyboard keycodes
   /// </summary>
   enum class KeyCode : char {
     None = 0,
     Left = 0x08,
     Down = 0x0A,
     Up   = 0x0B,
     Return = 0x0D,
     Right = 0x15,
   };

   /// <summary>
   /// General global keyboard interface
   /// </summary>
   class Keyboard {
   public:
      static KeyCode GetKey();
   };

}

#endif
