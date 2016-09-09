#include "evaluation.h"
#include "defs.h"
#include "movegen.h"

int boardEvaluation(const Board& board) {
     int score = 0;
     for (int i = 0; i < 120; i++) {
          switch (board.getSquare(i)) {
          case WHITEPAWN:
               score += PAWNVALUE;
               score += PAWN_PCSQTable.at(i);
               break;
          case WHITEKNIGHT:
               score += KNIGHTVALUE;
               score += KNIGHT_PCSQTable.at(i);
               break;
          case WHITEBISHOP:
               score += BISHOPVALUE;
               score += BISHOP_PCSQTable.at(i);
               break;
          case WHITEROOK:
               score += ROOKVALUE;
               score += ROOK_PCSQTable.at(i);
               break;
          case WHITEQUEEN:
               score += QUEENVALUE;
               score += QUEEN_PCSQTable.at(i);
               break;
          case WHITEKING:
               score += KINGVALUE;
               if (board.getEndgame()) {
                    score += KING_PCSQTable_ENDGAME.at(i);
               }
               else {
                    score += KING_PCSQTable.at(i);
               }
               break;
          case BLACKPAWN:
               score -= PAWNVALUE;
               score -= PAWN_PCSQTable.at(reversePosition(i));
               break;
          case BLACKKNIGHT:
               score -= KNIGHTVALUE;
               score -= KNIGHT_PCSQTable.at(reversePosition(i));
               break;
          case BLACKBISHOP:
               score -= BISHOPVALUE;
               score -= BISHOP_PCSQTable.at(reversePosition(i));
               break;
          case BLACKROOK:
               score -= ROOKVALUE;
               score -= ROOK_PCSQTable.at(reversePosition(i));
               break;
          case BLACKQUEEN:
               score -= QUEENVALUE;
               score -= QUEEN_PCSQTable.at(reversePosition(i));
               break;
          case BLACKKING:
               score -= KINGVALUE;
               if (board.getEndgame()) {
                    score -= KING_PCSQTable_ENDGAME.at(reversePosition(i));
               }
               else {
                    score -= KING_PCSQTable.at(reversePosition(i));
               }
               break;
          }
     }
     return score;
}
int reversePosition(const int position) {
     int row = position / 10, column = position % 10;
     return (12 - row - 1) * 10 + column;
}

int negaMax(const int depth, Board& board) {
     gameState state = checkGameState(board);
     if (state != NOTMATE) {
          printf("Terminal node!: %d\n", depth);
          switch (state) {
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
     }
     if (depth == 0) {
          return board.getTurn() * boardEvaluation(board);
     }
     int max_Score = INT_MIN;
     int score;
     int capturedPiece;

     MoveList moveList = moveGeneration(board);

     for (int i = 0; i <moveList.getCounter(); i++) {

          updateCastling(board, moveList.getMove(i));
          int enpassantSquare = board.getEnpassantSquare();

          capturedPiece = makeMove(board, moveList.getMove(i));

          score = -negaMax(depth - 1, board);

          if (score > max_Score) {
               max_Score = score;
          }

          undoMove(board, moveList.getMove(i), capturedPiece);
          board.setEnpassantSquare(enpassantSquare);
     }

     return max_Score;
}
int rootNegaMax(const int maxDepth, Board& board, Move& bestMove) {

     int max_Score = INT_MIN;
     int score;
     int capturedPiece;

     MoveList moveList = moveGeneration(board);

     for (int i = 0; i < moveList.getCounter(); i++) {
          updateCastling(board, moveList.getMove(i));

          int enpassantSquare = board.getEnpassantSquare();
          capturedPiece = makeMove(board, moveList.getMove(i));

          score = -negaMax(maxDepth - 1, board);

          if (score > max_Score) {
               max_Score = score;
               bestMove = moveList.getMove(i);
          }

          undoMove(board, moveList.getMove(i), capturedPiece);
          board.setEnpassantSquare(enpassantSquare);
     }

     return max_Score;
}

// REMINDER: Whatever is changed here should be changed in negaMax and divide too
int alphabeta(const int depth, Board& board, int alpha, int beta) {
     gameState state = checkGameState(board);
     if (state != NOTMATE) {
          printf("Terminal node!: %d\n", depth);
          switch (state) {
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
     }
     if (depth == 0) {
          return board.getTurn() * boardEvaluation(board);
     }

     int score;
     int capturedPiece;

     MoveList moveList = moveGeneration(board);
     Board savedBoard = board;
     for (int i = 0; i < moveList.getCounter(); i++) {

          updateCastling(board, moveList.getMove(i));

          // Save enpassantSquare so it doesn't get lost while making move
          int enpassantSquare = board.getEnpassantSquare();

          capturedPiece = makeMove(board, moveList.getMove(i));

          score = -alphabeta(depth - 1, board, -beta, -alpha);

          if (score >= beta) {
               board = savedBoard;
               return beta;
          }

          if (score > alpha) {
               alpha = score;
          }
          board = savedBoard;
     }

     return alpha;
}
int rootAlphabeta(const int maxDepth, Board board, int alpha, int beta, Move& bestMove) {
     int score;
     int capturedPiece;

     MoveList moveList = moveGeneration(board);
     Board savedBoard = board;
     for (int i = 0; i < moveList.getCounter(); i++) {

          updateCastling(board, moveList.getMove(i));
          int enpassantSquare = board.getEnpassantSquare();
          capturedPiece = makeMove(board, moveList.getMove(i));

          score = -alphabeta(maxDepth - 1, board, -beta, -alpha);

          // TODO: Check if this is needed and change it
          if (score >= beta) {

               board = savedBoard;
               return beta;
          }

          if (score > alpha) {
               alpha = score;
               bestMove = moveList.getMove(i);
          }

          board = savedBoard;
     }

     return alpha;
}