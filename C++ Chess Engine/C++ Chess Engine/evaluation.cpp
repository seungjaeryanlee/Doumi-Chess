#include "evaluation.h"
#include "movegen.h"


int alphabeta(const int depth, Board& board, int alpha, int beta) {   
     switch (checkGameState(board)) {
     case NOTMATE:
          break;
     case WHITE_CHECKMATE:
          return -1 * (MATE_VALUE + depth);
          break;
     case BLACK_CHECKMATE:
          return (MATE_VALUE + depth);
          break;
     case STALEMATE_3F:
     case STALEMATE_50:
     case STALEMATE_75:
          return 0;
          break;
     }

     if (depth == 0) {
          
          return board.getTurn() * board.boardEvaluation();
     }

     int score;
     int capturedPiece;

     Board oldBoard = board;
     MoveList moveList = moveGeneration(board);

     for (int i = 0; i < moveList.getCounter(); i++) {
          capturedPiece = makeMove(board, moveList.getMove(i));

          score = -alphabeta(depth - 1, board, -beta, -alpha);

          if (score >= beta) {
               board = oldBoard;
               return beta;
          }

          if (score > alpha) {
               alpha = score;
          }
          board = oldBoard;
     }

     return alpha;
}
int rootAlphabeta(const int maxDepth, Board board, int alpha, int beta, Move& bestMove) {
     int score;
     int capturedPiece;

     Board oldBoard = board;
     MoveList moveList = moveGeneration(board);
     

     for (int i = 0; i < moveList.getCounter(); i++) {
          capturedPiece = makeMove(board, moveList.getMove(i));

          score = -alphabeta(maxDepth - 1, board, -beta, -alpha);
          
          // TODO: Check if this is needed and change it
          if (score >= beta) {
               board = oldBoard;
               return beta;
          }

          if (score > alpha) {
               alpha = score;
               bestMove = moveList.getMove(i);
          }
          board = oldBoard;
     }

     return alpha;
}