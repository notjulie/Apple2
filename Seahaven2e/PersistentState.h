
#ifndef PERSISTENT_STATE_H
#define PERSISTENT_STATE_H

#include <stdint.h>

class PersistentState {
public:
   uint16_t GetNextGameSeed() { return ++gameSeed; }

public:
   static PersistentState instance;

private:
   uint16_t gameSeed = 0;
};

#endif // PERSISTENT_STATE_H
