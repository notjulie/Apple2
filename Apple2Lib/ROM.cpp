
#include "ROM.h"

namespace a2
{
   void puts(const char *s)
   {
      while (*s)
         COUT(*s++);
   }
}

