
#ifndef INT6502_H
#define INT6502_H


namespace c6502 {

   struct Int16 {
      Int16 &operator++() {
         ++i16;
         return *this;
      }

      union {
         int16_t i16;
         struct {
            uint8_t lo;
            uint8_t hi;
         };
      };
   };

}


#endif // INT6502_H
