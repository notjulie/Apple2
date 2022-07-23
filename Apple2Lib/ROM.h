
#ifndef APPLE2ROM_H
#define APPLE2ROM_H

namespace a2 {

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

}

#endif // APPLE2ROM_H

