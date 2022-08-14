
#ifndef APPLE2IO_H
#define APPLE2IO_H

#include <stdint.h>

namespace a2 {

   /** \brief
    * Select low-resolution graphics
    */
   inline void HIRESOFF()
   {
      asm volatile ("STA\t$C056" : );
   }


   /** \brief
    * Select high-resolution graphics
    */
   inline void HIRESON()
   {
      asm volatile ("STA\t$C057" : );
   }

   /** \brief
    * Select graphics mode
    */
   inline void TEXTOFF()
   {
      asm volatile ("STA\t$C050" : );
   }

   /** \brief
    * Select text mode
    */
   inline void TEXTON()
   {
      asm volatile ("STA\t$C051" : );
   }

   int16_t getchar();
}

/*
$C000 (49152) w 80STOREOFF Allow PAGE2 to switch between video page1
and page2
$C001 (49153) w 80STOREON Allow PAGE2 to switch between main and aux.
video memory
$C018 {49176) R7 80S TORE 1 = PAGE2 switches main/aux.
0 = PAGE2 switches video pages
$COOC (49164) w 80COLOFF Turn off 80-column display
$C00D {49165) w 80COLON Turn on 80-column display
$C01F {49183) R7 80COL 1 = 80-column display is on
0 = 40-column display is on
$C01A (49178) R7 TEXT 1 = a text mode is active
0 = a graphics mode active
$C052 (49234) RW MIXED OFF Use full-screen for graphics
$C053 (49235) RW MIXED ON Use graphics with four lines of text
$C01B (49179) R7 MIXED 1 = mixed graphics and text 2
0 = full screen graphics
$C054 (49236) RW PAGE20FF Select page1 display (or main video memory) 1
$C055 (49237) RW PAGE20N Select page2 display (or aux. video memory) 1
$C01C (49180) R7 PAGE2 1 = video page2 selected OR aux. video page 1
selected
$C01D (49181) R7 HIRES 1 = high-resolution graphics 1,2
0 = low-resolution graphics
*/

#endif // APPLE2IO_H
