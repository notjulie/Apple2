
#ifndef TEXTPAGE_H
#define TEXTPAGE_H

#include <stdint.h>

namespace a2 {

   class TextPage
   {
   public:
      TextPage() {}

      void Clear();
      void Show();
      void WriteAt(uint8_t x, uint8_t y, const char *s);

   public:
      static constexpr TextPage TEXT1() { return TextPage(Page1MemoryPage); }
      static constexpr TextPage TEXT2() { return TextPage(Page2MemoryPage); }

   private:
      constexpr TextPage(uint8_t memoryPage) : pageOffset(memoryPage) {}

      uint8_t *GetRowAddress(uint8_t row);

   private:
      static constexpr uint8_t Page1MemoryPage = 0x04; // address 0x0400
      static constexpr uint8_t Page2MemoryPage = 0x08; // address 0x0800

      uint8_t pageOffset;
   };

}

#endif // TEXTPAGE_H

