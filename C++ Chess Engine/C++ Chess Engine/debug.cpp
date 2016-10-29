#include "debug.h"
#include "move.h"
#include "board.h"
#include "movegen.h"
#include <fstream>

void printDebugMenu() {
     printf("--------------------------------------------------\n");
     printf("%d: Make move\n", MOVE);
     printf("%d: Display Board\n", DISPLAY_BOARD);
     printf("%d: Display FEN\n", DISPLAY_FEN);
     printf("%d: Reset Board\n", BOARD_RESET);
     printf("%d: Perft Test\n", PERFT);
     printf("%d: Quit\n", QUIT);
     printf("%d: Divide Perft Test\n", DIVIDE);
     printf("%d: Undo move\n", UNDO_MOVE);
     printf("%d: Computer Make Move\n", COM_MAKE_MOVE);
     printf("A: Print Possible Moves\n");
     printf("B: Negamax vs. Negamax + Alphabeta Pruning\n");
     printf("C: Evaluate Board\n");
     printf("D: Alphabeta Speed Check\n");
     printf("E: Print Saved FEN\n");
     printf("--------------------------------------------------\n");
     printf("Please choose command: ");
}

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

          capturedPiece = makeMove(board, moveList.getMove(i));

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

          capturedPiece = makeMove(board, moveList.getMove(i));


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