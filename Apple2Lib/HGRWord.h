
#ifndef HGRWORD_H
#define HGRWORD_H

namespace a2 {

   /** \brief
    * Represents a pair of HGR bytes (14 pixels)
    */
   class HGRWord {
   public:
      /** \brief
       * Initializes an instance based on a string where each character represents a pixel color
       */
      constexpr HGRWord(const char *s = "              ")
         : left(ParseByte(&s[0], 0)), right(ParseByte(&s[7], 1))
      {
      }

      /** \brief
       * Gets the left byte
       */
      uint8_t GetLeft() const { return left; }

      /** \brief
       * Gets the right byte
       */
      uint8_t GetRight() const { return right; }

   private:
      static constexpr uint8_t ParseByte(const char *s, int offset) {
         uint8_t result = 0;
         bool isGroup2 = false;

         for (int i=0; i<7; ++i)
         {
            switch (s[i])
            {
            case ' ':
            default:
               break;

            case 'W':
               result |= (1<<i);
               break;

            case 'R': // red
            case 'O': // orange
               isGroup2 = true;
               if (((i + offset) & 1) == 1)
                  result |= (1<<i);
               break;

            case 'G': // green
               if (((i + offset) & 1) == 1)
                  result |= (1<<i);
               break;

            case 'V': // violet
               if (((i + offset) & 1) == 0)
                  result |= (1<<i);
               break;

            case 'B': // blue
               isGroup2 = true;
               if (((i + offset) & 1) == 0)
                  result |= (1<<i);
               break;
            }
         }

         if (isGroup2)
            return 0x80 | result;
         else
            return result;
      }

   private:
      uint8_t left, right;
   };
}

#endif // HGRWORD_H

