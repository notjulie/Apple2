
#ifndef APPLE2ROM_H
#define APPLE2ROM_H

#include <stdint.h>

namespace a2 {

   // ========================================================================
   // ========================================================================
   //    functions
   // ========================================================================
   // ========================================================================

   /// <summary>
   /// This subroutine sends a bell (CTRL G) character to the current output device. It leaves the
   /// accumulator holding $87.
   /// </summary>
   inline void BELL()
   {
      asm volatile ("JSR\t$FF3A" ::: "a");
   }

   /// <summary>
   /// This subroutine beeps the Apple�s speaker for .1 second at lKHz. It scrambles the A and X
   /// registers.
   /// </summary>
   inline void BELLI()
   {
      asm volatile ("JSR\t$FBDD" ::: "a","x");
   }

   /** \brief
    * Sends a character of information
    * to the currently active output device (the address for the output
    * subroutine for this device is held in CSWL ($36) and CSWH ($37)).
    * On entry, A contains the byte to be sent. On exit, registers may be
    * destroyed, depending on the output subroutine for the output device.
    * \param
    * \param
    * \return
    *
    */
   inline void COUT(uint8_t c)
   {
      asm volatile ("JSR\t$FDED" : "+a"(c));
   }

   /** \brief
    * Moves the cursor to the first position of the next video display
    * line (and scrolls if required). On exit, A andY are destroyed.
    *
    */
   inline void CR()
   {
      asm volatile ("JSR\t$FC62" :::"a", "y" );
   }

   /** \brief
    * Clears the screen display from the current cursor position to the end
    * of the line without changing the cursor position. On exit, A andY
    * are destroyed.
    *
    */
   inline void CLREOL()
   {
      asm volatile ("JSR\t$FC9C" :::"a", "y" );
   }

   /** \brief
    * Displays a byte as two hexadecimal digits. On entry, A contains
    * the byte to be displayed. On exit, A is destroyed.
    *
    */
   inline void PRBYTE(uint8_t b)
   {
      asm volatile ("JSR\t$FDDA" : "+a"(b));
   }

   /** \brief
    * Exit to monitor
    */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-noreturn"
   [[noreturn]] inline void MONITOR()
   {
      asm volatile ("JMP\t$FF69" :::);
   }
#pragma GCC diagnostic pop

   void puts(const char *s);
}

#endif // APPLE2ROM_H

