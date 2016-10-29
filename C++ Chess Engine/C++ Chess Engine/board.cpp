#include "board.h"
#include <string>
#include <iostream>

int boardEvalCounter = 0;

void boardEvalCalled() {
     boardEvalCounter++;
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
          board.setHalfMoveClock(10 * (FEN.at(i) - '0') + (FEN.at(i + 1) - '0'));
          i += 3;
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
          printf("Invalid pieceType\n");
          return 0;
     }
}

int filerankToNumber(const char file, const int rank) {
     //  if it is not a correct filerank format, return error
     if ('a' > file || file > 'h' || 1 > rank || rank > 8) {
          return ERRORCODE;
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
          // TODO: Better Exception Handling
          throw "Board given as argument to isSquareAttacked does not have valid turn.\n";
          return true;
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