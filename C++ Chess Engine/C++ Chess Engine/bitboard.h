#pragma once
#include <stdint.h>
#include "defs.h"

/// <summary>
/// This function counts the number of bits in the bitboard.
/// </summary>
/// <param name="bb">The bitboard that will be counted.</param>
/// <returns>The number of bits in the given bitboard.</returns>
int bitCount(bitboard bb);