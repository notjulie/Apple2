
#ifndef APPLE2HGR_H
#define APPLE2HGR_H

#include <stdint.h>

namespace a2 {
   const uint8_t HGRHeight = 192;

   inline uint8_t *HGRPage1Base() {
      return (uint8_t *)0x2000;
   }

}

#endif // APPLE2HGR_H
