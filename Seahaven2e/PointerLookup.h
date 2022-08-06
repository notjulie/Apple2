
#ifndef POINTERLOOKUP_H
#define POINTERLOOKUP_H


template <typename T, int length> class PointerLookup {
public:
   inline void Set(uint8_t index, T *value) {
      hi[index] = ((uint16_t)value) >> 8;
      lo[index] = (uint8_t)(uint16_t)value;
   }

   inline T *Get(uint8_t index) {
      return (T *)((hi[index]<<8) | lo[index]);
   }

private:
   uint8_t hi[length];
   uint8_t lo[length];
};

#endif // POINTERLOOKUP_H
