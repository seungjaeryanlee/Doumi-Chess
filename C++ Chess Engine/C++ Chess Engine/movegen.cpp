#include "defs.h"
#include "protos.h"
#include "movegen.h"

void moveGeneration(const Board& board, MoveList moveList) {
     MoveList pseudolegalMoveList;
     // STEP 1: PSEUDOLEGAL MOVEGEN
     castlingMoveGeneration(board, pseudolegalMoveList);
     enpassantMoveGeneration(board, pseudolegalMoveList);

     if (board.getTurn() == WHITE) {
          for (int i = 0; i < 120; i++) {
               switch (board.getSquare(i)) {
               case WHITEPAWN:
                    pawnMoveGeneration(board, i, pseudolegalMoveList);
                    break;
               case WHITEKNIGHT:
                    knightMoveGeneration(board, i, pseudolegalMoveList);
                    break;
               case WHITEBISHOP:
                    bishopMoveGeneration(board, i, pseudolegalMoveList);
                    break;
               case WHITEROOK:
                    rookMoveGeneration(board, i, pseudolegalMoveList);
                    break;
               case WHITEQUEEN:
                    queenMoveGeneration(board, i, pseudolegalMoveList);
                    break;
               case WHITEKING:
                    kingMoveGeneration(board, i, pseudolegalMoveList);
                    break;
               }
          }
     }
     if (board.getTurn() == BLACK) {
          for (int i = 0; i < 120; i++) {
               switch (board.getSquare(i)) {
               case BLACKPAWN:
                    pawnMoveGeneration(board, i, pseudolegalMoveList);
                    break;
               case BLACKKNIGHT:
                    knightMoveGeneration(board, i, pseudolegalMoveList);
                    break;
               case BLACKBISHOP:
                    bishopMoveGeneration(board, i, pseudolegalMoveList);
                    break;
               case BLACKROOK:
                    rookMoveGeneration(board, i, pseudolegalMoveList);
                    break;
               case BLACKQUEEN:
                    queenMoveGeneration(board, i, pseudolegalMoveList);
                    break;
               case BLACKKING:
                    kingMoveGeneration(board, i, pseudolegalMoveList);
                    break;
               }
          }
     }

     // STEP 2: CHECK LEGALITY
     moveList.setCounterToZero();
     Board copiedBoard(board); // Clone

     //  find king position
     int kingPosition = 0, changedKingPosition = 0;
     int terminalValue;
     for (int i = 0; i < 120; i++) {
          if (copiedBoard.getTurn() == WHITE && copiedBoard.getSquare(i) == WHITEKING ||
               copiedBoard.getTurn() == BLACK && copiedBoard.getSquare(i) == BLACKKING) {
               kingPosition = i;
               break;
          }
     }

     for (int i = 0; i < pseudolegalMoveList.getCounter(); i++) {
          //  check if king will be moved
          if (copiedBoard.getSquare(pseudolegalMoveList.getMove(i).getInitial()) == WHITEKING || copiedBoard.getSquare(pseudolegalMoveList.getMove(i).getInitial()) == BLACKKING) {
               if (pseudolegalMoveList.getMove(i).getType() == NORMAL) {
                    changedKingPosition = pseudolegalMoveList.getMove(i).getTerminal();
               }
               if (pseudolegalMoveList.getMove(i).getType() == KINGSIDE_CASTLING) {
                    changedKingPosition = pseudolegalMoveList.getMove(i).getInitial() + 2 * COLUMN;
               }
               if (pseudolegalMoveList.getMove(i).getType() == QUEENSIDE_CASTLING) {
                    changedKingPosition = pseudolegalMoveList.getMove(i).getInitial() - 2 * COLUMN;
               }

          }
          else { changedKingPosition = kingPosition; }

          //  make move
          terminalValue = makeMove(copiedBoard, pseudolegalMoveList.getMove(i));
          //  In this case, we don't want makeMove to change turn, so let's change it again
          copiedBoard.changeTurn();

          //  if king is safe
          if (!squareAttackCheck(copiedBoard, changedKingPosition)) {
               moveList.addMove(pseudolegalMoveList.getMove(i));
          }

          //  undo move
          undoMove(copiedBoard, pseudolegalMoveList.getMove(i), terminalValue);
          //  Same reason as above
          copiedBoard.changeTurn();
     }

}
void pawnMoveGeneration(const Board& board, const int position, MoveList moveList) {
     if (board.getTurn() == WHITE) {
          //  if on the last row before promotion, just call promotion
          if (A7 <= position && position <= H7) {
               promotionMoveGeneration(board, position, moveList);
               return;
          }

          //  Advance 1 square
          if (board.getSquare(position - ROW) == EMPTYSQUARE) {
               addMove(position, position - ROW, NORMAL, moveList);
               //  Advance 2 squares
               if (A2 <= position && position <= H2 &&
                    board.getSquare(position - 2 * ROW) == EMPTYSQUARE) {
                    addMove(position, position - 2 * ROW, DOUBLEMOVE, moveList);
               }
          }

          //  attack diagonals
          if (checkColor(board.getSquare(position - ROW - COLUMN)) == BLACK) {
               addMove(position, position - ROW - COLUMN, NORMAL, moveList);
          }
          if (checkColor(board.getSquare(position - ROW + COLUMN)) == BLACK) {
               addMove(position, position - ROW + COLUMN, NORMAL, moveList);
          }
     }
     if (board.getTurn() == BLACK) {
          //  if on the last row before promotion, just call promotion
          if (A2 <= position && position <= H2) {
               promotionMoveGeneration(board, position, moveList);
               return;
          }

          //  Advance 1 square
          if (board.getSquare(position + ROW) == EMPTYSQUARE) {
               addMove(position, position + ROW, NORMAL, moveList);
               //  Advance 2 squares
               if (A7 <= position && position <= H7 &&
                    board.getSquare(position + 2 * ROW) == EMPTYSQUARE) {
                    addMove(position, position + 2 * ROW, DOUBLEMOVE, moveList);
               }
          }

          //  attack diagonals
          if (checkColor(board.getSquare(position + ROW - COLUMN)) == WHITE) {
               addMove(position, position + ROW - COLUMN, NORMAL, moveList);
          }
          if (checkColor(board.getSquare(position + ROW + COLUMN)) == WHITE) {
               addMove(position, position + ROW + COLUMN, NORMAL, moveList);
          }
     }
}
void knightMoveGeneration(const Board& board, const int position, MoveList moveList) {
     int turn = board.getTurn();

     if (checkColor(board.getSquare(position + ROW + 2 * COLUMN)) == -turn ||
          board.getSquare(position + ROW + 2 * COLUMN) == EMPTYSQUARE) {
          addMove(position, position + ROW + 2 * COLUMN, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position + ROW - 2 * COLUMN)) == -turn ||
          board.getSquare(position + ROW - 2 * COLUMN) == EMPTYSQUARE) {
          addMove(position, position + ROW - 2 * COLUMN, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position - ROW + 2 * COLUMN)) == -turn ||
          board.getSquare(position - ROW + 2 * COLUMN) == EMPTYSQUARE) {
          addMove(position, position - ROW + 2 * COLUMN, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position - ROW - 2 * COLUMN)) == -turn ||
          board.getSquare(position - ROW - 2 * COLUMN) == EMPTYSQUARE) {
          addMove(position, position - ROW - 2 * COLUMN, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position + 2 * ROW + COLUMN)) == -turn ||
          board.getSquare(position + 2 * ROW + COLUMN) == EMPTYSQUARE) {
          addMove(position, position + 2 * ROW + COLUMN, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position + 2 * ROW - COLUMN)) == -turn ||
          board.getSquare(position + 2 * ROW - COLUMN) == EMPTYSQUARE) {
          addMove(position, position + 2 * ROW - COLUMN, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position - 2 * ROW + COLUMN)) == -turn ||
          board.getSquare(position - 2 * ROW + COLUMN) == EMPTYSQUARE) {
          addMove(position, position - 2 * ROW + COLUMN, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position - 2 * ROW - COLUMN)) == -turn ||
          board.getSquare(position - 2 * ROW - COLUMN) == EMPTYSQUARE) {
          addMove(position, position - 2 * ROW - COLUMN, NORMAL, moveList);
     }
}
void bishopMoveGeneration(const Board& board, const int position, MoveList moveList) {
     int turn = board.getTurn();
     bool topright = true, downright = true, downleft = true, topleft = true;
     for (int i = 1; i < 8; i++) {
          if (topright == true &&
               (checkColor(board.getSquare(position - i*ROW + i*COLUMN)) == -turn ||
                    board.getSquare(position - i*ROW + i*COLUMN) == EMPTYSQUARE)) {
               addMove(position, position - i*ROW + i*COLUMN, NORMAL, moveList);
               if (checkColor(board.getSquare(position - i*ROW + i*COLUMN)) == -turn) {
                    topright = false;
               }
          }
          else { topright = false; }

          if (downright == true &&
               (checkColor(board.getSquare(position + i*ROW + i*COLUMN)) == -turn ||
                    board.getSquare(position + i*ROW + i*COLUMN) == EMPTYSQUARE)) {
               addMove(position, position + i*ROW + i*COLUMN, NORMAL, moveList);
               if (checkColor(board.getSquare(position + i*ROW + i*COLUMN)) == -turn) {
                    downright = false;
               }
          }
          else { downright = false; }

          if (downleft == true &&
               (checkColor(board.getSquare(position + i*ROW - i*COLUMN)) == -turn ||
                    board.getSquare(position + i*ROW - i*COLUMN) == EMPTYSQUARE)) {
               addMove(position, position + i*ROW - i*COLUMN, NORMAL, moveList);
               if (checkColor(board.getSquare(position + i*ROW - i*COLUMN)) == -turn) {
                    downleft = false;
               }
          }
          else { downleft = false; }

          if (topleft == true &&
               (checkColor(board.getSquare(position - i*ROW - i*COLUMN)) == -turn ||
                    board.getSquare(position - i*ROW - i*COLUMN) == EMPTYSQUARE)) {
               addMove(position, position - i*ROW - i*COLUMN, NORMAL, moveList);
               if (checkColor(board.getSquare(position - i*ROW - i*COLUMN)) == -turn) {
                    topleft = false;
               }
          }
          else { topleft = false; }
     }
}
void rookMoveGeneration(const Board& board, const int position, MoveList moveList) {
     int turn = board.getTurn();
     bool top = true, right = true, down = true, left = true;

     for (int i = 1; i < 8; i++) {
          if (top == true &&
               (checkColor(board.getSquare(position - i*ROW)) == -turn ||
                    board.getSquare(position - i*ROW) == EMPTYSQUARE)) {
               addMove(position, position - i*ROW, NORMAL, moveList);
               if (checkColor(board.getSquare(position - i*ROW)) == -turn) {
                    top = false;
               }
          }
          else { top = false; }

          if (right == true &&
               (checkColor(board.getSquare(position + i*COLUMN)) == -turn ||
                    board.getSquare(position + i*COLUMN) == EMPTYSQUARE)) {
               addMove(position, position + i*COLUMN, NORMAL, moveList);
               if (checkColor(board.getSquare(position + i*COLUMN)) == -turn) {
                    right = false;
               }
          }
          else { right = false; }

          if (down == true &&
               (checkColor(board.getSquare(position + i*ROW)) == -turn ||
                    board.getSquare(position + i*ROW) == EMPTYSQUARE)) {
               addMove(position, position + i*ROW, NORMAL, moveList);
               if (checkColor(board.getSquare(position + i*ROW)) == -turn) {
                    down = false;
               }
          }
          else { down = false; }

          if (left == true &&
               (checkColor(board.getSquare(position - i*COLUMN)) == -turn ||
                    board.getSquare(position - i*COLUMN) == EMPTYSQUARE)) {
               addMove(position, position - i*COLUMN, NORMAL, moveList);
               if (checkColor(board.getSquare(position - i*COLUMN)) == -turn) {
                    left = false;
               }
          }
          else { left = false; }
     }
}
void queenMoveGeneration(const Board& board, const int position, MoveList moveList) {
     rookMoveGeneration(board, position, moveList);
     bishopMoveGeneration(board, position, moveList);
}
void kingMoveGeneration(const Board& board, const int position, MoveList moveList) {
     int turn = board.getTurn();

     if (checkColor(board.getSquare(position + ROW)) == -turn ||
          board.getSquare(position + ROW) == EMPTYSQUARE) {
          addMove(position, position + ROW, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position - ROW)) == -turn ||
          board.getSquare(position - ROW) == EMPTYSQUARE) {
          addMove(position, position - ROW, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position + COLUMN)) == -turn ||
          board.getSquare(position + COLUMN) == EMPTYSQUARE) {
          addMove(position, position + COLUMN, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position - COLUMN)) == -turn ||
          board.getSquare(position - COLUMN) == EMPTYSQUARE) {
          addMove(position, position - COLUMN, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position + ROW + COLUMN)) == -turn ||
          board.getSquare(position + ROW + COLUMN) == EMPTYSQUARE) {
          addMove(position, position + ROW + COLUMN, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position + ROW - COLUMN)) == -turn ||
          board.getSquare(position + ROW - COLUMN) == EMPTYSQUARE) {
          addMove(position, position + ROW - COLUMN, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position - ROW + COLUMN)) == -turn ||
          board.getSquare(position - ROW + COLUMN) == EMPTYSQUARE) {
          addMove(position, position - ROW + COLUMN, NORMAL, moveList);
     }
     if (checkColor(board.getSquare(position - ROW - COLUMN)) == -turn ||
          board.getSquare(position - ROW - COLUMN) == EMPTYSQUARE) {
          addMove(position, position - ROW - COLUMN, NORMAL, moveList);
     }
}

void castlingMoveGeneration(const Board& board, MoveList moveList) {
     if (board.getTurn() == WHITE) {
          if (board.getCastling(WKCASTLING) &&                                             //  neither piece moved
               board.getSquare(E1) == WHITEKING && board.getSquare(H1) == WHITEROOK &&     //  both pieces exists on board
               board.getSquare(F1) == EMPTYSQUARE && board.getSquare(G1) == EMPTYSQUARE && //  between them are empty
               squareAttackCheck(board, E1) == false &&                                    //  not in check
               squareAttackCheck(board, F1) == false &&                                    //  not attacked while moving
               squareAttackCheck(board, G1) == false) {

               addMove(E1, G1, KINGSIDE_CASTLING, moveList);
          }
          if (board.getCastling(WQCASTLING) && board.getSquare(B1) == EMPTYSQUARE &&
               board.getSquare(C1) == EMPTYSQUARE && board.getSquare(D1) == EMPTYSQUARE &&
               board.getSquare(E1) == WHITEKING && board.getSquare(A1) == WHITEROOK &&
               squareAttackCheck(board, E1) == false &&
               squareAttackCheck(board, C1) == false &&
               squareAttackCheck(board, D1) == false) {
               addMove(E1, C1, QUEENSIDE_CASTLING, moveList);
          }

     }
     if (board.getTurn() == BLACK) {
          if (board.getCastling(BKCASTLING) &&                                             //  neither piece moved
               board.getSquare(E8) == BLACKKING && board.getSquare(H8) == BLACKROOK &&     //  both pieces exists on board
               board.getSquare(F8) == EMPTYSQUARE && board.getSquare(G8) == EMPTYSQUARE && //  between them are empty
               squareAttackCheck(board, E8) == false &&                                    //  not in check
               squareAttackCheck(board, F8) == false &&                                    //  not attacked while moving
               squareAttackCheck(board, G8) == false) {

               addMove(E8, G8, KINGSIDE_CASTLING, moveList);
          }
          if (board.getCastling(BQCASTLING) && board.getSquare(B8) == EMPTYSQUARE &&
               board.getSquare(C8) == EMPTYSQUARE && board.getSquare(D8) == EMPTYSQUARE &&
               board.getSquare(E8) == BLACKKING && board.getSquare(A8) == BLACKROOK &&
               squareAttackCheck(board, E8) == false &&
               squareAttackCheck(board, C8) == false &&
               squareAttackCheck(board, D8) == false) {
               addMove(E8, C8, QUEENSIDE_CASTLING, moveList);
          }
     }
}
void promotionMoveGeneration(const Board& board, const int position, MoveList moveList) {
     if (board.getTurn() == WHITE) {
          if (checkColor(board.getSquare(position - ROW - COLUMN)) == -board.getTurn()) {
               addPromotionMove(position, position - ROW - COLUMN, moveList);
          }
          if (checkColor(board.getSquare(position - ROW + COLUMN)) == -board.getTurn()) {
               addPromotionMove(position, position - ROW + COLUMN, moveList);
          }
          if (board.getSquare(position - ROW) == EMPTYSQUARE) {
               addPromotionMove(position, position - ROW, moveList);
          }
     }
     if (board.getTurn() == BLACK) {
          if (checkColor(board.getSquare(position + ROW - COLUMN)) == -board.getTurn()) {
               addPromotionMove(position, position + ROW - COLUMN, moveList);
          }
          if (checkColor(board.getSquare(position + ROW + COLUMN)) == -board.getTurn()) {
               addPromotionMove(position, position + ROW + COLUMN, moveList);
          }
          if (board.getSquare(position + ROW) == EMPTYSQUARE) {
               addPromotionMove(position, position + ROW, moveList);
          }
     }
}
void enpassantMoveGeneration(const Board& board, MoveList moveList) {
     if (board.getEnpassantSquare() == 0) { return; }

     int enpassantSquare = board.getEnpassantSquare();

     if (board.getTurn() == WHITE) {
          if (board.getSquare(enpassantSquare + ROW + COLUMN) == WHITEPAWN) {
               addMove(enpassantSquare + ROW + COLUMN, enpassantSquare, ENPASSANT, moveList);
          }
          if (board.getSquare(enpassantSquare + ROW - COLUMN) == WHITEPAWN) {
               addMove(enpassantSquare + ROW - COLUMN, enpassantSquare, ENPASSANT, moveList);
          }
     }
     if (board.getTurn() == BLACK) {
          if (board.getSquare(enpassantSquare - ROW + COLUMN) == BLACKPAWN) {
               addMove(enpassantSquare - ROW + COLUMN, enpassantSquare, ENPASSANT, moveList);
          }
          if (board.getSquare(enpassantSquare - ROW - COLUMN) == BLACKPAWN) {
               addMove(enpassantSquare - ROW - COLUMN, enpassantSquare, ENPASSANT, moveList);
          }
     }
}

void addMove(int initial, int terminal, int moveType, MoveList moveList) {
     moveList.addMove(Move(initial, terminal, moveType));
}
void addPromotionMove(int initial, int terminal, MoveList moveList) {
     addMove(initial, terminal, KNIGHT_PROMOTION, moveList);
     addMove(initial, terminal, BISHOP_PROMOTION, moveList);
     addMove(initial, terminal, ROOK_PROMOTION, moveList);
     addMove(initial, terminal, QUEEN_PROMOTION, moveList);
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

     else if (board.getTurn() == BLACK) {
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

     else {
          // TODO: Better Exception Handling
          throw "Board given as argument to squareAttackCheck does not have valid turn.\n";
          return true;
     }
     
}
