#include "evaluation.h"
#include "defs.h"
#include "movegen.h"

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
          return board.getTurn() * board.boardEvaluation();
     }
     int max_Score = INT_MIN;
     int score;
     int capturedPiece;

     MoveList moveList = moveGeneration(board);
     std::array<bool, 4> castlingRights = board.getCastlingRights();
     int enpassantSquare = board.getEnpassantSquare();
     int halfMoveClock = board.getHalfMoveClock();
     int moveNumber = board.getMoveNumber();
     bool isEndgame = board.getEndgame();

     for (int i = 0; i <moveList.getCounter(); i++) {

          capturedPiece = makeMove(board, moveList.getMove(i));
          updateBoard(board, moveList.getMove(i));

          score = -negaMax(depth - 1, board);

          if (score > max_Score) {
               max_Score = score;
          }

          undoMove(board, moveList.getMove(i), capturedPiece);
          board.setCastlingRights(castlingRights);
          board.setEnpassantSquare(enpassantSquare);
          board.setHalfMoveClock(halfMoveClock);
          board.setMoveNumber(moveNumber);
          board.setEndgame(isEndgame);
     }

     return max_Score;
}
int rootNegaMax(const int maxDepth, Board& board, Move& bestMove) {

     int max_Score = INT_MIN;
     int score;
     int capturedPiece;

     MoveList moveList = moveGeneration(board);
     std::array<bool, 4> castlingRights = board.getCastlingRights();
     int enpassantSquare = board.getEnpassantSquare();
     int halfMoveClock = board.getHalfMoveClock();
     int moveNumber = board.getMoveNumber();
     bool isEndgame = board.getEndgame();

     for (int i = 0; i < moveList.getCounter(); i++) {     
          capturedPiece = makeMove(board, moveList.getMove(i));
          updateBoard(board, moveList.getMove(i));

          score = -negaMax(maxDepth - 1, board);

          if (score > max_Score) {
               max_Score = score;
               bestMove = moveList.getMove(i);
          }

          undoMove(board, moveList.getMove(i), capturedPiece);
          board.setCastlingRights(castlingRights);
          board.setEnpassantSquare(enpassantSquare);
          board.setHalfMoveClock(halfMoveClock);
          board.setMoveNumber(moveNumber);
          board.setEndgame(isEndgame);
     }

     return max_Score;
}

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
          return board.getTurn() * board.boardEvaluation();
     }

     int score;
     int capturedPiece;

     MoveList moveList = moveGeneration(board);
     std::array<bool, 4> castlingRights = board.getCastlingRights();
     int enpassantSquare = board.getEnpassantSquare();
     int halfMoveClock = board.getHalfMoveClock();
     int moveNumber = board.getMoveNumber();
     bool isEndgame = board.getEndgame();


     for (int i = 0; i < moveList.getCounter(); i++) {
          capturedPiece = makeMove(board, moveList.getMove(i));
          updateBoard(board, moveList.getMove(i));

          score = -alphabeta(depth - 1, board, -beta, -alpha);

          if (score >= beta) {
               undoMove(board, moveList.getMove(i), capturedPiece);
               board.setCastlingRights(castlingRights);
               board.setEnpassantSquare(enpassantSquare);
               board.setHalfMoveClock(halfMoveClock);
               board.setMoveNumber(moveNumber);
               board.setEndgame(isEndgame);
               return beta;
          }

          if (score > alpha) {
               alpha = score;
          }
          undoMove(board, moveList.getMove(i), capturedPiece);
          board.setCastlingRights(castlingRights);
          board.setEnpassantSquare(enpassantSquare);
          board.setHalfMoveClock(halfMoveClock);
          board.setMoveNumber(moveNumber);
          board.setEndgame(isEndgame);
     }

     return alpha;
}
int rootAlphabeta(const int maxDepth, Board board, int alpha, int beta, Move& bestMove) {
     int score;
     int capturedPiece;

     MoveList moveList = moveGeneration(board);
     std::array<bool, 4> castlingRights = board.getCastlingRights();
     int enpassantSquare = board.getEnpassantSquare();
     int halfMoveClock = board.getHalfMoveClock();
     int moveNumber = board.getMoveNumber();
     bool isEndgame = board.getEndgame();
     for (int i = 0; i < moveList.getCounter(); i++) {
          capturedPiece = makeMove(board, moveList.getMove(i));
          updateBoard(board, moveList.getMove(i));

          score = -alphabeta(maxDepth - 1, board, -beta, -alpha);

          // TODO: Check if this is needed and change it
          if (score >= beta) {
               undoMove(board, moveList.getMove(i), capturedPiece);
               board.setCastlingRights(castlingRights);
               board.setEnpassantSquare(enpassantSquare);
               board.setHalfMoveClock(halfMoveClock);
               board.setMoveNumber(moveNumber);
               board.setEndgame(isEndgame);
               return beta;
          }

          if (score > alpha) {
               alpha = score;
               bestMove = moveList.getMove(i);
          }

          undoMove(board, moveList.getMove(i), capturedPiece);
          board.setCastlingRights(castlingRights);
          board.setEnpassantSquare(enpassantSquare);
          board.setHalfMoveClock(halfMoveClock);
          board.setMoveNumber(moveNumber);
          board.setEndgame(isEndgame);
     }

     return alpha;
}