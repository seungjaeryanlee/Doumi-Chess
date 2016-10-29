#include "board.h"
#include "movegen.h"


int moveGenCounter = 0;
void moveGenCalled() {
     moveGenCounter++;
}

MoveList moveGeneration(const Board& board) {
     moveGenCalled();
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
     MoveList moveList;
     Board copiedBoard = board; // QUESTION: Make argument not-const and skip this?

     //  find king position
     int kingPosition = 0, changedKingPosition = 0;
     int turn = board.getTurn();
     int capturedPiece;
     for (int i = 0; i < 120; i++) {
          if (copiedBoard.getTurn() == WHITE && copiedBoard.getSquare(i) == WHITEKING ||
               copiedBoard.getTurn() == BLACK && copiedBoard.getSquare(i) == BLACKKING) {
               kingPosition = i;
               break;
          }
     }

     /*
     // Find pinned pieces
     bool maybePinned;            // There is a ally piece
     bool pinned;                 // There is a rook/bishop/queen behind the ally piece
     int pinnedPiece[8] = { 0, }; // The pinned piece
     
     // UPLEFT
     maybePinned = false; pinned = false;
     if (turn == WHITE) {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[UPLEFT])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[UPLEFT] = kingPosition + i*GO[UPLEFT];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[UPLEFT]) == WHITEBISHOP)
                    || (board.getSquare(kingPosition + i*GO[UPLEFT]) == WHITEQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }
     else {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[UPLEFT])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[UPLEFT] = kingPosition + i*GO[UPLEFT];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[UPLEFT]) == BLACKBISHOP)
                    || (board.getSquare(kingPosition + i*GO[UPLEFT]) == BLACKQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }
     
     // UPRIGHT
     maybePinned = false; pinned = false;
     if (turn == WHITE) {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[UPRIGHT])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[UPRIGHT] = kingPosition + i*GO[UPRIGHT];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[UPRIGHT]) == WHITEBISHOP)
                    || (board.getSquare(kingPosition + i*GO[UPRIGHT]) == WHITEQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }
     else {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[UPRIGHT])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[UPRIGHT] = kingPosition + i*GO[UPRIGHT];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[UPRIGHT]) == BLACKBISHOP)
                    || (board.getSquare(kingPosition + i*GO[UPRIGHT]) == BLACKQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }

     // DOWNLEFT
     maybePinned = false; pinned = false;
     if (turn == WHITE) {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[DOWNLEFT])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[DOWNLEFT] = kingPosition + i*GO[DOWNLEFT];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[DOWNLEFT]) == WHITEBISHOP)
                    || (board.getSquare(kingPosition + i*GO[DOWNLEFT]) == WHITEQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }
     else {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[DOWNLEFT])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[DOWNLEFT] = kingPosition + i*GO[DOWNLEFT];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[DOWNLEFT]) == BLACKBISHOP)
                    || (board.getSquare(kingPosition + i*GO[DOWNLEFT]) == BLACKQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }

     // DOWNRIGHT
     maybePinned = false; pinned = false;
     if (turn == WHITE) {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[DOWNRIGHT])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[DOWNRIGHT] = kingPosition + i*GO[DOWNRIGHT];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[DOWNRIGHT]) == WHITEBISHOP)
                    || (board.getSquare(kingPosition + i*GO[DOWNRIGHT]) == WHITEQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }
     else {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[DOWNRIGHT])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[DOWNRIGHT] = kingPosition + i*GO[DOWNRIGHT];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[DOWNRIGHT]) == BLACKBISHOP)
                    || (board.getSquare(kingPosition + i*GO[DOWNRIGHT]) == BLACKQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }

     // UP
     maybePinned = false; pinned = false;
     if (turn == WHITE) {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[UP])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[UP] = kingPosition + i*GO[UP];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[UP]) == WHITEROOK)
                    || (board.getSquare(kingPosition + i*GO[UP]) == WHITEQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }
     else {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[UP])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[UP] = kingPosition + i*GO[UP];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[UP]) == BLACKROOK)
                    || (board.getSquare(kingPosition + i*GO[UP]) == BLACKQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }

     // DOWN
     maybePinned = false; pinned = false;
     if (turn == WHITE) {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[DOWN])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[DOWN] = kingPosition + i*GO[DOWN];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[DOWN]) == WHITEROOK)
                    || (board.getSquare(kingPosition + i*GO[DOWN]) == WHITEQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }
     else {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[DOWN])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[DOWN] = kingPosition + i*GO[DOWN];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[DOWN]) == BLACKROOK)
                    || (board.getSquare(kingPosition + i*GO[DOWN]) == BLACKQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }

     // LEFT
     maybePinned = false; pinned = false;
     if (turn == WHITE) {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[LEFT])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[LEFT] = kingPosition + i*GO[LEFT];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[LEFT]) == WHITEROOK)
                    || (board.getSquare(kingPosition + i*GO[LEFT]) == WHITEQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }
     else {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[LEFT])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[LEFT] = kingPosition + i*GO[LEFT];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[LEFT]) == BLACKROOK)
                    || (board.getSquare(kingPosition + i*GO[LEFT]) == BLACKQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }

     // RIGHT
     maybePinned = false; pinned = false;
     if (turn == WHITE) {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[RIGHT])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[RIGHT] = kingPosition + i*GO[RIGHT];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[RIGHT]) == WHITEROOK)
                    || (board.getSquare(kingPosition + i*GO[RIGHT]) == WHITEQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }
     else {
          for (int i = 1; i < 8; i++) {
               if (checkColor(board.getSquare(kingPosition + i*GO[RIGHT])) == turn) {
                    if (maybePinned) { break; }
                    else {
                         maybePinned = true;
                         pinnedPiece[RIGHT] = kingPosition + i*GO[RIGHT];
                    }

               }
               if (maybePinned && (board.getSquare(kingPosition + i*GO[RIGHT]) == BLACKROOK)
                    || (board.getSquare(kingPosition + i*GO[RIGHT]) == BLACKQUEEN)) {
                    pinned = true;
                    break;
               }
          }
     }
     */

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
          capturedPiece = makeMove(copiedBoard, pseudolegalMoveList.getMove(i));
          //  In this case, we don't want makeMove to change turn, so let's change it again
          copiedBoard.changeTurn();

          //  if king is safe
          if (!isSquareAttacked(copiedBoard, changedKingPosition)) {
               moveList.addMove(pseudolegalMoveList.getMove(i));
          }

          //  undo move
          undoMove(copiedBoard, pseudolegalMoveList.getMove(i), capturedPiece);
          //  Same reason as above
          copiedBoard.changeTurn();
     }
     
     return moveList;
}
void pawnMoveGeneration(const Board& board, const int position, MoveList& moveList) {
     if (board.getTurn() == WHITE) {
          //  if on the last row before promotion, just call promotion
          if (A7 <= position && position <= H7) {
               promotionMoveGeneration(board, position, moveList);
               return;
          }

          //  Advance 1 square
          if (board.getSquare(position + GO[UP]) == EMPTYSQUARE) {
               addMove(position, position + GO[UP], NORMAL, moveList);
               //  Advance 2 squares
               if (A2 <= position && position <= H2 &&
                    board.getSquare(position + 2 * GO[UP]) == EMPTYSQUARE) {
                    addMove(position, position + 2 * GO[UP], DOUBLEMOVE, moveList);
               }
          }

          //  attack diagonals
          if (checkColor(board.getSquare(position + GO[UPLEFT])) == BLACK) {
               addMove(position, position + GO[UPLEFT], NORMAL, moveList);
          }
          if (checkColor(board.getSquare(position + GO[UPRIGHT])) == BLACK) {
               addMove(position, position + GO[UPRIGHT], NORMAL, moveList);
          }
     }
     if (board.getTurn() == BLACK) {
          //  if on the last row before promotion, just call promotion
          if (A2 <= position && position <= H2) {
               promotionMoveGeneration(board, position, moveList);
               return;
          }

          //  Advance 1 square
          if (board.getSquare(position + GO[DOWN]) == EMPTYSQUARE) {
               addMove(position, position + GO[DOWN], NORMAL, moveList);
               //  Advance 2 squares
               if (A7 <= position && position <= H7 &&
                    board.getSquare(position + 2 * GO[DOWN]) == EMPTYSQUARE) {
                    addMove(position, position + 2 * GO[DOWN], DOUBLEMOVE, moveList);
               }
          }

          //  attack diagonals
          if (checkColor(board.getSquare(position + GO[DOWNLEFT])) == WHITE) {
               addMove(position, position + GO[DOWNLEFT], NORMAL, moveList);
          }
          if (checkColor(board.getSquare(position + GO[DOWNRIGHT])) == WHITE) {
               addMove(position, position + GO[DOWNRIGHT], NORMAL, moveList);
          }
     }
}
void knightMoveGeneration(const Board& board, const int position, MoveList& moveList) {
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
void bishopMoveGeneration(const Board& board, const int position, MoveList& moveList) {
     int turn = board.getTurn();

     // TOPRIGHT
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[UPRIGHT]) == EMPTYSQUARE) {
               addMove(position, position + i*GO[UPRIGHT], NORMAL, moveList);
          }
          else if (checkColor(board.getSquare(position + i*GO[UPRIGHT])) == -turn) {
               addMove(position, position + i*GO[UPRIGHT], NORMAL, moveList);
               break;
          }
          else { break; }
     }

     // DOWNRIGHT
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[DOWNRIGHT]) == EMPTYSQUARE) {
               addMove(position, position + i*GO[DOWNRIGHT], NORMAL, moveList);
          }
          else if (checkColor(board.getSquare(position + i*GO[DOWNRIGHT])) == -turn) {
               addMove(position, position + i*GO[DOWNRIGHT], NORMAL, moveList);
               break;
          }
          else { break; }
     }

     // DOWNLEFT
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[DOWNLEFT]) == EMPTYSQUARE) {
               addMove(position, position + i*GO[DOWNLEFT], NORMAL, moveList);
          }
          else if (checkColor(board.getSquare(position + i*GO[DOWNLEFT])) == -turn) {
               addMove(position, position + i*GO[DOWNLEFT], NORMAL, moveList);
               break;
          }
          else { break; }
     }

     // TOPLEFT
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[UPLEFT]) == EMPTYSQUARE) {
               addMove(position, position + i*GO[UPLEFT], NORMAL, moveList);
          }
          else if (checkColor(board.getSquare(position + i*GO[UPLEFT])) == -turn) {
               addMove(position, position + i*GO[UPLEFT], NORMAL, moveList);
               break;
          }
          else { break; }
     }     
}
void rookMoveGeneration(const Board& board, const int position, MoveList& moveList) {
     int turn = board.getTurn();

     // UP
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[UP]) == EMPTYSQUARE) {
               addMove(position, position + i*GO[UP], NORMAL, moveList);
          }
          else if (checkColor(board.getSquare(position + i*GO[UP])) == -turn) {
               addMove(position, position + i*GO[UP], NORMAL, moveList);
               break;
          }
          else { break; }
     }

     // DOWN
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[DOWN]) == EMPTYSQUARE) {
               addMove(position, position + i*GO[DOWN], NORMAL, moveList);
          }
          else if (checkColor(board.getSquare(position + i*GO[DOWN])) == -turn) {
               addMove(position, position + i*GO[DOWN], NORMAL, moveList);
               break;
          }
          else { break; }
     }

     // LEFT
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[LEFT]) == EMPTYSQUARE) {
               addMove(position, position + i*GO[LEFT], NORMAL, moveList);
          }
          else if (checkColor(board.getSquare(position + i*GO[LEFT])) == -turn) {
               addMove(position, position + i*GO[LEFT], NORMAL, moveList);
               break;
          }
          else { break; }
     }

     // RIGHT
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[RIGHT]) == EMPTYSQUARE) {
               addMove(position, position + i*GO[RIGHT], NORMAL, moveList);
          }
          else if (checkColor(board.getSquare(position + i*GO[RIGHT])) == -turn) {
               addMove(position, position + i*GO[RIGHT], NORMAL, moveList);
               break;
          }
          else { break; }
     }
}
void queenMoveGeneration(const Board& board, const int position, MoveList& moveList) {
     rookMoveGeneration(board, position, moveList);
     bishopMoveGeneration(board, position, moveList);
}
void kingMoveGeneration(const Board& board, const int position, MoveList& moveList) {
     for (int i = 0; i < 8; i++) {
          if (checkColor(board.getSquare(position + GO[i])) == -board.getTurn() ||
               board.getSquare(position + GO[i]) == EMPTYSQUARE) {
               addMove(position, position + GO[i], NORMAL, moveList);
          }
     }
}

void castlingMoveGeneration(const Board& board, MoveList& moveList) {
     if (board.getTurn() == WHITE) {
          if (board.getCastlingRight(WKCASTLING) &&                                             //  neither piece moved
               board.getSquare(E1) == WHITEKING && board.getSquare(H1) == WHITEROOK &&     //  both pieces exists on board
               board.getSquare(F1) == EMPTYSQUARE && board.getSquare(G1) == EMPTYSQUARE && //  between them are empty
               isSquareAttacked(board, E1) == false &&                                    //  not in check
               isSquareAttacked(board, F1) == false &&                                    //  not attacked while moving
               isSquareAttacked(board, G1) == false) {

               addMove(E1, G1, KINGSIDE_CASTLING, moveList);
          }
          if (board.getCastlingRight(WQCASTLING) && board.getSquare(B1) == EMPTYSQUARE &&
               board.getSquare(C1) == EMPTYSQUARE && board.getSquare(D1) == EMPTYSQUARE &&
               board.getSquare(E1) == WHITEKING && board.getSquare(A1) == WHITEROOK &&
               isSquareAttacked(board, E1) == false &&
               isSquareAttacked(board, C1) == false &&
               isSquareAttacked(board, D1) == false) {
               addMove(E1, C1, QUEENSIDE_CASTLING, moveList);
          }

     }
     if (board.getTurn() == BLACK) {
          if (board.getCastlingRight(BKCASTLING) &&                                             //  neither piece moved
               board.getSquare(E8) == BLACKKING && board.getSquare(H8) == BLACKROOK &&     //  both pieces exists on board
               board.getSquare(F8) == EMPTYSQUARE && board.getSquare(G8) == EMPTYSQUARE && //  between them are empty
               isSquareAttacked(board, E8) == false &&                                    //  not in check
               isSquareAttacked(board, F8) == false &&                                    //  not attacked while moving
               isSquareAttacked(board, G8) == false) {

               addMove(E8, G8, KINGSIDE_CASTLING, moveList);
          }
          if (board.getCastlingRight(BQCASTLING) && board.getSquare(B8) == EMPTYSQUARE &&
               board.getSquare(C8) == EMPTYSQUARE && board.getSquare(D8) == EMPTYSQUARE &&
               board.getSquare(E8) == BLACKKING && board.getSquare(A8) == BLACKROOK &&
               isSquareAttacked(board, E8) == false &&
               isSquareAttacked(board, C8) == false &&
               isSquareAttacked(board, D8) == false) {
               addMove(E8, C8, QUEENSIDE_CASTLING, moveList);
          }
     }
}
void promotionMoveGeneration(const Board& board, const int position, MoveList& moveList) {
     if (board.getTurn() == WHITE) {
          if (checkColor(board.getSquare(position + GO[UPLEFT])) == -board.getTurn()) {
               addPromotionMove(position, position + GO[UPLEFT], moveList);
          }
          if (checkColor(board.getSquare(position + GO[UPRIGHT])) == -board.getTurn()) {
               addPromotionMove(position, position + GO[UPRIGHT], moveList);
          }
          if (board.getSquare(position + GO[UP]) == EMPTYSQUARE) {
               addPromotionMove(position, position + GO[UP], moveList);
          }
     }
     if (board.getTurn() == BLACK) {
          if (checkColor(board.getSquare(position + GO[DOWNLEFT])) == -board.getTurn()) {
               addPromotionMove(position, position + GO[DOWNLEFT], moveList);
          }
          if (checkColor(board.getSquare(position + GO[DOWNRIGHT])) == -board.getTurn()) {
               addPromotionMove(position, position + GO[DOWNRIGHT], moveList);
          }
          if (board.getSquare(position + GO[DOWN]) == EMPTYSQUARE) {
               addPromotionMove(position, position + GO[DOWN], moveList);
          }
     }
}
void enpassantMoveGeneration(const Board& board, MoveList& moveList) {
     if (board.getEnpassantSquare() == 0) { return; }

     int enpassantSquare = board.getEnpassantSquare();

     if (board.getTurn() == WHITE) {
          if (board.getSquare(enpassantSquare + GO[DOWNRIGHT]) == WHITEPAWN) {
               addMove(enpassantSquare + GO[DOWNRIGHT], enpassantSquare, ENPASSANT, moveList);
          }
          if (board.getSquare(enpassantSquare + GO[DOWNLEFT]) == WHITEPAWN) {
               addMove(enpassantSquare + GO[DOWNLEFT], enpassantSquare, ENPASSANT, moveList);
          }
     }
     if (board.getTurn() == BLACK) {
          if (board.getSquare(enpassantSquare + GO[UPRIGHT]) == BLACKPAWN) {
               addMove(enpassantSquare + GO[UPRIGHT], enpassantSquare, ENPASSANT, moveList);
          }
          if (board.getSquare(enpassantSquare + GO[UPLEFT]) == BLACKPAWN) {
               addMove(enpassantSquare + GO[UPLEFT], enpassantSquare, ENPASSANT, moveList);
          }
     }
}

void addMove(int initial, int terminal, int moveType, MoveList& moveList) {
     moveList.addMove(Move(initial, terminal, moveType));
}
void addPromotionMove(int initial, int terminal, MoveList& moveList) {
     addMove(initial, terminal, KNIGHT_PROMOTION, moveList);
     addMove(initial, terminal, BISHOP_PROMOTION, moveList);
     addMove(initial, terminal, ROOK_PROMOTION, moveList);
     addMove(initial, terminal, QUEEN_PROMOTION, moveList);
}