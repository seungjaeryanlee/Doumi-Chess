#include "board.h"
#include <string>
#include <iostream>
#include <stdexcept>

/******************************************************************************/
/* BOARD CLASS                                                                */
/******************************************************************************/
// Default Constructor
Board::Board() {}

// Constructor adding all content at once
Board::Board(std::array<int, 120> b, std::array<bool, 4> cc, color t, int e, int f, int m) {
     board = b;
     castlingRights = cc;
     turn = t;
     enpassantSquare = e;
     halfMoveClock = f;
     moveNumber = m;
}

//  Clone Method
Board::Board(const Board& originalBoard) {
     board = originalBoard.getBoard();
     castlingRights = originalBoard.getCastlingRights();
     turn = originalBoard.getTurn();
     enpassantSquare = originalBoard.getEnpassantSquare();
     halfMoveClock = originalBoard.getHalfMoveClock();
     moveNumber = originalBoard.getMoveNumber();
}

//  equality for threefold repetition checking
bool Board::isEqual_3F(const Board& thatBoard) {
     if (this == &thatBoard) {
          return true;
     }
     if (board == thatBoard.board &&
          castlingRights == thatBoard.castlingRights &&
          enpassantSquare == thatBoard.enpassantSquare) {
          return true;
     }
     else {
          return false;
     }
}

void Board::updateEndgame() {
     if (isEndgame) { return; }
     else {
          int queenCount = 0;
          for (int i = 0; i < 120; i++) {
               if (board[i] == WHITEQUEEN || board[i] == BLACKQUEEN) {
                    queenCount++;
               }
          }
          if (queenCount == 0) { isEndgame = true; }
          else { isEndgame = false; }
     }
}
void Board::updateEndgame(Move move) {
     if (isEndgame) { return; }
     if (move.getTerminal() != WHITEQUEEN && move.getTerminal() != BLACKQUEEN) { return; }
     else {
          int queenCount = 0;
          for (int i = 0; i < 120; i++) {
               if (board[i] == WHITEQUEEN || board[i] == BLACKQUEEN) {
                    queenCount++;
               }
          }
          if (queenCount == 0) { isEndgame = true; }
          else { isEndgame = false; }
     }
}
/*
void updatePieceCount() {
for (int i = 0; i < 14; i++) {
pieceCount[i] = 0;
}
for (int i = 0; i < 8; i++) {
for (int j = 0; j < 8; j++) {
int position120 = ROW*(i + 2) + (j + 1);
pieceCount[board[position120]]++;
}
}
}
void updatePieceCount(const Move& move, const int capturedPiece) {
if (capturedPiece != EMPTYSQUARE) {
pieceCount[capturedPiece]--;
return;
}
else if (move.getType() == KNIGHT_PROMOTION) {
if (move.getTerminal() == WHITEKNIGHT) {
pieceCount[WHITEPAWN]--;
pieceCount[WHITEKNIGHT]++;
return;
}
else {
pieceCount[BLACKPAWN]--;
pieceCount[BLACKKNIGHT]++;
return;
}
}
else if (move.getType() == KNIGHT_PROMOTION) {
if (move.getTerminal() == WHITEKNIGHT) {
pieceCount[WHITEPAWN]--;
pieceCount[WHITEKNIGHT]++;
return;
}
else {
pieceCount[BLACKPAWN]--;
pieceCount[BLACKKNIGHT]++;
return;
}
}
else if (move.getType() == BISHOP_PROMOTION) {
if (move.getTerminal() == WHITEBISHOP) {
pieceCount[WHITEPAWN]--;
pieceCount[WHITEBISHOP]++;
return;
}
else {
pieceCount[BLACKPAWN]--;
pieceCount[BLACKBISHOP]++;
return;
}
}
else if (move.getType() == ROOK_PROMOTION) {
if (move.getTerminal() == WHITEROOK) {
pieceCount[WHITEPAWN]--;
pieceCount[WHITEROOK]++;
return;
}
else {
pieceCount[BLACKPAWN]--;
pieceCount[BLACKROOK]++;
return;
}
}
else if (move.getType() == QUEEN_PROMOTION) {
if (move.getTerminal() == WHITEQUEEN) {
pieceCount[WHITEPAWN]--;
pieceCount[WHITEQUEEN]++;
}
else {
pieceCount[BLACKPAWN]--;
pieceCount[BLACKQUEEN]++;
}
}
}
*/

/// <summary>
/// This function returns evaluation score of the board using piece values and PCSQ tables. Positive score signifies white's advantage.
/// </summary>
/// <returns>The score of the board</returns>
int Board::evaluate() {
     int score = 0;

     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               int position120 = ROW*(i + 2) + (j + 1);
               if (!isEndgame) {
                    score += PIECEVALUE[board[position120]] + PCSQVALUE[board[position120]][position120];
               }
               else {
                    score += PIECEVALUE[board[position120]] + PCSQVALUE_ENDGAME[board[position120]][position120];
               }
          }
     }
     return score;
}






void board120Setup(Board& board) {
     board.setTurn(WHITE);
     board.setEnpassantSquare(0);
     board.setMoveNumber(1);
     board.setHalfMoveClock(0);
     board.setCastlingRights({ true, true, true, true });

     //  Add Empty Squares
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               board.setSquare((i + 2) * 10 + j + 1, EMPTYSQUARE);
          }
     }

     //  Add Error Squares
     for (int j = 0; j < 10; j++) {
          board.setSquare(j, ERRORSQUARE);
          board.setSquare(10 + j, ERRORSQUARE);
          board.setSquare(100 + j, ERRORSQUARE);
          board.setSquare(110 + j, ERRORSQUARE);
     }
     for (int j = 0; j < 12; j++) {
          board.setSquare(10 * j, ERRORSQUARE);
          board.setSquare(10 * j + 9, ERRORSQUARE);
     }

     //  Add Non-Pawn Pieces
     board.setSquare(A8, BLACKROOK);
     board.setSquare(B8, BLACKKNIGHT);
     board.setSquare(C8, BLACKBISHOP);
     board.setSquare(D8, BLACKQUEEN);
     board.setSquare(E8, BLACKKING);
     board.setSquare(F8, BLACKBISHOP);
     board.setSquare(G8, BLACKKNIGHT);
     board.setSquare(H8, BLACKROOK);

     board.setSquare(A1, WHITEROOK);
     board.setSquare(B1, WHITEKNIGHT);
     board.setSquare(C1, WHITEBISHOP);
     board.setSquare(D1, WHITEQUEEN);
     board.setSquare(E1, WHITEKING);
     board.setSquare(F1, WHITEBISHOP);
     board.setSquare(G1, WHITEKNIGHT);
     board.setSquare(H1, WHITEROOK);

     //  Add Pawn Pieces
     for (int i = 0; i < 8; i++) {
          board.setSquare(A2 + i, WHITEPAWN);
          board.setSquare(A7 + i, BLACKPAWN);
     }
}
void FENboardSetup(Board& board, const std::string FEN) {
     board.setCastlingRights({ false, false, false, false });
     board.setEnpassantSquare(0);

     //  Add Error Squares
     for (int j = 0; j < 10; j++) {
          board.setSquare(j, ERRORSQUARE);
          board.setSquare(10 + j, ERRORSQUARE);
          board.setSquare(100 + j, ERRORSQUARE);
          board.setSquare(110 + j, ERRORSQUARE);
     }
     for (int j = 0; j < 12; j++) {
          board.setSquare(10 * j, ERRORSQUARE);
          board.setSquare(10 * j + 9, ERRORSQUARE);
     }

     int currentSquare = 21;
     unsigned int i;
     for (i = 0; i < FEN.length(); i++) {
          if (FEN.at(i) == ' ') { break; }
          if (FEN.at(i) == '/') {
               //  next row, first square
               currentSquare = currentSquare / ROW*ROW + ROW + 1;
               continue;
          }
          else if ('1' <= FEN.at(i) && FEN.at(i) <= '8') {
               for (int k = 0; k < FEN.at(i) - '0'; k++) {
                    board.setSquare(currentSquare, EMPTYSQUARE);
                    currentSquare++;
               }
          }
          else {
               switch (FEN.at(i)) {
               case 'p':
                    board.setSquare(currentSquare, BLACKPAWN);
                    break;
               case 'r':
                    board.setSquare(currentSquare, BLACKROOK);
                    break;
               case 'n':
                    board.setSquare(currentSquare, BLACKKNIGHT);
                    break;
               case 'b':
                    board.setSquare(currentSquare, BLACKBISHOP);
                    break;
               case 'q':
                    board.setSquare(currentSquare, BLACKQUEEN);
                    break;
               case 'k':
                    board.setSquare(currentSquare, BLACKKING);
                    break;
               case 'P':
                    board.setSquare(currentSquare, WHITEPAWN);
                    break;
               case 'R':
                    board.setSquare(currentSquare, WHITEROOK);
                    break;
               case 'N':
                    board.setSquare(currentSquare, WHITEKNIGHT);
                    break;
               case 'B':
                    board.setSquare(currentSquare, WHITEBISHOP);
                    break;
               case 'Q':
                    board.setSquare(currentSquare, WHITEQUEEN);
                    break;
               case 'K':
                    board.setSquare(currentSquare, WHITEKING);
                    break;
               }
               currentSquare++;
          }
     }
     i++;
     if (FEN.at(i) == 'w') { board.setTurn(WHITE); }
     else { board.setTurn(BLACK); }

     i += 2;
     if (FEN.at(i) != '-') {
          while (FEN.at(i) != ' ') {
               if (FEN.at(i) == 'K') {
                    board.setCastlingRight(WKCASTLING, true);
               }
               if (FEN.at(i) == 'Q') {
                    board.setCastlingRight(WQCASTLING, true);
               }
               if (FEN.at(i) == 'k') {
                    board.setCastlingRight(BKCASTLING, true);
               }
               if (FEN.at(i) == 'q') {
                    board.setCastlingRight(BQCASTLING, true);
               }
               i++;
          }
          i++;
     }
     else { i += 2; }

     if (FEN.at(i) != '-') {
          //  get enpassant square
          int enpassantSquare;
          enpassantSquare = COLUMN*(FEN.at(i) - 'a' + 1);
          i++;
          enpassantSquare += ROW*('9' - FEN.at(i) + 1);

          board.setEnpassantSquare(enpassantSquare);
     }

     i += 2;
     // One-digit Fifty Move Count
     if (FEN.at(i + 1) == ' ') {
          board.setHalfMoveClock(FEN.at(i) - '0');
          i += 2;
     }
     // Two-digit Fifty Move Count
     else if ('0' <= FEN.at(i + 1) && FEN.at(i + 1) <= '9') {
          if (FEN.at(i + 2) == ' ') {
               board.setHalfMoveClock(10 * (FEN.at(i) - '0') + (FEN.at(i + 1) - '0'));
               i += 3;
          }
          else if ('0' <= FEN.at(i + 2) && FEN.at(i + 2) <= '9') {
               board.setHalfMoveClock(100 * (FEN.at(i) - '0') + 
                                       10 * (FEN.at(i + 1) - '0') + 
                                            (FEN.at(i + 2) - '0'));
               i += 4;
          }
     }

     // One-digit Move Number
     if (FEN.length() == i + 1 || FEN.at(i + 1) == ' ') {
          board.setMoveNumber(FEN.at(i) - '0');
     }
     // Two-digit Move Number
     else if ('0' <= FEN.at(i + 1) && FEN.at(i + 1) <= '9') {
          board.setMoveNumber(10 * (FEN.at(i) - '0') + (FEN.at(i + 1) - '0'));
     }


}
std::string boardToFEN(const Board& board) {
     std::string FEN;
     int emptySquareCount = 0;

     for (int i = 2; i < 10; i++) {
          for (int j = 1; j < 9; j++) {
               if (board.getSquare(i*ROW + j*COLUMN) == EMPTYSQUARE) {
                    emptySquareCount++;
                    continue;
               }
               if (board.getSquare(i*ROW + j*COLUMN) != EMPTYSQUARE &&
                    emptySquareCount != 0) {
                    FEN += ('0' + emptySquareCount);
                    emptySquareCount = 0;
               }
               switch (board.getSquare(i*ROW + j*COLUMN)) {
               case WHITEPAWN:
                    FEN += 'P';
                    break;
               case WHITEKNIGHT:
                    FEN += 'N';
                    break;
               case WHITEBISHOP:
                    FEN += 'B';
                    break;
               case WHITEROOK:
                    FEN += 'R';
                    break;
               case WHITEQUEEN:
                    FEN += 'Q';
                    break;
               case WHITEKING:
                    FEN += 'K';
                    break;
               case BLACKPAWN:
                    FEN += 'p';
                    break;
               case BLACKKNIGHT:
                    FEN += 'n';
                    break;
               case BLACKBISHOP:
                    FEN += 'b';
                    break;
               case BLACKROOK:
                    FEN += 'r';
                    break;
               case BLACKQUEEN:
                    FEN += 'q';
                    break;
               case BLACKKING:
                    FEN += 'k';
                    break;

               }
          }
          if (emptySquareCount != 0) {
               FEN += ('0' + emptySquareCount);
               emptySquareCount = 0;
          }

          if (i != 9) {
               FEN += '/';
          }

     }

     FEN += ' ';
     if (board.getTurn() == WHITE) { FEN += 'w'; }
     else { FEN += 'b'; }

     FEN += ' ';
     //  no castling available
     bool WKCastling = board.getCastlingRight(WKCASTLING);
     bool WQCastling = board.getCastlingRight(WQCASTLING);
     bool BKCastling = board.getCastlingRight(BKCASTLING);
     bool BQCastling = board.getCastlingRight(BQCASTLING);
     if (!(WKCastling || WQCastling || BKCastling || BQCastling)) {
          FEN += '-';
     }
     else {
          if (WKCastling) {
               FEN += 'K';
          }
          if (WQCastling) {
               FEN += 'Q';
          }
          if (BKCastling) {
               FEN += 'k';
          }
          if (BQCastling) {
               FEN += 'q';
          }
     }

     FEN += ' ';
     if (board.getEnpassantSquare() != 0) {
          FEN += numberToFile(board.getEnpassantSquare());
          FEN += ('0' + numberToRank(board.getEnpassantSquare()));
     }
     else { FEN += '-'; }

     FEN += ' ';
     FEN += std::to_string(board.getHalfMoveClock());
     FEN += ' ';
     FEN += std::to_string(board.getMoveNumber());

     std::cout << FEN << std::endl;
     return FEN;

}

int checkColor(const int pieceType) {
     if (WHITEPAWN <= pieceType && pieceType <= WHITEKING) {
          return WHITE;
     }
     else if (BLACKPAWN <= pieceType && pieceType <= BLACKKING) {
          return BLACK;
     }
     else if (pieceType == EMPTYSQUARE || pieceType == ERRORSQUARE) {
          return NEITHER;
     }
     else {
          throw std::invalid_argument("Invalid pieceType.");
     }
}

int filerankToNumber(const char file, const int rank) {
     //  if it is not a correct filerank format, return -1
     if ('a' > file || file > 'h' || 1 > rank || rank > 8) {
          return -1;
     }
     return COLUMN*(file - 'a' + 1) + ROW*(9 - (rank - 1));
}

bool isSquareAttacked(const Board& board, int kingPos) {
     if (board.getTurn() == WHITE) {
          //  1. pawn
          if (board.getSquare(kingPos - ROW - COLUMN) == BLACKPAWN ||
               board.getSquare(kingPos - ROW + COLUMN) == BLACKPAWN) {
               return true;
          }
          //  2. knight
          if (board.getSquare(kingPos - ROW - 2 * COLUMN) == BLACKKNIGHT ||
               board.getSquare(kingPos - ROW + 2 * COLUMN) == BLACKKNIGHT ||
               board.getSquare(kingPos + ROW - 2 * COLUMN) == BLACKKNIGHT ||
               board.getSquare(kingPos + ROW + 2 * COLUMN) == BLACKKNIGHT ||
               board.getSquare(kingPos - 2 * ROW - COLUMN) == BLACKKNIGHT ||
               board.getSquare(kingPos - 2 * ROW + COLUMN) == BLACKKNIGHT ||
               board.getSquare(kingPos + 2 * ROW - COLUMN) == BLACKKNIGHT ||
               board.getSquare(kingPos + 2 * ROW + COLUMN) == BLACKKNIGHT) {
               return true;
          }
          //  3. bishop
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos - i*ROW - i*COLUMN) == BLACKBISHOP ||
                    board.getSquare(kingPos - i*ROW - i*COLUMN) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos - i*ROW - i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos - i*ROW - i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos - i*ROW + i*COLUMN) == BLACKBISHOP ||
                    board.getSquare(kingPos - i*ROW + i*COLUMN) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos - i*ROW + i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos - i*ROW + i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos + i*ROW - i*COLUMN) == BLACKBISHOP ||
                    board.getSquare(kingPos + i*ROW - i*COLUMN) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos + i*ROW - i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos + i*ROW - i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos + i*ROW + i*COLUMN) == BLACKBISHOP ||
                    board.getSquare(kingPos + i*ROW + i*COLUMN) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos + i*ROW + i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos + i*ROW + i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          //  4. rook
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos - i*ROW) == BLACKROOK ||
                    board.getSquare(kingPos - i*ROW) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos - i*ROW) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos - i*ROW) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos + i*ROW) == BLACKROOK ||
                    board.getSquare(kingPos + i*ROW) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos + i*ROW) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos + i*ROW) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos - i*COLUMN) == BLACKROOK ||
                    board.getSquare(kingPos - i*COLUMN) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos - i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos - i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos + i*COLUMN) == BLACKROOK ||
                    board.getSquare(kingPos + i*COLUMN) == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos + i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos + i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          //  (5. queen: added to bishop and rook)

          //  6. king: is it needed?
          if (board.getSquare(kingPos + 1) == BLACKKING ||
               board.getSquare(kingPos - 1) == BLACKKING ||
               board.getSquare(kingPos + 11) == BLACKKING ||
               board.getSquare(kingPos - 11) == BLACKKING ||
               board.getSquare(kingPos + 9) == BLACKKING ||
               board.getSquare(kingPos - 9) == BLACKKING ||
               board.getSquare(kingPos + 10) == BLACKKING ||
               board.getSquare(kingPos - 10) == BLACKKING) {
               return true;
          }

          return false;
     }

     else if (board.getTurn() == BLACK) {
          //  1. pawn
          if (board.getSquare(kingPos + ROW - COLUMN) == WHITEPAWN ||
               board.getSquare(kingPos + ROW + COLUMN) == WHITEPAWN) {
               return true;
          }
          //  2. knight
          if (board.getSquare(kingPos - ROW - 2 * COLUMN) == WHITEKNIGHT ||
               board.getSquare(kingPos - ROW + 2 * COLUMN) == WHITEKNIGHT ||
               board.getSquare(kingPos + ROW - 2 * COLUMN) == WHITEKNIGHT ||
               board.getSquare(kingPos + ROW + 2 * COLUMN) == WHITEKNIGHT ||
               board.getSquare(kingPos - 2 * ROW - COLUMN) == WHITEKNIGHT ||
               board.getSquare(kingPos - 2 * ROW + COLUMN) == WHITEKNIGHT ||
               board.getSquare(kingPos + 2 * ROW - COLUMN) == WHITEKNIGHT ||
               board.getSquare(kingPos + 2 * ROW + COLUMN) == WHITEKNIGHT) {
               return true;
          }
          //  3. bishop
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos - i*ROW - i*COLUMN) == WHITEBISHOP ||
                    board.getSquare(kingPos - i*ROW - i*COLUMN) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos - i*ROW - i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos - i*ROW - i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos - i*ROW + i*COLUMN) == WHITEBISHOP ||
                    board.getSquare(kingPos - i*ROW + i*COLUMN) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos - i*ROW + i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos - i*ROW + i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos + i*ROW - i*COLUMN) == WHITEBISHOP ||
                    board.getSquare(kingPos + i*ROW - i*COLUMN) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos + i*ROW - i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos + i*ROW - i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos + i*ROW + i*COLUMN) == WHITEBISHOP ||
                    board.getSquare(kingPos + i*ROW + i*COLUMN) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos + i*ROW + i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos + i*ROW + i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          //  4. rook
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos - i*ROW) == WHITEROOK ||
                    board.getSquare(kingPos - i*ROW) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos - i*ROW) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos - i*ROW) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos + i*ROW) == WHITEROOK ||
                    board.getSquare(kingPos + i*ROW) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos + i*ROW) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos + i*ROW) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos - i*COLUMN) == WHITEROOK ||
                    board.getSquare(kingPos - i*COLUMN) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos - i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos - i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board.getSquare(kingPos + i*COLUMN) == WHITEROOK ||
                    board.getSquare(kingPos + i*COLUMN) == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board.getSquare(kingPos + i*COLUMN) != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board.getSquare(kingPos + i*COLUMN) == ERRORSQUARE) {
                    break;
               }
          }

          //  (5. queen: added to bishop and rook)

          //  6. king: is it needed?
          if (board.getSquare(kingPos + 1) == WHITEKING ||
               board.getSquare(kingPos - 1) == WHITEKING ||
               board.getSquare(kingPos + 11) == WHITEKING ||
               board.getSquare(kingPos - 11) == WHITEKING ||
               board.getSquare(kingPos + 9) == WHITEKING ||
               board.getSquare(kingPos - 9) == WHITEKING ||
               board.getSquare(kingPos + 10) == WHITEKING ||
               board.getSquare(kingPos - 10) == WHITEKING) {
               return true;
          }

          return false;
     }

     else {
          throw std::invalid_argument("The board has invalid turn.");
     }

}

void printBoard(const Board& board) {
     for (int i = 0; i < 120; i++) {
          if (i % 10 == 0) {
               printf("\n");
          }
          switch (board.getSquare(i)) {
          case ERRORSQUARE:
               printf("X ");
               break;
          case EMPTYSQUARE:
               printf("- ");
               break;
          case WHITEPAWN:
               printf("P ");
               break;
          case WHITEKNIGHT:
               printf("N ");
               break;
          case WHITEBISHOP:
               printf("B ");
               break;
          case WHITEROOK:
               printf("R ");
               break;
          case WHITEQUEEN:
               printf("Q ");
               break;
          case WHITEKING:
               printf("K ");
               break;
          case BLACKPAWN:
               printf("p ");
               break;
          case BLACKKNIGHT:
               printf("n ");
               break;
          case BLACKBISHOP:
               printf("b ");
               break;
          case BLACKROOK:
               printf("r ");
               break;
          case BLACKQUEEN:
               printf("q ");
               break;
          case BLACKKING:
               printf("k ");
               break;
          }
     }
     printf("\n");
}
void printSimpleBoard(const Board& board) {
     for (int i = 2; i < 10; i++) {
          printf("%d| ", 10 - i);
          for (int j = 1; j < 9; j++) {
               switch (board.getSquare(i*ROW + j*COLUMN)) {
               case ERRORSQUARE:
                    printf("X ");
                    break;
               case EMPTYSQUARE:
                    printf("- ");
                    break;
               case WHITEPAWN:
                    printf("P ");
                    break;
               case WHITEKNIGHT:
                    printf("N ");
                    break;
               case WHITEBISHOP:
                    printf("B ");
                    break;
               case WHITEROOK:
                    printf("R ");
                    break;
               case WHITEQUEEN:
                    printf("Q ");
                    break;
               case WHITEKING:
                    printf("K ");
                    break;
               case BLACKPAWN:
                    printf("p ");
                    break;
               case BLACKKNIGHT:
                    printf("n ");
                    break;
               case BLACKBISHOP:
                    printf("b ");
                    break;
               case BLACKROOK:
                    printf("r ");
                    break;
               case BLACKQUEEN:
                    printf("q ");
                    break;
               case BLACKKING:
                    printf("k ");
                    break;
               }
          }
          printf("\n");
     }
     printf("  ----------------\n");
     printf("   a b c d e f g h\n");
}


int makeMove(Board &board, const Move& move) {
     int capturedPiece = EMPTYSQUARE;
     int initial = move.getInitial(), terminal = move.getTerminal(), moveType = move.getType();

     board.setEnpassantSquare(0);
     board.changeTurn();

     if (moveType == NORMAL) {
          capturedPiece = board.getSquare(terminal);
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
     }
     else if (moveType == DOUBLEMOVE) {
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          board.setEnpassantSquare((terminal + initial) / 2);
     }
     else if (moveType == QUEENSIDE_CASTLING) {
          //  move king
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          //  move rook
          board.setSquare(terminal + COLUMN, board.getSquare(initial - 4 * COLUMN));
          board.setSquare(initial - 4 * COLUMN, EMPTYSQUARE);
          //  castling does not involve capture
     }
     else if (moveType == KINGSIDE_CASTLING) {
          //  move king
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          //  move rook
          board.setSquare(terminal - COLUMN, board.getSquare(terminal + COLUMN));
          board.setSquare(terminal + COLUMN, EMPTYSQUARE);
          //  castling does not involve capture
     }
     else if (moveType == KNIGHT_PROMOTION) {
          capturedPiece = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEKNIGHT);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKKNIGHT);
          }
          board.setSquare(initial, EMPTYSQUARE);
     }
     else if (moveType == BISHOP_PROMOTION) {
          capturedPiece = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEBISHOP);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKBISHOP);
          }
          board.setSquare(initial, EMPTYSQUARE);
     }
     else if (moveType == ROOK_PROMOTION) {
          capturedPiece = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEROOK);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKROOK);
          }
          board.setSquare(initial, EMPTYSQUARE);
     }
     else if (moveType == QUEEN_PROMOTION) {
          capturedPiece = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEQUEEN);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKQUEEN);
          }
          board.setSquare(initial, EMPTYSQUARE);
     }
     else if (moveType == ENPASSANT) {
          //  White turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, board.getSquare(initial));
               board.setSquare(initial, EMPTYSQUARE);
               board.setSquare(terminal + ROW, EMPTYSQUARE);
               capturedPiece = BLACKPAWN;
          }
          //  Black turn
          else {
               board.setSquare(terminal, board.getSquare(initial));
               board.setSquare(initial, EMPTYSQUARE);
               board.setSquare(terminal - ROW, EMPTYSQUARE);
               capturedPiece = WHITEPAWN;
          }
     }
     else {
          std::invalid_argument("makeMove() called with invalid moveType.");
     }

     updateBoard(board, move, capturedPiece);
     return capturedPiece;
}
void undoMove(Board &board, const Move& move, const int capturedPiece) {
     int initial = move.getInitial(), terminal = move.getTerminal(), moveType = move.getType();

     board.changeTurn();

     if (moveType == NORMAL) {
          board.setSquare(initial, board.getSquare(terminal));
          board.setSquare(terminal, capturedPiece);
     }
     else if (moveType == DOUBLEMOVE) {
          board.setSquare(initial, board.getSquare(terminal));
          board.setSquare(terminal, EMPTYSQUARE);
     }
     else if (moveType == QUEENSIDE_CASTLING) {
          //  undo king move
          board.setSquare(initial, board.getSquare(terminal));
          board.setSquare(terminal, EMPTYSQUARE);

          //  undo rook move
          board.setSquare(initial - 4 * COLUMN, board.getSquare(terminal + COLUMN));
          board.setSquare(terminal + COLUMN, EMPTYSQUARE);

     }
     else if (moveType == KINGSIDE_CASTLING) {
          //  undo king move
          board.setSquare(initial, board.getSquare(terminal));
          board.setSquare(terminal, EMPTYSQUARE);

          //  undo rook move
          board.setSquare(terminal + COLUMN, board.getSquare(terminal - COLUMN));
          board.setSquare(terminal - COLUMN, EMPTYSQUARE);
     }
     else if (moveType == KNIGHT_PROMOTION || moveType == BISHOP_PROMOTION ||
          moveType == ROOK_PROMOTION || moveType == QUEEN_PROMOTION) {
          //  white turn
          if (checkColor(board.getSquare(terminal)) == WHITE) {
               board.setSquare(terminal, capturedPiece);
               board.setSquare(initial, WHITEPAWN);
          }
          //  black turn
          else {
               board.setSquare(terminal, capturedPiece);
               board.setSquare(initial, BLACKPAWN);
          }
     }
     else if (moveType == ENPASSANT) {
          //  white turn
          if (board.getSquare(terminal) == WHITEPAWN) {
               board.setSquare(terminal, EMPTYSQUARE);
               board.setSquare(initial, WHITEPAWN);
               board.setSquare(terminal + ROW, BLACKPAWN);
          }
          //  black turn
          else {
               board.setSquare(terminal, EMPTYSQUARE);
               board.setSquare(initial, BLACKPAWN);
               board.setSquare(terminal - ROW, WHITEPAWN);
          }
     }
}

void updateCastling(Board& board, const Move& move) {
     if (board.getSquare(move.getInitial()) == WHITEKING) {
          board.setCastlingRight(WKCASTLING, false);
          board.setCastlingRight(WQCASTLING, false);
     }
     if (board.getSquare(move.getInitial()) == BLACKKING) {
          board.setCastlingRight(BKCASTLING, false);
          board.setCastlingRight(BQCASTLING, false);
     }
     if (board.getSquare(move.getInitial()) == WHITEROOK) {
          if (move.getInitial() == A1) {
               board.setCastlingRight(WQCASTLING, false);
          }
          if (move.getInitial() == H1) {
               board.setCastlingRight(WKCASTLING, false);
          }
     }
     if (board.getSquare(move.getInitial()) == BLACKROOK) {
          if (move.getInitial() == A8) {
               board.setCastlingRight(BQCASTLING, false);
          }
          if (move.getInitial() == H8) {
               board.setCastlingRight(BKCASTLING, false);
          }
     }
}
void updateEnPassant(Board& board, const Move& move) {
     if (move.getType() == DOUBLEMOVE) {
          board.setEnpassantSquare((move.getInitial() + move.getTerminal()) / 2);
     }
     else { board.setEnpassantSquare(0); }
}
void updateHalfMoveClock(Board& board, const Move& move) {
     if (board.getSquare(move.getTerminal()) == EMPTYSQUARE
          && board.getSquare(move.getInitial()) != WHITEPAWN
          && board.getSquare(move.getInitial()) != BLACKPAWN) {
          board.incrementHalfMoveClock();
     }
     else { board.setHalfMoveClock(0); }
}
void updateMoveNumber(Board& board) {
     if (board.getTurn() == WHITE) { board.incrementMoveNumber(); }
}

void updateBoard(Board& board, const Move& move, const int capturedPiece) {
     updateCastling(board, move);
     updateEnPassant(board, move);
     updateHalfMoveClock(board, move);
     board.updateEndgame(move);
     //board.updatePieceCount(move, capturedPiece);
     updateMoveNumber(board);
}

bool fiftyMoveCheck(const Board& board) {
     return (board.getHalfMoveClock() >= 100);
}
