#include "evaluation.h"
#include "movegen.h"


int alphabeta(const int depth, Board& board, int alpha, int beta, Board savedBoard[MAX_MOVENUMBER], int saveIndex) {   
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

     // Check 50 move rule
     if (fiftyMoveCheck(board)) {
          // If bad, declare stalemate
          // TODO: Should the check be deeper before stalemate is claimed?
          if (board.getTurn() * board.boardEvaluation() <= STALEMATE_BOUND) {
               printf("Declare 50\n");
               return 0;
          }
     }

     // Check Threefold Repetition
     // TODO: Pass savedBoard + saveIndex as arguments
     int repetitionCount = 0;
     for (int i = 0; i < saveIndex; i++) {
          if (board.isAlmostEqual(savedBoard[i])) {
               repetitionCount++;
          }
          if (repetitionCount >= 3) { 
               // If bad, declare stalemate
               // TODO: Should the check be deeper before stalemate is claimed?
               if (board.getTurn() * board.boardEvaluation() <= STALEMATE_BOUND) {
                    printf("Declare 3Fold\n");
                    return 0;
               }
          }
     }

     if (depth == 0) {
          return board.getTurn() * board.boardEvaluation();
     }

     Board oldBoard = board;
     MoveList moveList = moveGeneration(board);
     for (int i = 0; i < moveList.getCounter(); i++) {
          int capturedPiece = makeMove(board, moveList.getMove(i));
          
          savedBoard[saveIndex] = board;

          int score = -alphabeta(depth - 1, board, -beta, -alpha, savedBoard, saveIndex + 1);

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
int rootAlphabeta(const int maxDepth, Board board, int alpha, int beta, Move& bestMove, Board savedBoard[MAX_MOVENUMBER], int saveIndex) {
     int score;
     int capturedPiece;

     Board oldBoard = board;
     MoveList moveList = moveGeneration(board);
     

     for (int i = 0; i < moveList.getCounter(); i++) {
          capturedPiece = makeMove(board, moveList.getMove(i));
          savedBoard[saveIndex] = board;

          score = -alphabeta(maxDepth - 1, board, -beta, -alpha, savedBoard, saveIndex + 1);
          
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

int alphabeta(const int depth, Board& board, int alpha, int beta, Move bestMoves[MAX_DEPTH], Board savedBoard[MAX_MOVENUMBER], int saveIndex) {
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

     // Check 50 move rule
     if (fiftyMoveCheck(board)) {
          // If bad, declare stalemate
          // TODO: Should the check be deeper before stalemate is claimed?
          if (board.getTurn() * board.boardEvaluation() <= STALEMATE_BOUND) {
               printf("Declare 50\n");
               return 0;
          }
     }

     // Check Threefold Repetition
     // TODO: Pass savedBoard + saveIndex as arguments
     int repetitionCount = 0;
     for (int i = 0; i < saveIndex; i++) {
          if (board.isAlmostEqual(savedBoard[i])) {
               repetitionCount++;
          }
          if (repetitionCount >= 3) {
               // If bad, declare stalemate
               // TODO: Should the check be deeper before stalemate is claimed?
               if (board.getTurn() * board.boardEvaluation() <= STALEMATE_BOUND) {
                    printf("Declare 3Fold\n");
                    return 0;
               }
          }
     }

     if (depth == 0) {
          return board.getTurn() * board.boardEvaluation();
     }

     Board oldBoard = board;
     MoveList moveList = moveGeneration(board);
     for (int i = 0; i < moveList.getCounter(); i++) {
          int capturedPiece = makeMove(board, moveList.getMove(i));

          savedBoard[saveIndex] = board;

          int score = -alphabeta(depth - 1, board, -beta, -alpha, bestMoves, savedBoard, saveIndex + 1);

          if (score >= beta) {
               board = oldBoard;
               return beta;
          }

          if (score > alpha) {
               alpha = score;
               bestMoves[depth] = moveList.getMove(i);
          }
          board = oldBoard;
     }

     return alpha;
}
int rootAlphabeta(const int maxDepth, Board board, int alpha, int beta, Move bestMoves[MAX_DEPTH], Board savedBoard[MAX_MOVENUMBER], int saveIndex) {
     int score;
     int capturedPiece;

     Board oldBoard = board;
     MoveList moveList = moveGeneration(board);


     for (int i = 0; i < moveList.getCounter(); i++) {
          capturedPiece = makeMove(board, moveList.getMove(i));
          savedBoard[saveIndex] = board;

          score = -alphabeta(maxDepth - 1, board, -beta, -alpha, bestMoves, savedBoard, saveIndex + 1);

          // TODO: Check if this is needed and change it
          if (score >= beta) {
               board = oldBoard;
               return beta;
          }

          if (score > alpha) {
               alpha = score;
               bestMoves[maxDepth] = moveList.getMove(i);
          }
          board = oldBoard;
     }

     return alpha;
}
gameState checkGameState(const Board& board) {
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

     // Stalemate: 75 Move Rule
     // TODO: 50 Move rule will be implemented in moveGen
     if (board.getHalfMoveClock() >= 150) {
          return STALEMATE_75;
     }

     MoveList moveList = moveGeneration(board);

     // Checkmate
     if (moveList.getCounter() == 0 && isSquareAttacked(board, kingPos)) {
          if (board.getSquare(kingPos) == WHITE) {
               return BLACK_CHECKMATE;
          }
          else {
               return WHITE_CHECKMATE;
          }
     }

     // Stalemate: No legal move
     if (moveList.getCounter() == 0) {
          return STALEMATE_MOVE;
     }

     return NOTMATE;
}