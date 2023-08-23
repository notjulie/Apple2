
#include "Keyboard.h"

#include "ROM.h"



namespace a2 {


   // ========================================================================
   // ========================================================================
   //    memory locations
   // ========================================================================
   // ========================================================================

   //const auto KSWL = (KeyboardHook **)0x0038;
   const auto KBD = (volatile uint8_t *)0xC000;
   //static const uint16_t KBDSTRB  = 0xC010;


   // ========================================================================
   // ========================================================================
   //    class Keyboard
   // ========================================================================
   // ========================================================================


   /// <summary>
   /// Apple2 version of something generally like getchar
   /// </summary>
   KeyCode Keyboard::GetKey()
   {
      // get a character if we have one
      int8_t b = *KBD;
      if (b < 0)
      {
         // got a character; clear the strobe
         asm volatile ("BIT\t$C010" : );
         return (KeyCode)(0x7F & b);
      }
      else
      {
         return KeyCode::None;
      }
   }

}

