#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include <array>
#include "board.h"
#include "movegen.h"
#include "timer.h"
#include "evaluation.h"
#include "pgn.h"

#include "debug.h"
#include "command.h"

/******************************************************************************/
/*                                   DEBUG                                    */
/******************************************************************************/
void main() {
     Board currentBoard;
     board120Setup(currentBoard);

     printf("Perft (Depth %d): %llu\n", 1, divide(1, 0, currentBoard, false));
     printf("Perft (Depth %d): %llu\n", 2, divide(2, 0, currentBoard, false));
     printf("Perft (Depth %d): %llu\n", 3, divide(3, 0, currentBoard, false));
     printf("Perft (Depth %d): %llu\n", 4, divide(4, 0, currentBoard, false));
}