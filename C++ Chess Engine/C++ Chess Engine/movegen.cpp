#include <stdio.h>
#include "defs.h"
#include "protos.h"
#include "movegen.h"

void moveGeneration(const Board& board, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount) {
     *moveCount = 0;

     castlingMoveGeneration(board, moveList, moveCount);
     enpassantMoveGeneration(board, moveList, moveCount);

     if (board.getTurn() == WHITE) {
          for (int i = 0; i < 120; i++) {
               switch (board.getSquare(i)) {
               case WHITEPAWN:
                    pawnMoveGeneration(board, i, moveList, moveCount);
                    break;
               case WHITEKNIGHT:
                    knightMoveGeneration(board, i, moveList, moveCount);
                    break;
               case WHITEBISHOP:
                    bishopMoveGeneration(board, i, moveList, moveCount);
                    break;
               case WHITEROOK:
                    rookMoveGeneration(board, i, moveList, moveCount);
                    break;
               case WHITEQUEEN:
                    queenMoveGeneration(board, i, moveList, moveCount);
                    break;
               case WHITEKING:
                    kingMoveGeneration(board, i, moveList, moveCount);
                    break;
               }
          }
     }
     if (board.getTurn() == BLACK) {
          for (int i = 0; i < 120; i++) {
               switch (board.getSquare(i)) {
               case BLACKPAWN:
                    pawnMoveGeneration(board, i, moveList, moveCount);
                    break;
               case BLACKKNIGHT:
                    knightMoveGeneration(board, i, moveList, moveCount);
                    break;
               case BLACKBISHOP:
                    bishopMoveGeneration(board, i, moveList, moveCount);
                    break;
               case BLACKROOK:
                    rookMoveGeneration(board, i, moveList, moveCount);
                    break;
               case BLACKQUEEN:
                    queenMoveGeneration(board, i, moveList, moveCount);
                    break;
               case BLACKKING:
                    kingMoveGeneration(board, i, moveList, moveCount);
                    break;
               }
          }
     }
}
void pawnMoveGeneration(const Board& board, int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount) {
     if (board.getTurn() == WHITE) {
          //  if on the last row before promotion, just call promotion
          if (A7 <= position && position <= H7) {
               promotionMoveGeneration(board, position, moveList, moveCount);
               return;
          }

          //  Advance 1 square
          if (board.getSquare(position - ROW) == EMPTYSQUARE) {
               addMove(position, position - ROW, NORMAL, moveList, moveCount);
               //  Advance 2 squares
               if (A2 <= position && position <= H2 &&
                    board.getSquare(position - 2 * ROW) == EMPTYSQUARE) {
                    addMove(position, position - 2 * ROW, DOUBLEMOVE, moveList, moveCount);
               }
          }

          //  attack diagonals
          if (checkColor(board.getSquare(position - ROW - COLUMN)) == BLACK) {
               addMove(position, position - ROW - COLUMN, NORMAL, moveList, moveCount);
          }
          if (checkColor(board.getSquare(position - ROW + COLUMN)) == BLACK) {
               addMove(position, position - ROW + COLUMN, NORMAL, moveList, moveCount);
          }
     }
     if (board.getTurn() == BLACK) {
          //  if on the last row before promotion, just call promotion
          if (A2 <= position && position <= H2) {
               promotionMoveGeneration(board, position, moveList, moveCount);
               return;
          }

          //  Advance 1 square
          if (board.getSquare(position + ROW) == EMPTYSQUARE) {
               addMove(position, position + ROW, NORMAL, moveList, moveCount);
               //  Advance 2 squares
               if (A7 <= position && position <= H7 &&
                    board.getSquare(position + 2 * ROW) == EMPTYSQUARE) {
                    addMove(position, position + 2 * ROW, DOUBLEMOVE, moveList, moveCount);
               }
          }

          //  attack diagonals
          if (checkColor(board.getSquare(position + ROW - COLUMN)) == WHITE) {
               addMove(position, position + ROW - COLUMN, NORMAL, moveList, moveCount);
          }
          if (checkColor(board.getSquare(position + ROW + COLUMN)) == WHITE) {
               addMove(position, position + ROW + COLUMN, NORMAL, moveList, moveCount);
          }
     }
}
void knightMoveGeneration(const Board& board, int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount) {
     int turn = board.getTurn();

     if (checkColor(board.getSquare(position + ROW + 2 * COLUMN)) == -turn ||
          board.getSquare(position + ROW + 2 * COLUMN) == EMPTYSQUARE) {
          addMove(position, position + ROW + 2 * COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position + ROW - 2 * COLUMN)) == -turn ||
          board.getSquare(position + ROW - 2 * COLUMN) == EMPTYSQUARE) {
          addMove(position, position + ROW - 2 * COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position - ROW + 2 * COLUMN)) == -turn ||
          board.getSquare(position - ROW + 2 * COLUMN) == EMPTYSQUARE) {
          addMove(position, position - ROW + 2 * COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position - ROW - 2 * COLUMN)) == -turn ||
          board.getSquare(position - ROW - 2 * COLUMN) == EMPTYSQUARE) {
          addMove(position, position - ROW - 2 * COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position + 2 * ROW + COLUMN)) == -turn ||
          board.getSquare(position + 2 * ROW + COLUMN) == EMPTYSQUARE) {
          addMove(position, position + 2 * ROW + COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position + 2 * ROW - COLUMN)) == -turn ||
          board.getSquare(position + 2 * ROW - COLUMN) == EMPTYSQUARE) {
          addMove(position, position + 2 * ROW - COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position - 2 * ROW + COLUMN)) == -turn ||
          board.getSquare(position - 2 * ROW + COLUMN) == EMPTYSQUARE) {
          addMove(position, position - 2 * ROW + COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position - 2 * ROW - COLUMN)) == -turn ||
          board.getSquare(position - 2 * ROW - COLUMN) == EMPTYSQUARE) {
          addMove(position, position - 2 * ROW - COLUMN, NORMAL, moveList, moveCount);
     }
}
void bishopMoveGeneration(const Board& board, int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount) {
     int turn = board.getTurn();
     bool topright = true, downright = true, downleft = true, topleft = true;
     for (int i = 1; i < 8; i++) {
          if (topright == true &&
               (checkColor(board.getSquare(position - i*ROW + i*COLUMN)) == -turn ||
                    board.getSquare(position - i*ROW + i*COLUMN) == EMPTYSQUARE)) {
               addMove(position, position - i*ROW + i*COLUMN, NORMAL, moveList, moveCount);
               if (checkColor(board.getSquare(position - i*ROW + i*COLUMN)) == -turn) {
                    topright = false;
               }
          }
          else { topright = false; }

          if (downright == true &&
               (checkColor(board.getSquare(position + i*ROW + i*COLUMN)) == -turn ||
                    board.getSquare(position + i*ROW + i*COLUMN) == EMPTYSQUARE)) {
               addMove(position, position + i*ROW + i*COLUMN, NORMAL, moveList, moveCount);
               if (checkColor(board.getSquare(position + i*ROW + i*COLUMN)) == -turn) {
                    downright = false;
               }
          }
          else { downright = false; }

          if (downleft == true &&
               (checkColor(board.getSquare(position + i*ROW - i*COLUMN)) == -turn ||
                    board.getSquare(position + i*ROW - i*COLUMN) == EMPTYSQUARE)) {
               addMove(position, position + i*ROW - i*COLUMN, NORMAL, moveList, moveCount);
               if (checkColor(board.getSquare(position + i*ROW - i*COLUMN)) == -turn) {
                    downleft = false;
               }
          }
          else { downleft = false; }

          if (topleft == true &&
               (checkColor(board.getSquare(position - i*ROW - i*COLUMN)) == -turn ||
                    board.getSquare(position - i*ROW - i*COLUMN) == EMPTYSQUARE)) {
               addMove(position, position - i*ROW - i*COLUMN, NORMAL, moveList, moveCount);
               if (checkColor(board.getSquare(position - i*ROW - i*COLUMN)) == -turn) {
                    topleft = false;
               }
          }
          else { topleft = false; }
     }
}
void rookMoveGeneration(const Board& board, int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount) {
     int turn = board.getTurn();
     bool top = true, right = true, down = true, left = true;

     for (int i = 1; i < 8; i++) {
          if (top == true &&
               (checkColor(board.getSquare(position - i*ROW)) == -turn ||
                    board.getSquare(position - i*ROW) == EMPTYSQUARE)) {
               addMove(position, position - i*ROW, NORMAL, moveList, moveCount);
               if (checkColor(board.getSquare(position - i*ROW)) == -turn) {
                    top = false;
               }
          }
          else { top = false; }

          if (right == true &&
               (checkColor(board.getSquare(position + i*COLUMN)) == -turn ||
                    board.getSquare(position + i*COLUMN) == EMPTYSQUARE)) {
               addMove(position, position + i*COLUMN, NORMAL, moveList, moveCount);
               if (checkColor(board.getSquare(position + i*COLUMN)) == -turn) {
                    right = false;
               }
          }
          else { right = false; }

          if (down == true &&
               (checkColor(board.getSquare(position + i*ROW)) == -turn ||
                    board.getSquare(position + i*ROW) == EMPTYSQUARE)) {
               addMove(position, position + i*ROW, NORMAL, moveList, moveCount);
               if (checkColor(board.getSquare(position + i*ROW)) == -turn) {
                    down = false;
               }
          }
          else { down = false; }

          if (left == true &&
               (checkColor(board.getSquare(position - i*COLUMN)) == -turn ||
                    board.getSquare(position - i*COLUMN) == EMPTYSQUARE)) {
               addMove(position, position - i*COLUMN, NORMAL, moveList, moveCount);
               if (checkColor(board.getSquare(position - i*COLUMN)) == -turn) {
                    left = false;
               }
          }
          else { left = false; }
     }
}
void queenMoveGeneration(const Board& board, int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount) {
     rookMoveGeneration(board, position, moveList, moveCount);
     bishopMoveGeneration(board, position, moveList, moveCount);
}
void kingMoveGeneration(const Board& board, int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount) {
     int turn = board.getTurn();

     if (checkColor(board.getSquare(position + ROW)) == -turn ||
          board.getSquare(position + ROW) == EMPTYSQUARE) {
          addMove(position, position + ROW, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position - ROW)) == -turn ||
          board.getSquare(position - ROW) == EMPTYSQUARE) {
          addMove(position, position - ROW, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position + COLUMN)) == -turn ||
          board.getSquare(position + COLUMN) == EMPTYSQUARE) {
          addMove(position, position + COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position - COLUMN)) == -turn ||
          board.getSquare(position - COLUMN) == EMPTYSQUARE) {
          addMove(position, position - COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position + ROW + COLUMN)) == -turn ||
          board.getSquare(position + ROW + COLUMN) == EMPTYSQUARE) {
          addMove(position, position + ROW + COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position + ROW - COLUMN)) == -turn ||
          board.getSquare(position + ROW - COLUMN) == EMPTYSQUARE) {
          addMove(position, position + ROW - COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position - ROW + COLUMN)) == -turn ||
          board.getSquare(position - ROW + COLUMN) == EMPTYSQUARE) {
          addMove(position, position - ROW + COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board.getSquare(position - ROW - COLUMN)) == -turn ||
          board.getSquare(position - ROW - COLUMN) == EMPTYSQUARE) {
          addMove(position, position - ROW - COLUMN, NORMAL, moveList, moveCount);
     }
}

void castlingMoveGeneration(const Board& board, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount) {
     if (board.getTurn() == WHITE) {
          if (board.getCastling(WKCASTLING) &&                                             //  neither piece moved
               board.getSquare(E1) == WHITEKING && board.getSquare(H1) == WHITEROOK &&     //  both pieces exists on board
               board.getSquare(F1) == EMPTYSQUARE && board.getSquare(G1) == EMPTYSQUARE && //  between them are empty
               squareAttackCheck(board, E1) == false &&                                    //  not in check
               squareAttackCheck(board, F1) == false &&                                    //  not attacked while moving
               squareAttackCheck(board, G1) == false) {

               addMove(E1, G1, KINGSIDE_CASTLING, moveList, moveCount);
          }
          if (board.getCastling(WQCASTLING) && board.getSquare(B1) == EMPTYSQUARE &&
               board.getSquare(C1) == EMPTYSQUARE && board.getSquare(D1) == EMPTYSQUARE &&
               board.getSquare(E1) == WHITEKING && board.getSquare(A1) == WHITEROOK &&
               squareAttackCheck(board, E1) == false &&
               squareAttackCheck(board, C1) == false &&
               squareAttackCheck(board, D1) == false) {
               addMove(E1, C1, QUEENSIDE_CASTLING, moveList, moveCount);
          }

     }
     if (board.getTurn() == BLACK) {
          if (board.getCastling(BKCASTLING) &&                                             //  neither piece moved
               board.getSquare(E8) == BLACKKING && board.getSquare(H8) == BLACKROOK &&     //  both pieces exists on board
               board.getSquare(F8) == EMPTYSQUARE && board.getSquare(G8) == EMPTYSQUARE && //  between them are empty
               squareAttackCheck(board, E8) == false &&                                    //  not in check
               squareAttackCheck(board, F8) == false &&                                    //  not attacked while moving
               squareAttackCheck(board, G8) == false) {

               addMove(E8, G8, KINGSIDE_CASTLING, moveList, moveCount);
          }
          if (board.getCastling(BQCASTLING) && board.getSquare(B8) == EMPTYSQUARE &&
               board.getSquare(C8) == EMPTYSQUARE && board.getSquare(D8) == EMPTYSQUARE &&
               board.getSquare(E8) == BLACKKING && board.getSquare(A8) == BLACKROOK &&
               squareAttackCheck(board, E8) == false &&
               squareAttackCheck(board, C8) == false &&
               squareAttackCheck(board, D8) == false) {
               addMove(E8, C8, QUEENSIDE_CASTLING, moveList, moveCount);
          }
     }
}
void promotionMoveGeneration(const Board& board, int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount) {
     if (board.getTurn() == WHITE) {
          if (checkColor(board.getSquare(position - ROW - COLUMN)) == -board.getTurn()) {
               addPromotionMove(position, position - ROW - COLUMN, moveList, moveCount);
          }
          if (checkColor(board.getSquare(position - ROW + COLUMN)) == -board.getTurn()) {
               addPromotionMove(position, position - ROW + COLUMN, moveList, moveCount);
          }
          if (board.getSquare(position - ROW) == EMPTYSQUARE) {
               addPromotionMove(position, position - ROW, moveList, moveCount);
          }
     }
     if (board.getTurn() == BLACK) {
          if (checkColor(board.getSquare(position + ROW - COLUMN)) == -board.getTurn()) {
               addPromotionMove(position, position + ROW - COLUMN, moveList, moveCount);
          }
          if (checkColor(board.getSquare(position + ROW + COLUMN)) == -board.getTurn()) {
               addPromotionMove(position, position + ROW + COLUMN, moveList, moveCount);
          }
          if (board.getSquare(position + ROW) == EMPTYSQUARE) {
               addPromotionMove(position, position + ROW, moveList, moveCount);
          }
     }
}
void enpassantMoveGeneration(const Board& board, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount) {
     if (board.getEnpassantSquare() == 0) { return; }

     int enpassantSquare = board.getEnpassantSquare();

     if (board.getTurn() == WHITE) {
          if (board.getSquare(enpassantSquare + ROW + COLUMN) == WHITEPAWN) {
               addMove(enpassantSquare + ROW + COLUMN, enpassantSquare, ENPASSANT, moveList, moveCount);
          }
          if (board.getSquare(enpassantSquare + ROW - COLUMN) == WHITEPAWN) {
               addMove(enpassantSquare + ROW - COLUMN, enpassantSquare, ENPASSANT, moveList, moveCount);
          }
     }
     if (board.getTurn() == BLACK) {
          if (board.getSquare(enpassantSquare - ROW + COLUMN) == BLACKPAWN) {
               addMove(enpassantSquare - ROW + COLUMN, enpassantSquare, ENPASSANT, moveList, moveCount);
          }
          if (board.getSquare(enpassantSquare - ROW - COLUMN) == BLACKPAWN) {
               addMove(enpassantSquare - ROW - COLUMN, enpassantSquare, ENPASSANT, moveList, moveCount);
          }
     }
}

void addMove(int initial, int terminal, int moveType, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount) {
     moveList[*moveCount] = Move(initial, terminal, moveType);
     *moveCount += 1;
}
void addPromotionMove(int initial, int terminal, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount) {
     addMove(initial, terminal, KNIGHT_PROMOTION, moveList, moveCount);
     addMove(initial, terminal, BISHOP_PROMOTION, moveList, moveCount);
     addMove(initial, terminal, ROOK_PROMOTION, moveList, moveCount);
     addMove(initial, terminal, QUEEN_PROMOTION, moveList, moveCount);
}

void legalMoves(Board board, Move moveList[MAX_MOVEGEN_COUNT], int moveCount, Move legalMoveList[MAX_MOVEGEN_COUNT], int *legalMoveCount) {
     *legalMoveCount = 0;

     //  find king position
     int kingPosition = 0, changedKingPosition = 0;
     int terminalValue;
     for (int i = 0; i < 120; i++) {
          if (board.getTurn() == WHITE && board.getSquare(i) == WHITEKING ||
               board.getTurn() == BLACK && board.getSquare(i) == BLACKKING) {
               kingPosition = i;
               break;
          }
     }

     for (int i = 0; i < moveCount; i++) {
          //  check if king will be moved
          if (board.getSquare(moveList[i].getInitial()) == WHITEKING || board.getSquare(moveList[i].getInitial()) == BLACKKING) {
               if (moveList[i].getType() == NORMAL) {
                    changedKingPosition = moveList[i].getTerminal();
               }
               if (moveList[i].getType() == KINGSIDE_CASTLING) {
                    changedKingPosition = moveList[i].getInitial() + 2 * COLUMN;
               }
               if (moveList[i].getType() == QUEENSIDE_CASTLING) {
                    changedKingPosition = moveList[i].getInitial() - 2 * COLUMN;
               }

          }
          else { changedKingPosition = kingPosition; }

          //  make move
          terminalValue = makeMove(board, moveList[i]);
          //  In this case, we don't want makeMove to change turn, so let's change it again
          board.changeTurn();

          //  if king is safe
          if (!squareAttackCheck(board, changedKingPosition)) {
               legalMoveList[*legalMoveCount] = Move(moveList[i]);
               *legalMoveCount += 1;
          }

          //  undo move
          undoMove(board, moveList[i], terminalValue);
          //  Same reason as above
          board.changeTurn();
     }
}
bool squareAttackCheck(Board board, int position) {
     if (board.getTurn() == WHITE) {
          //  1. pawn
          if (board.getSquare(position - ROW - COLUMN) == BLACKPAWN ||
               board.getSquare(position - ROW + COLUMN) == BLACKPAWN) {
               return true;
          }
          //  2. knight
          if (board.getSquare(position - ROW - 2 * COLUMN) == BLACKKNIGHT ||
               board.getSquare(position - ROW + 2 * COLUMN) == BLACKKNIGHT ||
               board.getSquare(position + ROW - 2 * COLUMN) == BLACKKNIGHT ||
               board.getSquare(position + ROW + 2 * COLUMN) == BLACKKNIGHT ||
               board.getSquare(position - 2 * ROW - COLUMN) == BLACKKNIGHT ||
               board.getSquare(position - 2 * ROW + COLUMN) == BLACKKNIGHT ||
               board.getSquare(position + 2 * ROW - COLUMN) == BLACKKNIGHT ||
               board.getSquare(position + 2 * ROW + COLUMN) == BLACKKNIGHT) {
               return true;
          }
          //  3. bishop
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position - i*ROW - i*COLUMN) == BLACKBISHOP ||
                    board.getSquare(position - i*ROW - i*COLUMN) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position - i*ROW - i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position - i*ROW - i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position - i*ROW + i*COLUMN) == BLACKBISHOP ||
                    board.getSquare(position - i*ROW + i*COLUMN) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position - i*ROW + i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position - i*ROW + i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position + i*ROW - i*COLUMN) == BLACKBISHOP ||
                    board.getSquare(position + i*ROW - i*COLUMN) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position + i*ROW - i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position + i*ROW - i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position + i*ROW + i*COLUMN) == BLACKBISHOP ||
                    board.getSquare(position + i*ROW + i*COLUMN) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position + i*ROW + i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position + i*ROW + i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          //  4. rook
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position - i*ROW) == BLACKROOK ||
                    board.getSquare(position - i*ROW) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position - i*ROW) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position - i*ROW) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position + i*ROW) == BLACKROOK ||
                    board.getSquare(position + i*ROW) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position + i*ROW) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position + i*ROW) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position - i*COLUMN) == BLACKROOK ||
                    board.getSquare(position - i*COLUMN) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position - i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position - i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position + i*COLUMN) == BLACKROOK ||
                    board.getSquare(position + i*COLUMN) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position + i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position + i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          //  (5. queen: added to bishop and rook)

          //  6. king: is it needed?
          if (board.getSquare(position + 1) == BLACKKING ||
               board.getSquare(position - 1) == BLACKKING ||
               board.getSquare(position + 11) == BLACKKING ||
               board.getSquare(position - 11) == BLACKKING ||
               board.getSquare(position + 9) == BLACKKING ||
               board.getSquare(position - 9) == BLACKKING ||
               board.getSquare(position + 10) == BLACKKING ||
               board.getSquare(position - 10) == BLACKKING) {
               return true;
          }

          return false;
     }

     if (board.getTurn() == BLACK) {
          //  1. pawn
          if (board.getSquare(position + ROW - COLUMN) == WHITEPAWN ||
               board.getSquare(position + ROW + COLUMN) == WHITEPAWN) {
               return true;
          }
          //  2. knight
          if (board.getSquare(position - ROW - 2 * COLUMN) == WHITEKNIGHT ||
               board.getSquare(position - ROW + 2 * COLUMN) == WHITEKNIGHT ||
               board.getSquare(position + ROW - 2 * COLUMN) == WHITEKNIGHT ||
               board.getSquare(position + ROW + 2 * COLUMN) == WHITEKNIGHT ||
               board.getSquare(position - 2 * ROW - COLUMN) == WHITEKNIGHT ||
               board.getSquare(position - 2 * ROW + COLUMN) == WHITEKNIGHT ||
               board.getSquare(position + 2 * ROW - COLUMN) == WHITEKNIGHT ||
               board.getSquare(position + 2 * ROW + COLUMN) == WHITEKNIGHT) {
               return true;
          }
          //  3. bishop
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position - i*ROW - i*COLUMN) == WHITEBISHOP ||
                    board.getSquare(position - i*ROW - i*COLUMN) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position - i*ROW - i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position - i*ROW - i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position - i*ROW + i*COLUMN) == WHITEBISHOP ||
                    board.getSquare(position - i*ROW + i*COLUMN) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position - i*ROW + i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position - i*ROW + i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position + i*ROW - i*COLUMN) == WHITEBISHOP ||
                    board.getSquare(position + i*ROW - i*COLUMN) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position + i*ROW - i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position + i*ROW - i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position + i*ROW + i*COLUMN) == WHITEBISHOP ||
                    board.getSquare(position + i*ROW + i*COLUMN) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position + i*ROW + i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position + i*ROW + i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          //  4. rook
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position - i*ROW) == WHITEROOK ||
                    board.getSquare(position - i*ROW) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position - i*ROW) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position - i*ROW) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position + i*ROW) == WHITEROOK ||
                    board.getSquare(position + i*ROW) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position + i*ROW) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position + i*ROW) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position - i*COLUMN) == WHITEROOK ||
                    board.getSquare(position - i*COLUMN) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position - i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position - i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(position + i*COLUMN) == WHITEROOK ||
                    board.getSquare(position + i*COLUMN) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(position + i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(position + i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }

          //  (5. queen: added to bishop and rook)

          //  6. king: is it needed?
          if (board.getSquare(position + 1) == WHITEKING ||
               board.getSquare(position - 1) == WHITEKING ||
               board.getSquare(position + 11) == WHITEKING ||
               board.getSquare(position - 11) == WHITEKING ||
               board.getSquare(position + 9) == WHITEKING ||
               board.getSquare(position - 9) == WHITEKING ||
               board.getSquare(position + 10) == WHITEKING ||
               board.getSquare(position - 10) == WHITEKING) {
               return true;
          }

          return false;
     }

     printf("squareAttackCheck unreachable error\n");
     return true;

}
