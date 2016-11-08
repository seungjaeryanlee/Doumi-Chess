#pragma once
#include "board.h"


// TODO: Organize Documentation
// Receives board, check if there is an equivalent board in the opening
// book, and gives best move recorded in the boards
Move checkOpeningBook(const Board& board);

Move stringToMove(std::string string);