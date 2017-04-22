/******************************************************************************/
/* debug.cpp                                                                  */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#include "debug.h"
#include "movegen.h"
#include <fstream>

uint64_t divide(const int depth, const int maxDepth, Board& board, const bool showOutput) {

     if (depth == 0) { return 1; }

     uint64_t node = 0, individualNode = 0;
     int capturedPiece;

     MoveList moveList = moveGeneration(board);
     std::array<bool, 4> castlingRights = board.getCastlingRights();
     int enpassantSquare = board.getEnpassantSquare();
     int halfMoveClock = board.getHalfMoveClock();
     int moveNumber = board.getMoveNumber();
     bool isEndgame = board.getEndgame();

     if (depth == 1) { return moveList.getCounter(); }

     for (int i = 0; i <moveList.getCounter(); i++) {

          int initial = moveList.getMove(i).getInitial();
          int terminal = moveList.getMove(i).getTerminal();

          capturedPiece = board.makeMove(moveList.getMove(i));

          node += divide(depth - 1, maxDepth, board, showOutput);
          if (showOutput) {
               individualNode = divide(depth - 1, maxDepth, board, false);
          }

          if (depth >= maxDepth && showOutput) {
               for (int i = 0; i < 3 - depth; i++) { printf("  "); }
               printf("%c%d%c%d: %llu", numberToFile(initial), numberToRank(initial),
                    numberToFile(terminal), numberToRank(terminal), individualNode);
               printf("\n");
          }

          undoMove(board, moveList.getMove(i), capturedPiece);
          board.setCastlingRights(castlingRights);
          board.setEnpassantSquare(enpassantSquare);
          board.setHalfMoveClock(halfMoveClock);
          board.setMoveNumber(moveNumber);
          board.setEndgame(isEndgame);
     }
     return node;

}
uint64_t divide2(const int depth, const int maxDepth, Board& board, const bool showOutput) {

     if (depth == 0) { return 1; }

     //  output text file for large output
     std::ofstream output2;
     output2.open("divide.txt");

     uint64_t node = 0, individualNode = 0;
     int capturedPiece;

     MoveList moveList = moveGeneration(board);
     std::array<bool, 4> castlingRights = board.getCastlingRights();
     int enpassantSquare = board.getEnpassantSquare();
     int halfMoveClock = board.getHalfMoveClock();
     int moveNumber = board.getMoveNumber();
     bool isEndgame = board.getEndgame();

     //if (depth == 1) { return depthLegalMoveCount[depth]; }

     for (int i = 0; i < moveList.getCounter(); i++) {
          int initial = moveList.getMove(i).getInitial();
          int terminal = moveList.getMove(i).getTerminal();

          capturedPiece = board.makeMove(moveList.getMove(i));


          node += divide(depth - 1, maxDepth, board, showOutput);
          if (showOutput) {
               individualNode = divide(depth - 1, maxDepth, board, false);
          }

          if (depth >= maxDepth && showOutput) {
               output2 << numberToFile(initial) << numberToRank(initial) <<
                    numberToFile(terminal) << numberToRank(terminal) << ": " << individualNode << std::endl;
          }


          undoMove(board, moveList.getMove(i), capturedPiece);
          board.setCastlingRights(castlingRights);
          board.setEnpassantSquare(enpassantSquare);
          board.setHalfMoveClock(halfMoveClock);
          board.setMoveNumber(moveNumber);
          board.setEndgame(isEndgame);
     }
     return node;
     output2.close();
}