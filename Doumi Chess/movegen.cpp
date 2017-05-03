/******************************************************************************/
/* movegen.cpp                                                                */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/
     
#include "board.h"
#include "movegen.h"

MoveList moveGeneration(const Board& board) {
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

     // find king position
     int kingPosition = 0, changedKingPosition = 0;
     int turn = board.getTurn();
     int capturedPiece;
     for (int i = 0; i < 120; i++) {
          if (board.getTurn() == WHITE && board.getSquare(i) == WHITEKING ||
               board.getTurn() == BLACK && board.getSquare(i) == BLACKKING) {
               kingPosition = i;
               break;
          }
     }

     // FIXME: Maybe find Pinned Pieces

     Board copiedBoard = board;
     for (int i = 0; i < pseudolegalMoveList.getCounter(); i++) {

          // check if king will be moved
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

          capturedPiece = copiedBoard.makeMove(pseudolegalMoveList.getMove(i));
          // In this case, we don't want makeMove to change turn, so let's change it again
          copiedBoard.changeTurn();

          // if king is safe
          if (!copiedBoard.isAttacked(changedKingPosition)) {
               moveList.push(pseudolegalMoveList.getMove(i));
          }
          copiedBoard = board;
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
               moveList.push(position, position + GO[UP], NORMAL);
               //  Advance 2 squares
               if (A2 <= position && position <= H2
                && board.getSquare(position + 2 * GO[UP]) == EMPTYSQUARE) {
                    moveList.push(position, position + 2 * GO[UP], DOUBLEMOVE);
               }
          }

          //  attack diagonals
          if (board.colorOf(position + GO[UPLEFT]) == BLACK) {
               moveList.push(position, position + GO[UPLEFT], NORMAL);
          }
          if (board.colorOf(position + GO[UPRIGHT]) == BLACK) {
               moveList.push(position, position + GO[UPRIGHT], NORMAL);
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
               moveList.push(position, position + GO[DOWN], NORMAL);
               //  Advance 2 squares
               if (A7 <= position && position <= H7 
                && board.getSquare(position + 2 * GO[DOWN]) == EMPTYSQUARE) {
                    moveList.push(position, position + 2 * GO[DOWN], DOUBLEMOVE);
               }
          }

          //  attack diagonals
          if (board.colorOf(position + GO[DOWNLEFT]) == WHITE) {
               moveList.push(position, position + GO[DOWNLEFT], NORMAL);
          }
          if (board.colorOf(position + GO[DOWNRIGHT]) == WHITE) {
               moveList.push(position, position + GO[DOWNRIGHT], NORMAL);
          }
     }
}
void knightMoveGeneration(const Board& board, const int position, MoveList& moveList) {
     int turn = board.getTurn();

     if (board.colorOf(position + ROW + 2 * COLUMN) == -turn ||
          board.getSquare(position + ROW + 2 * COLUMN) == EMPTYSQUARE) {
          moveList.push(position, position + ROW + 2 * COLUMN, NORMAL);
     }
     if (board.colorOf(position + ROW - 2 * COLUMN) == -turn ||
          board.getSquare(position + ROW - 2 * COLUMN) == EMPTYSQUARE) {
          moveList.push(position, position + ROW - 2 * COLUMN, NORMAL);
     }
     if (board.colorOf(position - ROW + 2 * COLUMN) == -turn ||
          board.getSquare(position - ROW + 2 * COLUMN) == EMPTYSQUARE) {
          moveList.push(position, position - ROW + 2 * COLUMN, NORMAL);
     }
     if (board.colorOf(position - ROW - 2 * COLUMN) == -turn ||
          board.getSquare(position - ROW - 2 * COLUMN) == EMPTYSQUARE) {
          moveList.push(position, position - ROW - 2 * COLUMN, NORMAL);
     }
     if (board.colorOf(position + 2 * ROW + COLUMN) == -turn ||
          board.getSquare(position + 2 * ROW + COLUMN) == EMPTYSQUARE) {
          moveList.push(position, position + 2 * ROW + COLUMN, NORMAL);
     }
     if (board.colorOf(position + 2 * ROW - COLUMN) == -turn ||
          board.getSquare(position + 2 * ROW - COLUMN) == EMPTYSQUARE) {
          moveList.push(position, position + 2 * ROW - COLUMN, NORMAL);
     }
     if (board.colorOf(position - 2 * ROW + COLUMN) == -turn ||
          board.getSquare(position - 2 * ROW + COLUMN) == EMPTYSQUARE) {
          moveList.push(position, position - 2 * ROW + COLUMN, NORMAL);
     }
     if (board.colorOf(position - 2 * ROW - COLUMN) == -turn ||
          board.getSquare(position - 2 * ROW - COLUMN) == EMPTYSQUARE) {
          moveList.push(position, position - 2 * ROW - COLUMN, NORMAL);
     }
}
void bishopMoveGeneration(const Board& board, const int position, MoveList& moveList) {
     int turn = board.getTurn();

     // TOPRIGHT
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[UPRIGHT]) == EMPTYSQUARE) {
               moveList.push(position, position + i*GO[UPRIGHT], NORMAL);
          }
          else if (board.colorOf(position + i*GO[UPRIGHT]) == -turn) {
               moveList.push(position, position + i*GO[UPRIGHT], NORMAL);
               break;
          }
          else { break; }
     }

     // DOWNRIGHT
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[DOWNRIGHT]) == EMPTYSQUARE) {
               moveList.push(position, position + i*GO[DOWNRIGHT], NORMAL);
          }
          else if (board.colorOf(position + i*GO[DOWNRIGHT]) == -turn) {
               moveList.push(position, position + i*GO[DOWNRIGHT], NORMAL);
               break;
          }
          else { break; }
     }

     // DOWNLEFT
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[DOWNLEFT]) == EMPTYSQUARE) {
               moveList.push(position, position + i*GO[DOWNLEFT], NORMAL);
          }
          else if (board.colorOf(position + i*GO[DOWNLEFT]) == -turn) {
               moveList.push(position, position + i*GO[DOWNLEFT], NORMAL);
               break;
          }
          else { break; }
     }

     // TOPLEFT
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[UPLEFT]) == EMPTYSQUARE) {
               moveList.push(position, position + i*GO[UPLEFT], NORMAL);
          }
          else if (board.colorOf(position + i*GO[UPLEFT]) == -turn) {
               moveList.push(position, position + i*GO[UPLEFT], NORMAL);
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
               moveList.push(position, position + i*GO[UP], NORMAL);
          }
          else if (board.colorOf(position + i*GO[UP]) == -turn) {
               moveList.push(position, position + i*GO[UP], NORMAL);
               break;
          }
          else { break; }
     }

     // DOWN
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[DOWN]) == EMPTYSQUARE) {
               moveList.push(position, position + i*GO[DOWN], NORMAL);
          }
          else if (board.colorOf(position + i*GO[DOWN]) == -turn) {
               moveList.push(position, position + i*GO[DOWN], NORMAL);
               break;
          }
          else { break; }
     }

     // LEFT
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[LEFT]) == EMPTYSQUARE) {
               moveList.push(position, position + i*GO[LEFT], NORMAL);
          }
          else if (board.colorOf(position + i*GO[LEFT]) == -turn) {
               moveList.push(position, position + i*GO[LEFT], NORMAL);
               break;
          }
          else { break; }
     }

     // RIGHT
     for (int i = 1; i < 8; i++) {
          if (board.getSquare(position + i*GO[RIGHT]) == EMPTYSQUARE) {
               moveList.push(position, position + i*GO[RIGHT], NORMAL);
          }
          else if (board.colorOf(position + i*GO[RIGHT]) == -turn) {
               moveList.push(position, position + i*GO[RIGHT], NORMAL);
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
          if (board.colorOf(position + GO[i]) == -board.getTurn() ||
               board.getSquare(position + GO[i]) == EMPTYSQUARE) {
               moveList.push(position, position + GO[i], NORMAL);
          }
     }
}

void castlingMoveGeneration(const Board& board, MoveList& moveList) {
     if (board.getTurn() == WHITE) {
          if (board.getCastlingRight(WKCASTLING) &&                                             //  neither piece moved
               board.getSquare(E1) == WHITEKING && board.getSquare(H1) == WHITEROOK &&     //  both pieces exists on board
               board.getSquare(F1) == EMPTYSQUARE && board.getSquare(G1) == EMPTYSQUARE && //  between them are empty
               board.isAttacked(E1) == false &&                                    //  not in check
               board.isAttacked(F1) == false &&                                    //  not attacked while moving
               board.isAttacked(G1) == false) {

               moveList.push(E1, G1, KINGSIDE_CASTLING);
          }
          if (board.getCastlingRight(WQCASTLING) && board.getSquare(B1) == EMPTYSQUARE &&
               board.getSquare(C1) == EMPTYSQUARE && board.getSquare(D1) == EMPTYSQUARE &&
               board.getSquare(E1) == WHITEKING && board.getSquare(A1) == WHITEROOK &&
               board.isAttacked(E1) == false &&
               board.isAttacked(C1) == false &&
               board.isAttacked(D1) == false) {
               moveList.push(E1, C1, QUEENSIDE_CASTLING);
          }

     }
     if (board.getTurn() == BLACK) {
          if (board.getCastlingRight(BKCASTLING) &&                                             //  neither piece moved
               board.getSquare(E8) == BLACKKING && board.getSquare(H8) == BLACKROOK &&     //  both pieces exists on board
               board.getSquare(F8) == EMPTYSQUARE && board.getSquare(G8) == EMPTYSQUARE && //  between them are empty
               board.isAttacked(E8) == false &&                                    //  not in check
               board.isAttacked(F8) == false &&                                    //  not attacked while moving
               board.isAttacked(G8) == false) {

               moveList.push(E8, G8, KINGSIDE_CASTLING);
          }
          if (board.getCastlingRight(BQCASTLING) && board.getSquare(B8) == EMPTYSQUARE &&
               board.getSquare(C8) == EMPTYSQUARE && board.getSquare(D8) == EMPTYSQUARE &&
               board.getSquare(E8) == BLACKKING && board.getSquare(A8) == BLACKROOK &&
               board.isAttacked(E8) == false &&
               board.isAttacked(C8) == false &&
               board.isAttacked(D8) == false) {
               moveList.push(E8, C8, QUEENSIDE_CASTLING);
          }
     }
}
void promotionMoveGeneration(const Board& board, const int position, MoveList& moveList) {
     if (board.getTurn() == WHITE) {
          if (board.colorOf(position + GO[UPLEFT]) == -board.getTurn()) {
               moveList.pushPromotion(position, position + GO[UPLEFT]);
          }
          if (board.colorOf(position + GO[UPRIGHT]) == -board.getTurn()) {
               moveList.pushPromotion(position, position + GO[UPRIGHT]);
          }
          if (board.getSquare(position + GO[UP]) == EMPTYSQUARE) {
               moveList.pushPromotion(position, position + GO[UP]);
          }
     }
     if (board.getTurn() == BLACK) {
          if (board.colorOf(position + GO[DOWNLEFT]) == -board.getTurn()) {
               moveList.pushPromotion(position, position + GO[DOWNLEFT]);
          }
          if (board.colorOf(position + GO[DOWNRIGHT]) == -board.getTurn()) {
               moveList.pushPromotion(position, position + GO[DOWNRIGHT]);
          }
          if (board.getSquare(position + GO[DOWN]) == EMPTYSQUARE) {
               moveList.pushPromotion(position, position + GO[DOWN]);
          }
     }
}
void enpassantMoveGeneration(const Board& board, MoveList& moveList) {
     if (board.getEnpassantSquare() == 0) { return; }

     int enpassantSquare = board.getEnpassantSquare();

     if (board.getTurn() == WHITE) {
          if (board.getSquare(enpassantSquare + GO[DOWNRIGHT]) == WHITEPAWN) {
               moveList.push(enpassantSquare + GO[DOWNRIGHT], enpassantSquare, ENPASSANT);
          }
          if (board.getSquare(enpassantSquare + GO[DOWNLEFT]) == WHITEPAWN) {
               moveList.push(enpassantSquare + GO[DOWNLEFT], enpassantSquare, ENPASSANT);
          }
     }
     if (board.getTurn() == BLACK) {
          if (board.getSquare(enpassantSquare + GO[UPRIGHT]) == BLACKPAWN) {
               moveList.push(enpassantSquare + GO[UPRIGHT], enpassantSquare, ENPASSANT);
          }
          if (board.getSquare(enpassantSquare + GO[UPLEFT]) == BLACKPAWN) {
               moveList.push(enpassantSquare + GO[UPLEFT], enpassantSquare, ENPASSANT);
          }
     }
}
