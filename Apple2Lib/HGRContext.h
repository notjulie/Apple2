// =============================================================
//    Copyright 2023 Randy Rasmussen
// =============================================================

#ifndef HGRCONTEXT_H
#define HGRCONTEXT_H

#include "HGRPage.h"

namespace a2 {
   /// <summary>
   /// A context for calculating HGR locations, using globals for
   /// the inputs so that repeated calls don't have to repeatedly pass
   /// parameters
   /// </summary>
   class HGRContext {
   public:
      static HGRPage page;
      static uint8_t row;
      static uint8_t byteOffset;

      static uint8_t *GetByteAddress();
   };
}

#endif
