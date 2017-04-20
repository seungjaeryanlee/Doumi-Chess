/******************************************************************************/
/* evaluation.cpp                                                             */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#include "evaluation.h"
#include "movegen.h"
#include <fstream>
#include <string>

// Principal Variation:
// https://chessprogramming.wikispaces.com/Principal+Variation

int alphabeta(const int depth, Board& board, int alpha, int beta, Variation* pVariation, Board savedBoard[MAX_MOVENUMBER], int saveIndex) {

     switch (checkGameState(board, savedBoard, saveIndex)) {
     case NOTMATE:
          break;
     case WHITE_CHECKMATE:
          pVariation->length = 0;
          return -1 * (MATE_VALUE + depth);
          break;
     case BLACK_CHECKMATE:
          pVariation->length = 0;
          return (MATE_VALUE + depth);
          break;
     case STALEMATE_3F: case STALEMATE_50:
          if (board.getTurn() * board.evaluate() <= STALEMATE_BOUND) {
               pVariation->length = 0;
               return 0;
          }
          break;
     case STALEMATE_75: case STALEMATE_MOVE:
          pVariation->length = 0;
          return 0;
          break;
     }

     if (depth == 0) {
          pVariation->length = 0;
          return board.getTurn() * board.evaluate();
     }

     Variation variation;
     Board oldBoard = board;
     MoveList moveList = moveGeneration(board);
     for (int i = 0; i < moveList.getCounter(); i++) {
          int capturedPiece = makeMove(board, moveList.getMove(i));
          
          savedBoard[saveIndex] = board;
          int score = -alphabeta(depth - 1, board, -beta, -alpha, &variation, savedBoard, saveIndex + 1);
          board = oldBoard;

          if (score >= beta) {
               return beta;
          }
          if (score > alpha) {
               alpha = score;
               pVariation->moves[0] = moveList.getMove(i); 
               for (int j = 0; j < variation.length; j++) {
                    pVariation->moves[j + 1] = variation.moves[j];
               }
               pVariation->length = variation.length + 1;
          }
     }

     return alpha;
}
int rootAlphabeta(const int maxDepth, Board board, Variation* principalVariation, Board savedBoard[MAX_MOVENUMBER], int saveIndex) {
     Variation variation;
     Board oldBoard = board;
     MoveList moveList = moveGeneration(board);

     int alpha = DEFAULT_ALPHA;
     int beta = DEFAULT_BETA;
     for (int i = 0; i < moveList.getCounter(); i++) {
          int capturedPiece = makeMove(board, moveList.getMove(i));
          
          savedBoard[saveIndex] = board;
          int score = -alphabeta(maxDepth - 1, board, -beta, -alpha, &variation, savedBoard, saveIndex + 1);
          board = oldBoard;

          if (score > alpha) {
               alpha = score;
               principalVariation->moves[0] = moveList.getMove(i);
               for (int j = 0; j < variation.length; j++) {
                    principalVariation->moves[j + 1] = variation.moves[j];
               }
               principalVariation->length = variation.length + 1;
          }
     }
     return alpha;
}
gameState checkGameState(Board& board, const Board savedBoard[MAX_MOVENUMBER], int saveIndex) {
     // Stalemate: 75 Move Rule
     if (board.getHalfMoveClock() >= 150) {
          return STALEMATE_75;
     }

     int kingPos = -1;
     for (int i = 0; i < 120; i++) {
          if (board.getSquare(i) == WHITEKING && board.getTurn() == WHITE) {
               kingPos = i;
               break;
          }
          if (board.getSquare(i) == BLACKKING && board.getTurn() == BLACK) {
               kingPos = i;
               break;
          }
     }
     MoveList moveList = moveGeneration(board);
     // Checkmate
     if (moveList.getCounter() == 0 && board.isAttacked(kingPos)) {
          if (board.getSquare(kingPos) == WHITE) {
               return BLACK_CHECKMATE;
          }
          else {
               return WHITE_CHECKMATE;
          }
     }
     // Stalemate: No Legal Move
     if (moveList.getCounter() == 0) {
          return STALEMATE_MOVE;
     }

     // Stalemate: 50 Move Rule
     if (fiftyMoveCheck(board)) {
          return STALEMATE_50;
     }

     // Stalemate: Threefold Repetition
     int repetitionCount = 0;
     for (int i = 0; i < saveIndex; i++) {
          if (board.isEqual_3F(savedBoard[i])) {
               repetitionCount++;
          }
          if (repetitionCount >= 3) {
               return STALEMATE_3F;
          }
     }

     return NOTMATE;
}

void printVariation(std::ostream& stream, Variation& var) {
     for (int i = 0; i < var.length; i++) {
          stream << numberToFilerank(var.moves[i].getInitial())
               << numberToFilerank(var.moves[i].getTerminal())
               << " ";
     }
     stream << std::endl;
}