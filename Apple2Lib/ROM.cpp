
#include "ROM.h"

namespace a2
{
   void puts(const char *s)
   {
      while (*s)
      {
         if (*s == '\n')
         {
            a2::CLREOL();
            a2::CR();
         }
         else
         {
            COUT(*s);
         }
         ++s;
      }
   }
}

