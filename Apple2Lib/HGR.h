
/// \file HGR.h
///
/// Author: Randy Rasmussen
/// Copyright: 2022
/// License: Public Domain
/// Warranties: None
///
/// Declarations for Apple ][ HIRES graphics support
///

#ifndef APPLE2HGR_H
#define APPLE2HGR_H

#include <stdint.h>

namespace a2 {

   /// <summary>
   /// Height of the HGR screen in pixels
   /// </summary>
   const uint8_t HGRHeight = 192;

   /// <summary>
   /// Width of the HGR screen in bytes
   /// </summary>
   const uint8_t HGRByteWidth = 40;

   /// <summary>
   /// Base address of HGR page 1
   /// </summary>
   inline uint8_t *HGRPage1Base() {
      return (uint8_t *)0x2000;
   }

}

#endif // APPLE2HGR_H
