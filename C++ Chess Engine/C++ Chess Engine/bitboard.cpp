#include "bitboard.h"

int bitCount(uint64_t bb) {
     int count = 0;
     for (count = 0; bb; count++) {
          bb &= bb - 1;
     }
     return count;
}