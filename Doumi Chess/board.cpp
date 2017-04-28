/******************************************************************************/
/* board.cpp                                                                  */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

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

void Board::setup() {
     turn = WHITE;
     enpassantSquare = 0;
     moveNumber = 1;
     halfMoveClock = 0;
     castlingRights = { true, true, true, true };

     //  Add Empty Squares
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               board[(i + 2) * 10 + j + 1] = EMPTYSQUARE;
          }
     }

     //  Add Error Squares
     for (int j = 0; j < 10; j++) {
          board[j] = ERRORSQUARE;
          board[10 + j] = ERRORSQUARE;
          board[100 + j] = ERRORSQUARE;
          board[110 + j] = ERRORSQUARE;
     }
     for (int j = 0; j < 12; j++) {
          board[10 * j] = ERRORSQUARE;
          board[10 * j + 9] = ERRORSQUARE;
     }

     //  Add Non-Pawn Pieces
     board[A8] = BLACKROOK;
     board[B8] = BLACKKNIGHT;
     board[C8] = BLACKBISHOP;
     board[D8] = BLACKQUEEN;
     board[E8] = BLACKKING;
     board[F8] = BLACKBISHOP;
     board[G8] = BLACKKNIGHT;
     board[H8] = BLACKROOK;

     board[A1] = WHITEROOK;
     board[B1] = WHITEKNIGHT;
     board[C1] = WHITEBISHOP;
     board[D1] = WHITEQUEEN;
     board[E1] = WHITEKING;
     board[F1] = WHITEBISHOP;
     board[G1] = WHITEKNIGHT;
     board[H1] = WHITEROOK;

     //  Add Pawn Pieces
     for (int i = 0; i < 8; i++) {
          board[A2 + i] = WHITEPAWN;
          board[A7 + i] = BLACKPAWN;
     }
}

void Board::import(const std::string FEN) {
     castlingRights = { false, false, false, false };
     enpassantSquare = 0;

     //  Add Error Squares
     for (int j = 0; j < 10; j++) {
          board[j] = ERRORSQUARE;
          board[10 + j] = ERRORSQUARE;
          board[100 + j] = ERRORSQUARE;
          board[110 + j] = ERRORSQUARE;
     }
     for (int j = 0; j < 12; j++) {
          board[10 * j] = ERRORSQUARE;
          board[10 * j + 9] = ERRORSQUARE;
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
                    board[currentSquare] = EMPTYSQUARE;
                    currentSquare++;
               }
          }
          else {
               switch (FEN.at(i)) {
               case 'p':
                    board[currentSquare] = BLACKPAWN;
                    break;
               case 'r':
                    board[currentSquare] = BLACKROOK;
                    break;
               case 'n':
                    board[currentSquare] = BLACKKNIGHT;
                    break;
               case 'b':
                    board[currentSquare] = BLACKBISHOP;
                    break;
               case 'q':
                    board[currentSquare] = BLACKQUEEN;
                    break;
               case 'k':
                    board[currentSquare] = BLACKKING;
                    break;
               case 'P':
                    board[currentSquare] = WHITEPAWN;
                    break;
               case 'R':
                    board[currentSquare] = WHITEROOK;
                    break;
               case 'N':
                    board[currentSquare] = WHITEKNIGHT;
                    break;
               case 'B':
                    board[currentSquare] = WHITEBISHOP;
                    break;
               case 'Q':
                    board[currentSquare] = WHITEQUEEN;
                    break;
               case 'K':
                    board[currentSquare] = WHITEKING;
                    break;
               }
               currentSquare++;
          }
     }
     i++;
     if (FEN.at(i) == 'w') { turn = WHITE; }
     else { turn = BLACK; }

     i += 2;
     if (FEN.at(i) != '-') {
          while (FEN.at(i) != ' ') {
               if (FEN.at(i) == 'K') {
                    castlingRights[WKCASTLING] = true;
               }
               if (FEN.at(i) == 'Q') {
                    castlingRights[WQCASTLING] = true;
               }
               if (FEN.at(i) == 'k') {
                    castlingRights[BKCASTLING] = true;
               }
               if (FEN.at(i) == 'q') {
                    castlingRights[BQCASTLING] = true;
               }
               i++;
          }
          i++;
     }
     else { i += 2; }

     // Set enpassant square
     if (FEN.at(i) != '-') {
          enpassantSquare = COLUMN*(FEN.at(i) - 'a' + 1);
          i++;
          enpassantSquare += ROW*('9' - FEN.at(i) + 1);
     }

     i += 2;
     // One-digit Fifty Move Count
     if (FEN.at(i + 1) == ' ') {
          halfMoveClock = FEN.at(i) - '0';
          i += 2;
     }
     // Two-digit Fifty Move Count
     else if ('0' <= FEN.at(i + 1) && FEN.at(i + 1) <= '9') {
          if (FEN.at(i + 2) == ' ') {
               halfMoveClock = 10 * (FEN.at(i) - '0') + (FEN.at(i + 1) - '0');
               i += 3;
          }
          else if ('0' <= FEN.at(i + 2) && FEN.at(i + 2) <= '9') {
               halfMoveClock = 100 * (FEN.at(i) - '0') +
                                10 * (FEN.at(i + 1) - '0') +
                                     (FEN.at(i + 2) - '0');
               i += 4;
          }
     }

     // One-digit Move Number
     if (FEN.length() == i + 1 || FEN.at(i + 1) == ' ') {
          moveNumber = FEN.at(i) - '0';
     }
     // Two-digit Move Number
     else if ('0' <= FEN.at(i + 1) && FEN.at(i + 1) <= '9') {
          moveNumber = 10 * (FEN.at(i) - '0') + (FEN.at(i + 1) - '0');
     }
}

std::string Board::fen() {
     std::string FEN;
     int emptySquareCount = 0;

     for (int i = 2; i < 10; i++) {
          for (int j = 1; j < 9; j++) {
               if (board[i*ROW + j*COLUMN] == EMPTYSQUARE) {
                    emptySquareCount++;
                    continue;
               }
               if (board[i*ROW + j*COLUMN] != EMPTYSQUARE &&
                    emptySquareCount != 0) {
                    FEN += ('0' + emptySquareCount);
                    emptySquareCount = 0;
               }
               switch (board[i*ROW + j*COLUMN]) {
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
     if (turn == WHITE) { FEN += 'w'; }
     else { FEN += 'b'; }

     FEN += ' ';
     //  no castling available
     bool WKCastling = castlingRights[WKCASTLING];
     bool WQCastling = castlingRights[WQCASTLING];
     bool BKCastling = castlingRights[BKCASTLING];
     bool BQCastling = castlingRights[BQCASTLING];
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
     if (enpassantSquare != 0) {
          FEN += numberToFile(enpassantSquare);
          FEN += ('0' + numberToRank(enpassantSquare));
     }
     else { FEN += '-'; }

     FEN += ' ';
     FEN += std::to_string(halfMoveClock);
     FEN += ' ';
     FEN += std::to_string(moveNumber);

     return FEN;
}

const bool Board::isAttacked(const int position) const {
     if (turn == WHITE) {
          //  1. pawn
          if (board[position - ROW - COLUMN] == BLACKPAWN ||
              board[position - ROW + COLUMN] == BLACKPAWN) {
               return true;
          }
          //  2. knight
          if (board[position - ROW - 2 * COLUMN] == BLACKKNIGHT ||
              board[position - ROW + 2 * COLUMN] == BLACKKNIGHT ||
              board[position + ROW - 2 * COLUMN] == BLACKKNIGHT ||
              board[position + ROW + 2 * COLUMN] == BLACKKNIGHT ||
              board[position - 2 * ROW - COLUMN] == BLACKKNIGHT ||
              board[position - 2 * ROW + COLUMN] == BLACKKNIGHT ||
              board[position + 2 * ROW - COLUMN] == BLACKKNIGHT ||
              board[position + 2 * ROW + COLUMN] == BLACKKNIGHT) {
               return true;
          }
          //  3. bishop
          for (int i = 1; i < 8; i++) {
               if (board[position - i*ROW - i*COLUMN] == BLACKBISHOP ||
                    board[position - i*ROW - i*COLUMN] == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position - i*ROW - i*COLUMN] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position - i*ROW - i*COLUMN] == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board[position - i*ROW + i*COLUMN] == BLACKBISHOP ||
                   board[position - i*ROW + i*COLUMN] == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position - i*ROW + i*COLUMN] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position - i*ROW + i*COLUMN] == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board[position + i*ROW - i*COLUMN] == BLACKBISHOP ||
                   board[position + i*ROW - i*COLUMN] == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position + i*ROW - i*COLUMN] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position + i*ROW - i*COLUMN] == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board[position + i*ROW + i*COLUMN] == BLACKBISHOP ||
                    board[position + i*ROW + i*COLUMN] == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position + i*ROW + i*COLUMN] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position + i*ROW + i*COLUMN] == ERRORSQUARE) {
                    break;
               }
          }
          //  4. rook
          for (int i = 1; i < 8; i++) {
               if (board[position - i*ROW] == BLACKROOK ||
                    board[position - i*ROW] == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position - i*ROW] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position - i*ROW] == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board[position + i*ROW] == BLACKROOK ||
                    board[position + i*ROW] == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position + i*ROW] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position + i*ROW] == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board[position - i*COLUMN] == BLACKROOK ||
                    board[position - i*COLUMN] == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position - i*COLUMN] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position - i*COLUMN] == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board[position + i*COLUMN] == BLACKROOK ||
                    board[position + i*COLUMN] == BLACKQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position + i*COLUMN] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position + i*COLUMN] == ERRORSQUARE) {
                    break;
               }
          }
          //  (5. queen: added to bishop and rook)

          //  6. king: is it needed?
          if (board[position + 1] == BLACKKING ||
              board[position - 1] == BLACKKING ||
              board[position + 11] == BLACKKING ||
              board[position - 11] == BLACKKING ||
              board[position + 9] == BLACKKING ||
              board[position - 9] == BLACKKING ||
              board[position + 10] == BLACKKING ||
              board[position - 10] == BLACKKING) {
                return true;
          }

          return false;
     }

     else if (turn == BLACK) {
          //  1. pawn
          if (board[position + ROW - COLUMN] == WHITEPAWN ||
               board[position + ROW + COLUMN] == WHITEPAWN) {
               return true;
          }
          //  2. knight
          if (board[position - ROW - 2 * COLUMN] == WHITEKNIGHT ||
               board[position - ROW + 2 * COLUMN] == WHITEKNIGHT ||
               board[position + ROW - 2 * COLUMN] == WHITEKNIGHT ||
               board[position + ROW + 2 * COLUMN] == WHITEKNIGHT ||
               board[position - 2 * ROW - COLUMN] == WHITEKNIGHT ||
               board[position - 2 * ROW + COLUMN] == WHITEKNIGHT ||
               board[position + 2 * ROW - COLUMN] == WHITEKNIGHT ||
               board[position + 2 * ROW + COLUMN] == WHITEKNIGHT) {
               return true;
          }
          //  3. bishop
          for (int i = 1; i < 8; i++) {
               if (board[position - i*ROW - i*COLUMN] == WHITEBISHOP ||
                    board[position - i*ROW - i*COLUMN] == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position - i*ROW - i*COLUMN] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position - i*ROW - i*COLUMN] == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board[position - i*ROW + i*COLUMN] == WHITEBISHOP ||
                    board[position - i*ROW + i*COLUMN] == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position - i*ROW + i*COLUMN] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position - i*ROW + i*COLUMN] == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board[position + i*ROW - i*COLUMN] == WHITEBISHOP ||
                    board[position + i*ROW - i*COLUMN] == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position + i*ROW - i*COLUMN] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position + i*ROW - i*COLUMN] == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board[position + i*ROW + i*COLUMN] == WHITEBISHOP ||
                    board[position + i*ROW + i*COLUMN] == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position + i*ROW + i*COLUMN] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position + i*ROW + i*COLUMN] == ERRORSQUARE) {
                    break;
               }
          }
          //  4. rook
          for (int i = 1; i < 8; i++) {
               if (board[position - i*ROW] == WHITEROOK ||
                    board[position - i*ROW] == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position - i*ROW] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position - i*ROW] == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board[position + i*ROW] == WHITEROOK ||
                    board[position + i*ROW] == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position + i*ROW] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position + i*ROW] == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board[position - i*COLUMN] == WHITEROOK ||
                    board[position - i*COLUMN] == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position - i*COLUMN] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position - i*COLUMN] == ERRORSQUARE) {
                    break;
               }
          }
          for (int i = 1; i < 8; i++) {
               if (board[position + i*COLUMN] == WHITEROOK ||
                    board[position + i*COLUMN] == WHITEQUEEN) {
                    return true;
               }

               //  if some other piece blocks it, no more serach is necessary
               else if (board[position + i*COLUMN] != EMPTYSQUARE) {
                    break;
               }

               //  also when it reaches the end of the board
               else if (board[position + i*COLUMN] == ERRORSQUARE) {
                    break;
               }
          }

          //  (5. queen: added to bishop and rook)

          //  6. king: is it needed?
          if (board[position + 1] == WHITEKING ||
               board[position - 1] == WHITEKING ||
               board[position + 11] == WHITEKING ||
               board[position - 11] == WHITEKING ||
               board[position + 9] == WHITEKING ||
               board[position - 9] == WHITEKING ||
               board[position + 10] == WHITEKING ||
               board[position - 10] == WHITEKING) {
               return true;
          }
          return false;
     }

     else {
          throw std::invalid_argument("The board has invalid turn.");
     }

}

const bool Board::fiftyMoveCheck() const {
     return (halfMoveClock >= 100);
}

void Board::print_verbose() const {
     for (int i = 0; i < 120; i++) {
          if (i % 10 == 0) {
               printf("\n");
          }
          switch (board[i]) {
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
void Board::print() const {
     for (int i = 2; i < 10; i++) {
          printf("%d| ", 10 - i);
          for (int j = 1; j < 9; j++) {
               switch (board[i*ROW + j*COLUMN]) {
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

int Board::makeMove(const Move& move) {
     int capturedPiece = EMPTYSQUARE;
     int initial = move.getInitial(), terminal = move.getTerminal(), moveType = move.getType();

     if (moveType == NORMAL) {
          capturedPiece = board[terminal];
          board[terminal] = board[initial];
          board[initial] = EMPTYSQUARE;
     }
     else if (moveType == DOUBLEMOVE) {
          board[terminal] = board[initial];
          board[initial] = EMPTYSQUARE;
          enpassantSquare = (terminal + initial) / 2;
     }
     else if (moveType == QUEENSIDE_CASTLING) {
          //  move king
          board[terminal] = board[initial];
          board[initial] = EMPTYSQUARE;
          //  move rook
          board[terminal + COLUMN] = board[initial - 4 * COLUMN];
          board[initial - 4 * COLUMN] = EMPTYSQUARE;
          //  castling does not involve capture
     }
     else if (moveType == KINGSIDE_CASTLING) {
          //  move king
          board[terminal] = board[initial];
          board[initial] = EMPTYSQUARE;
          //  move rook
          board[terminal - COLUMN] = board[terminal + COLUMN];
          board[terminal + COLUMN] = EMPTYSQUARE;
          //  castling does not involve capture
     }
     else if (moveType == KNIGHT_PROMOTION) {
          capturedPiece = board[terminal];

          //  white turn
          if (board[initial] == WHITEPAWN) {
               board[terminal] = WHITEKNIGHT;
          }
          //  black turn
          else {
               board[terminal] = BLACKKNIGHT;
          }
          board[initial] = EMPTYSQUARE;
     }
     else if (moveType == BISHOP_PROMOTION) {
          capturedPiece = board[terminal];

          //  white turn
          if (board[initial] == WHITEPAWN) {
               board[terminal] =  WHITEBISHOP;
          }
          //  black turn
          else {
               board[terminal] = BLACKBISHOP;
          }
          board[initial] = EMPTYSQUARE;
     }
     else if (moveType == ROOK_PROMOTION) {
          capturedPiece = board[terminal];

          //  white turn
          if (board[initial] == WHITEPAWN) {
               board[terminal] = WHITEROOK;
          }
          //  black turn
          else {
               board[terminal] = BLACKROOK;
          }
          board[initial] = EMPTYSQUARE;
     }
     else if (moveType == QUEEN_PROMOTION) {
          capturedPiece = board[terminal];

          //  white turn
          if (board[initial] == WHITEPAWN) {
               board[terminal] = WHITEQUEEN;
          }
          //  black turn
          else {
               board[terminal] = BLACKQUEEN;
          }
          board[initial] = EMPTYSQUARE;
     }
     else if (moveType == ENPASSANT) {
          //  White turn
          if (board[initial] == WHITEPAWN) {
               board[terminal] = board[initial];
               board[initial] = EMPTYSQUARE;
               board[terminal + ROW] = EMPTYSQUARE;
               capturedPiece = BLACKPAWN;
          }
          //  Black turn
          else {
               board[terminal] = board[initial];
               board[initial] = EMPTYSQUARE;
               board[terminal - ROW] = EMPTYSQUARE;
               capturedPiece = WHITEPAWN;
          }
     }
     else {
          std::invalid_argument("makeMove() called with invalid moveType.");
     }

     updateBoard(move, capturedPiece);
     return capturedPiece;
}
void Board::undoMove(const Move& move, const int capturedPiece) {
     int initial = move.getInitial(), terminal = move.getTerminal(), moveType = move.getType();

     changeTurn();

     if (moveType == NORMAL) {
          board[initial] = board[terminal];
          board[terminal] = capturedPiece;
     }
     else if (moveType == DOUBLEMOVE) {
          board[initial] = board[terminal];
          board[terminal] = EMPTYSQUARE;
     }
     else if (moveType == QUEENSIDE_CASTLING) {
          //  undo king move
          board[initial] = board[terminal];
          board[terminal] = EMPTYSQUARE;

          //  undo rook move
          board[initial - 4 * COLUMN] = board[terminal + COLUMN];
          board[terminal + COLUMN] = EMPTYSQUARE;

     }
     else if (moveType == KINGSIDE_CASTLING) {
          //  undo king move
          board[initial] = board[terminal];
          board[terminal] = EMPTYSQUARE;

          //  undo rook move
          board[terminal + COLUMN] =board[terminal - COLUMN];
          board[terminal - COLUMN] = EMPTYSQUARE;
     }
     else if (moveType == KNIGHT_PROMOTION || moveType == BISHOP_PROMOTION ||
          moveType == ROOK_PROMOTION || moveType == QUEEN_PROMOTION) {
          //  white turn
          if (checkColor(board[terminal]) == WHITE) {
               board[terminal] = capturedPiece;
               board[initial] = WHITEPAWN;
          }
          //  black turn
          else {
               board[terminal] = capturedPiece;
               board[initial] = BLACKPAWN;
          }
     }
     else if (moveType == ENPASSANT) {
          //  white turn
          if (board[terminal] == WHITEPAWN) {
               board[terminal] = EMPTYSQUARE;
               board[initial] = WHITEPAWN;
               board[terminal + ROW] = BLACKPAWN;
          }
          //  black turn
          else {
               board[terminal] = EMPTYSQUARE;
               board[initial] = BLACKPAWN;
               board[terminal - ROW] = WHITEPAWN;
          }
     }
}

void Board::updateCastling(const Move& move) {
     if (board[move.getInitial()] == WHITEKING) {
          castlingRights[WKCASTLING] = false;
          castlingRights[WQCASTLING] = false;
     }
     if (board[move.getInitial()] == BLACKKING) {
          castlingRights[BKCASTLING] = false;
          castlingRights[BQCASTLING] = false;
     }
     if (board[move.getInitial()] == WHITEROOK) {
          if (move.getInitial() == A1) {
               castlingRights[WQCASTLING] = false;
          }
          if (move.getInitial() == H1) {
               castlingRights[WKCASTLING] = false;
          }
     }
     if (board[move.getInitial()] == BLACKROOK) {
          if (move.getInitial() == A8) {
               castlingRights[BQCASTLING] = false;
          }
          if (move.getInitial() == H8) {
               castlingRights[BKCASTLING] = false;
          }
     }
}
void Board::updateEnPassant(const Move& move) {
     if (move.getType() == DOUBLEMOVE) {
          enpassantSquare = (move.getInitial() + move.getTerminal()) / 2;
     }
     else { enpassantSquare = 0; }
}
void Board::updateHalfMoveClock(const Move& move) {
     if (board[move.getTerminal()] == EMPTYSQUARE
          && board[move.getInitial()] != WHITEPAWN
          && board[move.getInitial()] != BLACKPAWN) {
          halfMoveClock++;
     }
     else { halfMoveClock = 0; }
}
void Board::updateMoveNumber() {
     if (turn == BLACK) { moveNumber++; }
}

void Board::updateBoard(const Move& move, const int capturedPiece) {
     updateCastling(move);
     updateEnPassant(move);
     updateHalfMoveClock(move);
     updateEndgame(move);
     //board.updatePieceCount(move, capturedPiece);
     updateMoveNumber();
     changeTurn();
}

int checkColor(const int pieceType) {
     if (WHITEPAWN <= pieceType && pieceType <= WHITEKING) {
          return WHITE;
     }
     else if (BLACKPAWN <= pieceType && pieceType <= BLACKKING) {
          return BLACK;
     }
     else {
          return NEITHER;
     }
}

int filerankToNumber(const char file, const int rank) {
     //  if it is not a correct filerank format, return -1
     if ('a' > file || file > 'h' || 1 > rank || rank > 8) {
          return -1;
     }
     return COLUMN*(file - 'a' + 1) + ROW*(9 - (rank - 1));
}
