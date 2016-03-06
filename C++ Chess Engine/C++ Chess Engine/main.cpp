
#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include <climits>
#include <array>
#include "protos.h"
#include "defs.h"

using namespace std;

/******************************************************************************/
/*                                 GLOBAL VARIABLE                            */
/******************************************************************************/
Board currentBoard;
bool gamePlaying = true;
bool endGame = false;
//  PCSQ Tables from 
//  https://chessprogramming.wikispaces.com/Simplified+evaluation+function
array<int, 120> PAWN_PCSQTable = {
     //  Possibly use -9999 for error squares for safety
     /*
     -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999,
     -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999,
     -9999,     0,     0,     0,     0,     0,     0,     0,     0, -9999,
     -9999,     0,     0,     0,     0,     0,     0,     0,     0, -9999,
         0,    50,    50,    50,    50,    50,    50,    50,    50,     0,*/
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0, 50, 50, 50, 50, 50, 50, 50, 50,  0,
      0, 10, 10, 20, 30, 30, 20, 10, 10,  0,
      0,  5,  5, 10, 25, 25, 10,  5,  5,  0,
      0,  0,  0,  0, 20, 20,  0,  0,  0,  0,
      0,  5, -5,-10,  0,  0,-10, -5,  5,  0,
      0,  5, 10, 10,-20,-20, 10, 10,  5,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
array<int, 120> KNIGHT_PCSQTable = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,-50,-40,-30,-30,-30,-30,-40,-50,  0,
     0,-40,-20,  0,  0,  0,  0,-20,-40,  0,
     0,-30,  0, 10, 15, 15, 10,  0,-30,  0,
     0,-30,  5, 15, 20, 20, 15,  5,-30,  0,
     0,-30,  0, 15, 20, 20, 15,  0,-30,  0,
     0,-30,  5, 10, 15, 15, 10,  5,-30,  0,
     0,-40,-20,  0,  5,  5,  0,-20,-40,  0,
     0,-50,-40,-30,-30,-30,-30,-40,-50,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
array<int, 120> BISHOP_PCSQTable = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,-20,-10,-10,-10,-10,-10,-10,-20,  0,
     0,-10,  0,  0,  0,  0,  0,  0,-10,  0,
     0,-10,  0,  5, 10, 10,  5,  0,-10,  0,
     0,-10,  5,  5, 10, 10,  5,  5,-10,  0,
     0,-10,  0, 10, 10, 10, 10,  0,-10,  0,
     0,-10, 10, 10, 10, 10, 10, 10,-10,  0,
     0,-10,  5,  0,  0,  0,  0,  5,-10,  0,
     0,-20,-10,-10,-10,-10,-10,-10,-20,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
array<int, 120> ROOK_PCSQTable = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  5, 10, 10, 10, 10, 10, 10,  5,  0,
     0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
     0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
     0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
     0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
     0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
     0,  0,  0,  0,  5,  5,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
array<int, 120> QUEEN_PCSQTable = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,-20,-10,-10, -5, -5,-10,-10,-20,  0,
     0,-10,  0,  0,  0,  0,  0,  0,-10,  0,
     0,-10,  0,  5,  5,  5,  5,  0,-10,  0,
     0, -5,  0,  5,  5,  5,  5,  0, -5,  0,
     0,  0,  0,  5,  5,  5,  5,  0, -5,  0,
     0,-10,  5,  5,  5,  5,  5,  0,-10,  0,
     0,-10,  0,  5,  0,  0,  0,  0,-10,  0,
     0,-20,-10,-10, -5, -5,-10,-10,-20,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
array<int, 120> KING_PCSQTable = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,-30,-40,-40,-50,-50,-40,-40,-30,  0,
     0,-30,-40,-40,-50,-50,-40,-40,-30,  0,
     0,-30,-40,-40,-50,-50,-40,-40,-30,  0,
     0,-30,-40,-40,-50,-50,-40,-40,-30,  0,
     0,-20,-30,-30,-40,-40,-30,-30,-20,  0,
     0,-10,-20,-20,-20,-20,-20,-20,-10,  0,
     0, 20, 20,  0,  0,  0,  0, 20, 20,  0,
     0, 20, 30, 10,  0,  0, 10, 30, 20,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};
array<int, 120> KING_PCSQTable_ENDGAME = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,-50,-40,-30,-20,-20,-30,-40,-50,  0,
     0,-30,-20,-10,  0,  0,-10,-20,-30,  0,
     0,-30,-10, 20, 30, 30, 20,-10,-30,  0,
     0,-30,-10, 30, 40, 40, 30,-10,-30,  0,
     0,-30,-10, 30, 40, 40, 30,-10,-30,  0,
     0,-30,-10, 20, 30, 30, 20,-10,-30,  0,
     0,-30,-30,  0,  0,  0,  0,-30,-30,  0,
     0,-50,-30,-30,-30,-30,-30,-30,-50,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};
//  Current Half Move Number, starts at 0
int halfMoveCount = 0;
//  move[3]: initial, terminal, moveType
int currentBoardMoveList[MAX_MOVEGEN_COUNT][3];
int currentBoardMoveCount;
int currentBoardLegalMoveList[MAX_MOVEGEN_COUNT][3];
int currentBoardLegalMoveCount;
int depthAllMoveList[MAXIMUM_DEPTH + 1][MAX_MOVEGEN_COUNT][3];
int depthAllMoveCount[MAXIMUM_DEPTH + 1];
int depthLegalMoveList[MAXIMUM_DEPTH + 1][MAX_MOVEGEN_COUNT][3];
int depthLegalMoveCount[MAXIMUM_DEPTH + 1];
//  added for time performance check
LARGE_INTEGER frequency, beginTime, endTime;
//  Records the result of the game
int gameResult = NOT_FINISHED;
//  Stores Board and Board States for threefold repetition
Board savedBoard[MAX_MOVENUMBER + 1];
//  Number of times the savedBoard state has occured
int repetitionCount[MAX_MOVENUMBER + 1];
//  Saved values for UNDO_MOVE command
int savedTerminalValue[MAX_MOVENUMBER]; // TODO: Check if it should be initialized as ERROR_INTEGER
int savedMove[MAX_MOVENUMBER + 1][3];

//  Which color user plays
int userColor = ERROR_INTEGER;
//  To create a log of moves
ofstream logtext;
//  if true, the game is between two computers
bool spectate = false;

/******************************************************************************/
/*                                  FUNCTIONS                                 */
/******************************************************************************/

/*                             BOARD SETUP FUNCTIONS                          */
void board120Setup() {
     currentBoard.setTurn(WHITE);
     currentBoard.setEnpassantSquare(0);
     currentBoard.setMoveNumber(1);
     currentBoard.setFiftyMoveCount(0);

     currentBoard.setCastlingArray({ true, true, true, true });
     
     //  Add Empty Squares
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               currentBoard.setSquare((i + 2) * 10 + j + 1, EMPTYSQUARE);
          }
     }
     
     //  Add Error Squares
     for (int j = 0; j < 10; j++) {
          currentBoard.setSquare(j, ERRORSQUARE);
          currentBoard.setSquare(10 + j, ERRORSQUARE);
          currentBoard.setSquare(100 + j, ERRORSQUARE);
          currentBoard.setSquare(110 + j, ERRORSQUARE);
     }
     for (int j = 0; j < 12; j++) {
          currentBoard.setSquare(10*j, ERRORSQUARE);
          currentBoard.setSquare(10*j + 9, ERRORSQUARE);
     }
     
     //  Add Non-Pawn Pieces
     currentBoard.setSquare(A8, BLACKROOK);
     currentBoard.setSquare(B8, BLACKKNIGHT);
     currentBoard.setSquare(C8, BLACKBISHOP);
     currentBoard.setSquare(D8, BLACKQUEEN);
     currentBoard.setSquare(E8, BLACKKING);
     currentBoard.setSquare(F8, BLACKBISHOP);
     currentBoard.setSquare(G8, BLACKKNIGHT);
     currentBoard.setSquare(H8, BLACKROOK);

     currentBoard.setSquare(A1, WHITEROOK);
     currentBoard.setSquare(B1, WHITEKNIGHT);
     currentBoard.setSquare(C1, WHITEBISHOP);
     currentBoard.setSquare(D1, WHITEQUEEN);
     currentBoard.setSquare(E1, WHITEKING);
     currentBoard.setSquare(F1, WHITEBISHOP);
     currentBoard.setSquare(G1, WHITEKNIGHT);
     currentBoard.setSquare(H1, WHITEROOK);

     //  Add Pawn Pieces
     for (int i = 0; i < 8; i++) {
          currentBoard.setSquare(A2 + i, WHITEPAWN);
          currentBoard.setSquare(A7 + i, BLACKPAWN);
     }
}
void FENboardSetup(const std::string FEN) {
     currentBoard.setCastlingArray({ false, false, false, false });
     currentBoard.setEnpassantSquare(0);

     //  Add Error Squares
     for (int j = 0; j < 10; j++) {
          currentBoard.setSquare(j, ERRORSQUARE);
          currentBoard.setSquare(10 + j, ERRORSQUARE);
          currentBoard.setSquare(100 + j, ERRORSQUARE);
          currentBoard.setSquare(110 + j, ERRORSQUARE);
     }
     for (int j = 0; j < 12; j++) {
          currentBoard.setSquare(10 * j, ERRORSQUARE);
          currentBoard.setSquare(10 * j + 9, ERRORSQUARE);
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
                    currentBoard.setSquare(currentSquare, EMPTYSQUARE);
                    currentSquare++;
               }
          }
          else {
               switch (FEN.at(i)) {
               case 'p':
                    currentBoard.setSquare(currentSquare, BLACKPAWN);
                    break;
               case 'r':
                    currentBoard.setSquare(currentSquare, BLACKROOK);
                    break;
               case 'n':
                    currentBoard.setSquare(currentSquare, BLACKKNIGHT);
                    break;
               case 'b':
                    currentBoard.setSquare(currentSquare, BLACKBISHOP);
                    break;
               case 'q':
                    currentBoard.setSquare(currentSquare, BLACKQUEEN);
                    break;
               case 'k':
                    currentBoard.setSquare(currentSquare, BLACKKING);
                    break;
               case 'P':
                    currentBoard.setSquare(currentSquare, WHITEPAWN);
                    break;
               case 'R':
                    currentBoard.setSquare(currentSquare, WHITEROOK);
                    break;
               case 'N':
                    currentBoard.setSquare(currentSquare, WHITEKNIGHT);
                    break;
               case 'B':
                    currentBoard.setSquare(currentSquare, WHITEBISHOP);
                    break;
               case 'Q':
                    currentBoard.setSquare(currentSquare, WHITEQUEEN);
                    break;
               case 'K':
                    currentBoard.setSquare(currentSquare, WHITEKING);
                    break;
               }
               currentSquare++;
          }
     }
     i++;
     if (FEN.at(i) == 'w') { currentBoard.setTurn(WHITE); }
     else { currentBoard.setTurn(WHITE); }

     i += 2;
     if (FEN.at(i) != '-') {
          while (FEN.at(i) != ' ') {
               if (FEN.at(i) == 'K') {
                    currentBoard.setCastling(WKCASTLING, true);
               }
               if (FEN.at(i) == 'Q') {
                    currentBoard.setCastling(WQCASTLING, true);
               }
               if (FEN.at(i) == 'k') {
                    currentBoard.setCastling(BKCASTLING, true);
               }
               if (FEN.at(i) == 'q') {
                    currentBoard.setCastling(BQCASTLING, true);
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

          currentBoard.setEnpassantSquare(enpassantSquare);
     }

     i += 2;
     currentBoard.setFiftyMoveCount(FEN.at(i) - '0');

     i += 2;
     currentBoard.setMoveNumber(FEN.at(i) - '0');


}
string boardToFEN(const Board& board) {
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
     bool WKCastling = board.getCastling(WKCASTLING);
     bool WQCastling = board.getCastling(WQCASTLING);
     bool BKCastling = board.getCastling(BKCASTLING);
     bool BQCastling = board.getCastling(BQCASTLING);
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
     FEN += ('0' + board.getFiftyMoveCount());
     FEN += ' ';
     FEN += to_string(board.getMoveNumber());
     
     cout << FEN << endl;
     return FEN;

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
               switch (board.getSquare(i*ROW+j*COLUMN)) {
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
int checkColor(int pieceType) {
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
          printf("checkColor unreachable error\n");
          return 0;
     }
}
char numberToFile(int position) {
     char file = 'a' + position % 10 - 1;
     return file;
}    
int numberToRank(int position) {
     int rank = 10 - position / 10;
     return rank;
}
int filerankToNumber(char file, int rank) {
     //  if it is not a correct filerank format, return error
     if ('a' > file || file > 'h' || '0' > rank || rank > '8' ) {
          return ERROR_INTEGER;
     }

     int position120;
     position120 = COLUMN*(file - 'a' + 1) + ROW*(9 - (rank - '1'));
     return position120;
}
string numberToFilerank(int position) {
     string fileRank = "";
     char file = 'a' + position % 10 - 1; 
     int rank = 10 - position / 10;
     fileRank += file;
     fileRank += std::to_string(rank);
     return fileRank;
}
void printMove(int move[3]) {
     cout << numberToFilerank(move[0]) << " " << numberToFilerank(move[1]) << " (" << move[2] << ")" << endl;
}

/*                             EVALUATION FUNCTIONS                           */
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
               if (endGame) {
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
               if (endGame) {
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
int reversePosition(int position) {
     int row = position / 10, column = position % 10;
     return (12 - row - 1) * 10 + column;
}

int negaMax(int depth, Board& board) {
     if (depth == 0) {
          return board.getTurn() * boardEvaluation(board);
     }
     int max_Score = INT_MIN;
     int score;
     int terminalValue;

     moveGeneration(board, depthAllMoveList[depth], &depthAllMoveCount[depth]);
     legalMoves(board, depthAllMoveList[depth], depthAllMoveCount[depth], depthLegalMoveList[depth], &depthLegalMoveCount[depth]);

     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {

          if (board.getSquare(depthLegalMoveList[depth][i][0]) == WHITEKING) {
               board.setCastling(WKCASTLING, false);
               board.setCastling(WQCASTLING, false);
          }
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == BLACKKING) {
               board.setCastling(BKCASTLING, false);
               board.setCastling(BQCASTLING, false);
          }
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == WHITEROOK) {
               if (depthLegalMoveList[depth][i][0] == A1) {
                    board.setCastling(WQCASTLING, false);
               }
               if (depthLegalMoveList[depth][i][0] == H1) {
                    board.setCastling(WKCASTLING, false);
               }
          }
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == BLACKROOK) {
               if (depthLegalMoveList[depth][i][0] == A8) {
                    board.setCastling(BQCASTLING, false);
               }
               if (depthLegalMoveList[depth][i][0] == H8) {
                    board.setCastling(BKCASTLING, false);
               }
          }
          int enpassantSquare = board.getEnpassantSquare();

          terminalValue = makeMove(board, depthLegalMoveList[depth][i]);

          score = -negaMax(depth-1, board);

          if (score > max_Score) {
               max_Score = score;
          }

          undoMove(board, depthLegalMoveList[depth][i], terminalValue);
          board.setEnpassantSquare(enpassantSquare);
     }

     return max_Score;
}
int rootNegaMax(int maxDepth, Board& board, int bestMove[3]) {

     int max_Score = INT_MIN;
     int score;
     int terminalValue;

     moveGeneration(board, depthAllMoveList[maxDepth], &depthAllMoveCount[maxDepth]);
     legalMoves(board, depthAllMoveList[maxDepth], depthAllMoveCount[maxDepth], depthLegalMoveList[maxDepth], &depthLegalMoveCount[maxDepth]);

     for (int i = 0; i < depthLegalMoveCount[maxDepth]; i++) {

          if (board.getSquare(depthLegalMoveList[maxDepth][i][0]) == WHITEKING) {
               board.setCastling(WKCASTLING, false);
               board.setCastling(WQCASTLING, false);
          }
          if (board.getSquare(depthLegalMoveList[maxDepth][i][0]) == BLACKKING) {
               board.setCastling(BKCASTLING, false);
               board.setCastling(BQCASTLING, false);
          }
          if (board.getSquare(depthLegalMoveList[maxDepth][i][0]) == WHITEROOK) {
               if (depthLegalMoveList[maxDepth][i][0] == A1) {
                    board.setCastling(WQCASTLING, false);
               }
               if (depthLegalMoveList[maxDepth][i][0] == H1) {
                    board.setCastling(WKCASTLING, false);
               }
          }
          if (board.getSquare(depthLegalMoveList[maxDepth][i][0]) == BLACKROOK) {
               if (depthLegalMoveList[maxDepth][i][0] == A8) {
                    board.setCastling(BQCASTLING, false);
               }
               if (depthLegalMoveList[maxDepth][i][0] == H8) {
                    board.setCastling(BKCASTLING, false);
               }
          }

          int enpassantSquare = board.getEnpassantSquare();
          terminalValue = makeMove(board, depthLegalMoveList[maxDepth][i]);

          score = -negaMax(maxDepth - 1, board);

          if (score > max_Score) {
               max_Score = score;
               bestMove[0] = depthLegalMoveList[maxDepth][i][0];
               bestMove[1] = depthLegalMoveList[maxDepth][i][1];
               bestMove[2] = depthLegalMoveList[maxDepth][i][2];

          }

          undoMove(board, depthLegalMoveList[maxDepth][i], terminalValue);
          board.setEnpassantSquare(enpassantSquare);
     }

     return max_Score;
}

int alphabeta(int depth, Board& board, int alpha, int beta) {
     if (depth == 0) {
          return board.getTurn() * boardEvaluation(board);
     }

     int score;
     int terminalValue;

     moveGeneration(board, depthAllMoveList[depth], &depthAllMoveCount[depth]);
     legalMoves(board, depthAllMoveList[depth], depthAllMoveCount[depth], depthLegalMoveList[depth], &depthLegalMoveCount[depth]);

     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {

          if (board.getSquare(depthLegalMoveList[depth][i][0]) == WHITEKING) {
               board.setCastling(WKCASTLING, false);
               board.setCastling(WQCASTLING, false);
          }
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == BLACKKING) {
               board.setCastling(BKCASTLING, false);
               board.setCastling(BQCASTLING, false);
          }
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == WHITEROOK) {
               if (depthLegalMoveList[depth][i][0] == A1) {
                    board.setCastling(WQCASTLING, false);
               }
               if (depthLegalMoveList[depth][i][0] == H1) {
                    board.setCastling(WKCASTLING, false);
               }
          }
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == BLACKROOK) {
               if (depthLegalMoveList[depth][i][0] == A8) {
                    board.setCastling(BQCASTLING, false);
               }
               if (depthLegalMoveList[depth][i][0] == H8) {
                    board.setCastling(BKCASTLING, false);
               }
          }
          // Save enpassantSquare so it doesn't get lost while making move
          int enpassantSquare = board.getEnpassantSquare();
         
          terminalValue = makeMove(board, depthLegalMoveList[depth][i]);

          score = -alphabeta(depth - 1, board, -beta, -alpha);

          if (score >= beta) {
               undoMove(board, depthLegalMoveList[depth][i], terminalValue);
               board.setEnpassantSquare(enpassantSquare);
               return beta;
          }
          
          if (score > alpha) {
               alpha = score;
          }
          undoMove(board, depthLegalMoveList[depth][i], terminalValue);
          board.setEnpassantSquare(enpassantSquare);
     }

     return alpha;
}
int rootAlphabeta(int maxDepth, Board board, int alpha, int beta, int bestMove[3]) {
     int score;
     int terminalValue;

     moveGeneration(board, depthAllMoveList[maxDepth], &depthAllMoveCount[maxDepth]);
     legalMoves(board, depthAllMoveList[maxDepth], depthAllMoveCount[maxDepth], depthLegalMoveList[maxDepth], &depthLegalMoveCount[maxDepth]);

     for (int i = 0; i < depthLegalMoveCount[maxDepth]; i++) {

          if (board.getSquare(depthLegalMoveList[maxDepth][i][0]) == WHITEKING) {
               board.setCastling(WKCASTLING, false);
               board.setCastling(WQCASTLING, false);
          }
          if (board.getSquare(depthLegalMoveList[maxDepth][i][0]) == BLACKKING) {
               board.setCastling(BKCASTLING, false);
               board.setCastling(BQCASTLING, false);
          }
          if (board.getSquare(depthLegalMoveList[maxDepth][i][0]) == WHITEROOK) {
               if (depthLegalMoveList[maxDepth][i][0] == A1) {
                    board.setCastling(WQCASTLING, false);
               }
               if (depthLegalMoveList[maxDepth][i][0] == H1) {
                    board.setCastling(WKCASTLING, false);
               }
          }
          if (board.getSquare(depthLegalMoveList[maxDepth][i][0]) == BLACKROOK) {
               if (depthLegalMoveList[maxDepth][i][0] == A8) {
                    board.setCastling(BQCASTLING, false);
               }
               if (depthLegalMoveList[maxDepth][i][0] == H8) {
                    board.setCastling(BKCASTLING, false);
               }
          }
          int enpassantSquare = board.getEnpassantSquare();
          terminalValue = makeMove(board, depthLegalMoveList[maxDepth][i]);

          score = -alphabeta(maxDepth - 1, board, -beta, -alpha);

          // TODO: Check if this is needed and change it
          if (score >= beta) {

               undoMove(board, depthLegalMoveList[maxDepth][i], terminalValue);
               // TODO: Make sure Castling & EP Square & other details are also undo-ed
               board.setEnpassantSquare(enpassantSquare);
               return beta;
          }

          if (score > alpha) {
               alpha = score;
               bestMove[0] = depthLegalMoveList[maxDepth][i][0];
               bestMove[1] = depthLegalMoveList[maxDepth][i][1];
               bestMove[2] = depthLegalMoveList[maxDepth][i][2];
          }

          undoMove(board, depthLegalMoveList[maxDepth][i], terminalValue);
          board.setEnpassantSquare(enpassantSquare);
     }

     return alpha;
}

/*                             GAME CYCLE FUNCTIONS                           */
bool checkGameEnd(const Board& board) {
     bool whiteKing = false, blackKing = false;
     for (int i = 0; i < 120; i++) {
          if (board.getSquare(i) == WHITEKING) { whiteKing = true; }
          if (board.getSquare(i) == BLACKKING) { blackKing = true; }
     }
     return !(whiteKing && blackKing);
}

/*                           MOVE GENERATION FUNCTIONS                        */
void moveGeneration(Board& board, int moveList[250][3], int *moveCount) {
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
                    pawnMoveGeneration(board,i, moveList, moveCount);
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
void pawnMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount) {
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
                    board.getSquare(position - 2*ROW) == EMPTYSQUARE) {
                    addMove(position, position - 2*ROW, DOUBLEMOVE, moveList, moveCount);
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
void knightMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount) {
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
void bishopMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount) {
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
void rookMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount) {
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
void queenMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount) {
     rookMoveGeneration(board, position, moveList, moveCount);
     bishopMoveGeneration(board, position, moveList, moveCount);
}
void kingMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount) {
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

void castlingMoveGeneration(Board& board, int moveList[250][3], int *moveCount) {
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
void promotionMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount) {
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
void enpassantMoveGeneration(Board& board, int moveList[250][3], int *moveCount) {
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

void addMove(int initial, int terminal, int moveType, int moveList[250][3], int *moveCount) {
     moveList[*moveCount][0] = initial;
     moveList[*moveCount][1] = terminal;
     moveList[*moveCount][2] = moveType;
     *moveCount += 1;
}
void addPromotionMove(int initial, int terminal, int moveList[250][3], int *moveCount) {
     addMove(initial, terminal, KNIGHT_PROMOTION, moveList, moveCount);
     addMove(initial, terminal, BISHOP_PROMOTION, moveList, moveCount);
     addMove(initial, terminal, ROOK_PROMOTION, moveList, moveCount);
     addMove(initial, terminal, QUEEN_PROMOTION, moveList, moveCount);
}

void legalMoves(Board board, int moveList[250][3], int moveCount, int legalMoveList[250][3], int *legalMoveCount) {
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
          if (board.getSquare(moveList[i][0]) == WHITEKING || board.getSquare(moveList[i][0]) == BLACKKING) {
               if (moveList[i][2] == NORMAL) {
                    changedKingPosition = moveList[i][1];
               }
               if (moveList[i][2] == KINGSIDE_CASTLING) {
                    changedKingPosition = moveList[i][0] + 2 * COLUMN;
               }
               if (moveList[i][2] == QUEENSIDE_CASTLING) {
                    changedKingPosition = moveList[i][0] - 2 * COLUMN;
               }
               
          }
          else { changedKingPosition = kingPosition; }

          //  make move
          terminalValue = makeMove(board, moveList[i]);
          //  In this case, we don't want makeMove to change turn, so let's change it again
          board.changeTurn();

          //  if king is safe
          if (!squareAttackCheck(board, changedKingPosition)) {
               legalMoveList[*legalMoveCount][0] = moveList[i][0];
               legalMoveList[*legalMoveCount][1] = moveList[i][1];
               legalMoveList[*legalMoveCount][2] = moveList[i][2];
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


/*                             RECURSION FUNCTIONS                             */
u64 divide(int depth, int maxDepth, Board& board, bool showOutput) {

     if (depth == 0) { return 1; }

     depthAllMoveCount[depth] = 0;
     depthLegalMoveCount[depth] = 0;

     u64 node = 0, individualNode = 0;
     int terminalValue;

     // MOVEGEN
     moveGeneration(board, depthAllMoveList[depth], &depthAllMoveCount[depth]);
     // CHECK FOR LEGALS
     legalMoves(board, depthAllMoveList[depth], depthAllMoveCount[depth], depthLegalMoveList[depth], &depthLegalMoveCount[depth]);

     if (depth == 1) { return depthLegalMoveCount[depth]; }

     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {

          if (board.getSquare(depthLegalMoveList[depth][i][0]) == WHITEKING) {
               board.setCastling(WKCASTLING, false);
               board.setCastling(WQCASTLING, false);
          }
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == BLACKKING) {
               board.setCastling(BKCASTLING, false);
               board.setCastling(BQCASTLING, false);
          }
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == WHITEROOK) {
               if (depthLegalMoveList[depth][i][0] == A1) {
                    board.setCastling(WQCASTLING, false);
               }
               if (depthLegalMoveList[depth][i][0] == H1) {
                    board.setCastling(WKCASTLING, false);
               }
          }
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == BLACKROOK) {
               if (depthLegalMoveList[depth][i][0] == A8) {
                    board.setCastling(BQCASTLING, false);
               }
               if (depthLegalMoveList[depth][i][0] == H8) {
                    board.setCastling(BKCASTLING, false);
               }
          }
          int enpassantSquare = board.getEnpassantSquare();
          
          terminalValue = makeMove(board, depthLegalMoveList[depth][i]);
          
          node += divide(depth - 1, maxDepth, board, showOutput);
          if (showOutput) {
               individualNode = divide(depth - 1, maxDepth, board, false);
          }
          
          if (depth >= maxDepth && showOutput) {
               for (int i = 0; i < 3-depth; i++) { printf("  "); }
               printf("%c%d%c%d: %llu", numberToFile(depthLegalMoveList[depth][i][0]), numberToRank(depthLegalMoveList[depth][i][0]),
                    numberToFile(depthLegalMoveList[depth][i][1]), numberToRank(depthLegalMoveList[depth][i][1]), individualNode);
               printf("\n");
          }

          undoMove(board, depthLegalMoveList[depth][i], terminalValue);
          board.setEnpassantSquare(enpassantSquare);
     }
     return node;

}
u64 divide2(int depth, int maxDepth, Board& board, bool showOutput) {

     if (depth == 0) { return 1; }

     //  output text file for large output
     ofstream output2;
     output2.open("divide.txt");

     depthAllMoveCount[depth] = 0;
     depthLegalMoveCount[depth] = 0;

     u64 node = 0, individualNode = 0;
     int terminalValue;

     // MOVEGEN
     moveGeneration(board, depthAllMoveList[depth], &depthAllMoveCount[depth]);
     // CHECK FOR LEGALS
     legalMoves(board, depthAllMoveList[depth], depthAllMoveCount[depth], depthLegalMoveList[depth], &depthLegalMoveCount[depth]);

     //if (depth == 1) { return depthLegalMoveCount[depth]; }

     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == WHITEKING) {
               board.setCastling(WKCASTLING, false);
               board.setCastling(WQCASTLING, false);
          }
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == BLACKKING) {
               board.setCastling(BKCASTLING, false);
               board.setCastling(BQCASTLING, false);
          }
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == WHITEROOK) {
               if (depthLegalMoveList[depth][i][0] == A1) {
                    board.setCastling(WQCASTLING, false);
               }
               if (depthLegalMoveList[depth][i][0] == H1) {
                    board.setCastling(WKCASTLING, false);
               }
          }
          if (board.getSquare(depthLegalMoveList[depth][i][0]) == BLACKROOK) {
               if (depthLegalMoveList[depth][i][0] == A8) {
                    board.setCastling(BQCASTLING, false);
               }
               if (depthLegalMoveList[depth][i][0] == H8) {
                    board.setCastling(BKCASTLING, false);
               }
          }
          int enpassantSquare = board.getEnpassantSquare();
          terminalValue = makeMove(currentBoard, depthLegalMoveList[depth][i]);


          node += divide(depth - 1, maxDepth, board, showOutput);
          if (showOutput) {
               individualNode = divide(depth - 1, maxDepth, board, false);
          }

          if (depth >= maxDepth && showOutput) {
               output2 << numberToFile(depthLegalMoveList[depth][i][0]) << numberToRank(depthLegalMoveList[depth][i][0]) <<
                    numberToFile(depthLegalMoveList[depth][i][1]) << numberToRank(depthLegalMoveList[depth][i][1]) << ": " << individualNode << std::endl;
          }

          undoMove(board, depthLegalMoveList[depth][i], terminalValue);
          board.setEnpassantSquare(enpassantSquare);
     }
     return node;
     output2.close();
}

int makeMove(Board &board, int move[3]) {
     int terminalValue;
     int initial = move[0], terminal = move[1], moveType = move[2];

     board.setEnpassantSquare(0);
     board.changeTurn();

     if (moveType == NORMAL) {
          terminalValue = board.getSquare(terminal);
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          return terminalValue;
     }
     if (moveType == DOUBLEMOVE) {
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          board.setEnpassantSquare((terminal + initial) / 2);
          //  terminalValue is actually enpassantSquare
          return (terminal+initial)/2;
     }
     else if (moveType == QUEENSIDE_CASTLING) {
          //  move king
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          //  move rook
          board.setSquare(terminal + COLUMN, board.getSquare(initial - 4 * COLUMN));
          board.setSquare(initial - 4 * COLUMN, EMPTYSQUARE);
          //  castling does not involve capture
          return 0;
     }
     else if (moveType == KINGSIDE_CASTLING) {
          //  move king
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          //  move rook
          board.setSquare(terminal - COLUMN, board.getSquare(terminal + COLUMN));
          board.setSquare(terminal + COLUMN, EMPTYSQUARE);
          //  castling does not involve capture
          return 0;
     }
     else if (moveType == KNIGHT_PROMOTION) {
          terminalValue = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEKNIGHT);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKKNIGHT);
          }
          board.setSquare(initial, EMPTYSQUARE);
          return terminalValue;
     }
     else if (moveType == BISHOP_PROMOTION) {
          terminalValue = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEBISHOP);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKBISHOP);
          }
          board.setSquare(initial, EMPTYSQUARE);
          return terminalValue;
     }
     else if (moveType == ROOK_PROMOTION) {
          terminalValue = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEROOK);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKROOK);
          }
          board.setSquare(initial, EMPTYSQUARE);
          return terminalValue;
     }
     else if (moveType == QUEEN_PROMOTION) {
          terminalValue = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEQUEEN);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKQUEEN);
          }
          board.setSquare(initial, EMPTYSQUARE);
          return terminalValue;
     }
     else if (moveType == ENPASSANT) {
          //  White turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, board.getSquare(initial));
               board.setSquare(initial, EMPTYSQUARE);
               board.setSquare(terminal + ROW, EMPTYSQUARE);
               return BLACKPAWN;
          }
          //  Black turn
          else {
               board.setSquare(terminal, board.getSquare(initial));
               board.setSquare(initial, EMPTYSQUARE);
               board.setSquare(terminal - ROW, EMPTYSQUARE);
               return WHITEPAWN;
          }
     }
     else {
          printf("makeMove unreachable error\n");
          return 0;
     }
}
void undoMove(Board &board, int move[3], int terminalValue) {
     int initial = move[0], terminal = move[1], moveType = move[2];

     board.changeTurn();
     if (moveType == NORMAL) {
          board.setSquare(initial, board.getSquare(terminal));
          board.setSquare(terminal, terminalValue);
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
               board.setSquare(terminal, terminalValue);
               board.setSquare(initial, WHITEPAWN);
          }
          //  black turn
          else {
               board.setSquare(terminal, terminalValue);
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

LARGE_INTEGER startTimer(LARGE_INTEGER *beginTime, int timerIndex) {
     LARGE_INTEGER frequency;  // ticks per second

     printf("Timer %d starting!\n", timerIndex);

     // get ticks per second
     QueryPerformanceFrequency(&frequency);

     // start timer
     QueryPerformanceCounter(beginTime);

     return frequency;
}
void stopTimer(LARGE_INTEGER *endTime, int timerIndex) {
     QueryPerformanceCounter(endTime);
     printf("Timer %d ended!\n", timerIndex);
}
void printElapsedTime(LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex) {
     // in millisecond
     double elapsedTime = (endTime.QuadPart - beginTime.QuadPart) * 1000.0 / frequency.QuadPart;
     std::cout << "Timer " << timerIndex << ": " << elapsedTime << " ms elapsed." << std::endl;
}
double elapsedTime (LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex) {
     // in millisecond
     return (endTime.QuadPart - beginTime.QuadPart) * 1000.0 / frequency.QuadPart;
}

void castlingUpdate(Board& board, const Move& move) {
     if (board.getSquare(move.getInitial()) == WHITEKING) {
          board.setCastling(WKCASTLING, false);
          board.setCastling(WQCASTLING, false);
     }
     if (board.getSquare(move.getInitial()) == BLACKKING) {
          board.setCastling(BKCASTLING, false);
          board.setCastling(BQCASTLING, false);
     }
     if (board.getSquare(move.getInitial()) == WHITEROOK) {
          if (move.getInitial() == A1) {
               board.setCastling(WQCASTLING, false);
          }
          if (move.getInitial() == H1) {
               board.setCastling(WKCASTLING, false);
          }
     }
     if (board.getSquare(move.getInitial()) == BLACKROOK) {
          if (move.getInitial() == A8) {
               board.setCastling(BQCASTLING, false);
          }
          if (move.getInitial() == H8) {
               board.setCastling(BKCASTLING, false);
          }
     }
}
int isTerminalNode(Board& board) {
     int tempBoardMoveList[MAX_MOVEGEN_COUNT][3];
     int tempBoardMoveCount;
     int tempBoardLegalMoveList[MAX_MOVEGEN_COUNT][3];
     int tempBoardLegalMoveCount;
     
     moveGeneration(board, tempBoardMoveList, &tempBoardMoveCount);
     legalMoves(board, tempBoardMoveList, tempBoardMoveCount, tempBoardLegalMoveList, &tempBoardLegalMoveCount);

     // Checkmate
     
     // Stalemate: No legal move
     

     // Stalemate: Fifty Move Rule
     
     // Stalemate: 75 Move Rule
     
     // Stalemate: Threefold Repetition

     return -1;
}

/******************************************************************************/
/*                               MAIN FUNCTION                                */
/******************************************************************************/
void main() {

     logtext.open("log.txt");
     
     //  Initialize Board
     board120Setup();
 
     //  FEN source:
     //  https://chessprogramming.wikispaces.com/Perft+Results
     //  - Position 1: Perft 6 Correct
     //  - Position 2: Perft 5 Correct
     //  - Position 3: Perft 6 Correct, Perft 7 makeMove unreachable error, doesn't end
     //  - Position 4: Perft 5 Correct
     //  - Position 5: Perft 5 Correct
     //  - Position 6: Perft 5 Correct

     printSimpleBoard(currentBoard);
     printf("--------------------------------------------------\n");
     printf("Engine Search Depth: %d\n", EVAL_DEPTH);
     printf("Castling - WK:%d WQ:%d BK:%d BQ:%d\n", currentBoard.getCastling(WKCASTLING), currentBoard.getCastling(WQCASTLING), currentBoard.getCastling(BKCASTLING), currentBoard.getCastling(BQCASTLING));
     printf("en passant Square: %d\n", currentBoard.getEnpassantSquare());
     printf("Move number: %d\n", currentBoard.getMoveNumber());
     if (currentBoard.getTurn() == WHITE) { printf("Turn: White\n"); }
     else { printf("Turn: Black\n"); }
     boardToFEN(currentBoard);
     printf("--------------------------------------------------\n");

     //  begin timer
     frequency = startTimer(&beginTime, 1);
     

     //  Game Loop: Player vs COM
     bool correctInput = false;
     string userCommand;

     // Repetition count reset
     for (int i = 0; i < MAX_MOVENUMBER; i++) {
          repetitionCount[i] = 0;
     }

     while (gamePlaying) {
          //  Check endgame
          if (!endGame) {
               //  if no queens are on the board
               int queenCount = 0;
               for (int i = 0; i < 120; i++) {
                    if (currentBoard.getSquare(i) == WHITEQUEEN || currentBoard.getSquare(i) == BLACKQUEEN) {
                         queenCount++;
                    }
               }
               if (queenCount == 0) {
                    endGame = true;
               }
          }

          //  Detect Checkmate/Stalemate
          moveGeneration(currentBoard, currentBoardMoveList, &currentBoardMoveCount);
          legalMoves(currentBoard, currentBoardMoveList, currentBoardMoveCount, currentBoardLegalMoveList, &currentBoardLegalMoveCount);
          if (currentBoardLegalMoveCount == 0) {
               int kingPosition = ERROR_INTEGER;
               for (int i = 0; i < 120; i++) {
                    if (currentBoard.getSquare(i) == WHITEKING && currentBoard.getTurn() == WHITE ||
                         currentBoard.getSquare(i) == BLACKKING && currentBoard.getTurn() == BLACK) {
                         kingPosition = i;
                         break;
                    }
               }
               if (kingPosition == ERROR_INTEGER) {
                    printf("Something went wrong!\n");
                    break;
               }
               if (squareAttackCheck(currentBoard, kingPosition)) {
                    gamePlaying = false;
                    if (currentBoard.getTurn() == WHITE) {
                         gameResult = BLACK_WIN;
                    }
                    if (currentBoard.getTurn() == BLACK) {
                         gameResult = WHITE_WIN;
                    }
                    break;
               }
               else {
                    gamePlaying = false;
                    gameResult = TIE;
                    break;
               }
          }

          //  Let user determine color to play in first loop
          correctInput = false;
          while (!correctInput && userColor == ERROR_INTEGER) {
               printf("Which color would you like to play? (W, B or N): ");
               std::getline(cin, userCommand);
               if (userCommand.size() == 0) {
                    printf("You must enter W or B!\n");
                    continue;
               }
               if (userCommand.at(0) == 'W') {
                    userColor = WHITE;
                    correctInput = true;
                    logtext << "Player (White) vs. COM (Black)" << endl;
                    break;
               }
               else if (userCommand.at(0) == 'B') {
                    userColor = BLACK;
                    correctInput = true;
                    logtext << "COM (White) vs. Player (Black)" << endl;
                    break;
               }
               else if (userCommand.at(0) == 'N') {
                    spectate = true;
                    correctInput = true;
                    userColor = NEITHER;
                    logtext << "COM (White) vs. COM (Black)" << endl;
                    break;
               }
               else {
                    printf("Wrong input! Write W, B or N.\n");
                    continue;
               }
          }

          logtext << "COM Search Depth: " << EVAL_DEPTH << endl;

          if (currentBoard.getTurn() == userColor && spectate == false) {
               
               int initialSquare, terminalSquare;
               int commandType = ERROR_COMMAND;
               int commandType2 = ERROR_COMMAND;
               
               correctInput = false;
               while (!correctInput) {
                    printf("--------------------------------------------------\n");
                    printf("%d: Make move\n", MOVE);
                    printf("%d: Display Board\n", DISPLAY_BOARD);
                    printf("%d: Display FEN\n", DISPLAY_FEN);
                    printf("%d: Reset Board\n", BOARD_RESET);
                    printf("%d: Perft Test\n", PERFT);
                    printf("%d: Quit\n", QUIT);
                    printf("%d: Divide Perft Test\n", DIVIDE);
                    printf("%d: Undo move\n", UNDO_MOVE);
                    printf("%d: Computer Make Move\n", COM_MAKE_MOVE);
                    printf("A: Print Possible Moves\n");
                    printf("B: Negamax vs. Negamax + Alphabeta Pruning\n");
                    printf("C: Evaluate Board\n");
                    printf("D: Alphabeta Speed Check\n");
                    printf("E: Print Saved FEN\n");
                    printf("--------------------------------------------------\n");
                    printf("Please choose command: ");
                    std::getline(cin, userCommand);

                    if (userCommand.size() == 0) {
                         printf("You must enter a number!\n");
                         continue;
                    }
                    if ('A' <= userCommand.at(0) && userCommand.at(0) <= 'E') {
                         commandType = userCommand.at(0) - 'A' + 10;
                         correctInput = true;
                         break;
                    }
                    commandType = userCommand.at(0) - '0';
                    if (1 <= commandType && commandType <= 9) {
                         correctInput = true;
                         break;
                    }
               }
               
               if (commandType == MOVE) {
                    
                    savedBoard[halfMoveCount] = currentBoard;

                    //  Movelist used for legality/movetype check
                    currentBoardMoveCount = 0;
                    currentBoardLegalMoveCount = 0;
                    moveGeneration(currentBoard, currentBoardMoveList, &currentBoardMoveCount);
                    legalMoves(currentBoard, currentBoardMoveList, currentBoardMoveCount, currentBoardLegalMoveList, &currentBoardLegalMoveCount);

                    int moveType = NORMAL;

                    correctInput = false;
                    while (!correctInput) {
                         printf("Please enter your move: ");
                         std::getline(cin, userCommand); // TODO: do I want to get the entire command?

                         //  Check size
                         if (userCommand.size() < 4) {
                              printf("Wrong format: correct format is [a-h][1-8][a-h][1-8].\n");
                              continue;
                         }

                         initialSquare = filerankToNumber(userCommand.at(0), userCommand.at(1));
                         terminalSquare = filerankToNumber(userCommand.at(2), userCommand.at(3));

                         //  Check if Filerank format is correct
                         if (initialSquare == ERROR_INTEGER || terminalSquare == ERROR_INTEGER) {
                              printf("Wrong format: correct format is [char][int][char][int].\n");
                              continue;
                         }

                         //  Check legality & movetype
                         bool legal = false;
                         for (int i = 0; i < currentBoardLegalMoveCount; i++) {
                              if (initialSquare == currentBoardLegalMoveList[i][0] && terminalSquare == currentBoardLegalMoveList[i][1]) {
                                   legal = true;
                                   moveType = currentBoardLegalMoveList[i][2];
                                   break;
                              }
                         }
                         if (legal == false) {
                              printf("The move is illegal!\n");
                              continue;
                         }
                         else {
                              correctInput = true;
                         }
                         //  TODO: check if there is anything else to check :D
                    }
                    // Check Promotion
                    if (currentBoard.getSquare(initialSquare) == WHITEPAWN && A8 <= terminalSquare && terminalSquare <= H8 ||
                         currentBoard.getSquare(initialSquare) == BLACKPAWN && A1 <= terminalSquare && terminalSquare <= H1) {
                         correctInput = false;
                         while (!correctInput) {
                              printf("Please pick a piece to promote to (N, B, R, Q): ");
                              std::getline(cin, userCommand); // do I want to get the entire command?

                              //  Check size
                              if (userCommand.size() != 1) {
                                   printf("Just one letter: N, B, R, Q\n");
                                   continue;
                              }
                              if (userCommand.at(0) != 'N' && userCommand.at(0) != 'B' && userCommand.at(0) != 'R' && userCommand.at(0) != 'Q') {
                                   printf("Wrong letter: just N, B, R, Q\n");
                                   continue;
                              }

                              switch (userCommand.at(0)) {
                              case 'N':
                                   moveType = KNIGHT_PROMOTION;
                                   correctInput = true;
                                   break;
                              case 'B':
                                   moveType = BISHOP_PROMOTION;
                                   correctInput = true;
                                   break;
                              case 'R':
                                   moveType = ROOK_PROMOTION;
                                   correctInput = true;
                                   break;
                              case 'Q':
                                   moveType = QUEEN_PROMOTION;
                                   correctInput = true;
                                   break;
                              }
                         }
                    }
                    
                    int userMove[3] = { initialSquare, terminalSquare, moveType};
                    // save terminalValue for undoMove;
                    savedTerminalValue[halfMoveCount] = makeMove(currentBoard, userMove);
                    
                    for (int i = 0; i < 3; i++) {
                         savedMove[halfMoveCount][i] = userMove[i];
                    }

                    if (currentBoard.getTurn() == WHITE) { currentBoard.moveNumberIncrement(); }
                    halfMoveCount++;

                    // add to log file
                    logtext << currentBoard.getMoveNumber() << ": " << numberToFile(userMove[0]) << numberToRank(userMove[0]) << " " << numberToFile(userMove[1]) << numberToRank(userMove[1]) << endl;

                    continue;
               }
               else if (commandType == DISPLAY_BOARD) {
                    printSimpleBoard(currentBoard);
                    continue;
               }
               else if (commandType == DISPLAY_FEN) {
                    boardToFEN(currentBoard);
                    continue;
               }
               else if (commandType == BOARD_RESET) {
                    board120Setup();
                    printSimpleBoard(currentBoard);
                    continue;
               }
               else if (commandType == PERFT) {
                    correctInput = false;
                    while (!correctInput) {
                         printf("What depth? (1~%d): ", MAXIMUM_DEPTH);
                         std::getline(cin, userCommand);
                         if (userCommand.size() == 0 || userCommand.at(0) - '0' < 1 || userCommand.at(0) - '0' > MAXIMUM_DEPTH) {
                              printf("Wrong Input!\n");
                              continue;
                         }
                         else {
                              printf("Perft (Depth %c): %llu\n", userCommand.at(0), divide(userCommand.at(0) - '0', 0, currentBoard, false));
                              correctInput = true;
                              break;
                         }
                    }
               }
               else if (commandType == QUIT) {
                    gamePlaying = false;
                    break;
               }
               else if (commandType == DIVIDE) {
                    correctInput = false;
                    while (!correctInput) {
                         printf("What depth? (1~%d): ", MAXIMUM_DEPTH);
                         std::getline(cin, userCommand);
                         if (userCommand.size() == 0 || userCommand.at(0) - '0' < 1 || userCommand.at(0) - '0' > MAXIMUM_DEPTH) {
                              printf("Wrong Input!\n");
                              continue;
                         }
                         else {
                              printf("Divide (Depth %c): %llu\n", userCommand.at(0), divide(userCommand.at(0) - '0', userCommand.at(0) - '0', currentBoard, true));
                              correctInput = true;
                              break;
                         }
                    }
               }
               else if (commandType == UNDO_MOVE) {
                    //  TerminalSquare needs to be saved
                    if (savedTerminalValue[halfMoveCount] == ERROR_INTEGER || halfMoveCount == 0) {
                         printf("No move can be undone!\n");
                         continue;
                    }
                    else {
                         halfMoveCount--;
                         undoMove(currentBoard, savedMove[halfMoveCount], savedTerminalValue[halfMoveCount]);
                    }

                    // Update castlingCheck, enpassantSquare, currentTurn, moveNumber, fiftyMoveCount
                    for (int i = 0; i < 4; i++) {
                         currentBoard.setCastling(i, savedBoard[halfMoveCount].getCastling(i));
                    }
                    currentBoard.setEnpassantSquare(savedBoard[halfMoveCount].getEnpassantSquare());
                    if (currentBoard.getFiftyMoveCount() > 0) {
                         currentBoard.fiftyMoveCountDecrement();
                    }
                    if (currentBoard.getTurn() == BLACK) {
                         currentBoard.moveNumberDecrement();
                    }
                    
                    // Now user makes the next move
                    userColor = -userColor;
               }
               else if (commandType == COM_MAKE_MOVE) {
                    userColor = -userColor;
                    continue;
               }
               else if (commandType == PRINT_ALL_MOVES) {
                    moveGeneration(currentBoard, currentBoardMoveList, &currentBoardMoveCount);
                    legalMoves(currentBoard, currentBoardMoveList, currentBoardMoveCount, currentBoardLegalMoveList, &currentBoardLegalMoveCount);

                    printf("Movecount: %d\n", currentBoardLegalMoveCount);
                    for (int i = 0; i < currentBoardLegalMoveCount; i++) {
                         printf("%d: ", i + 1);
                         printMove(currentBoardLegalMoveList[i]);
                    }
                    continue;
               }
               else if (commandType == ALPHABETA_COMPARE) {

                    LARGE_INTEGER frequency2, beginTime2, endTime2;
                    
                    frequency2 = startTimer(&beginTime2, 2);
                    int negamaxValue = negaMax(EVAL_DEPTH, currentBoard);
                    stopTimer(&endTime2, 2);
                    printf("Negamax Value: %d\n", negamaxValue);
                    std::cout << "Negamax timer " << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms elapsed." << std::endl;

                    frequency2 = startTimer(&beginTime2, 2);
                    int alphabetaValue = alphabeta(EVAL_DEPTH, currentBoard, -999999, 999999);
                    stopTimer(&endTime2, 2);
                    printf("Alphabeta Value: %d\n", alphabetaValue);
                    std::cout << "Alphabeta timer : " << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms elapsed." << std::endl;

               }
               else if (commandType == EVALUATE_BOARD) {
                    printf("Current Board Evaluation: %d\n", boardEvaluation(currentBoard));
                    continue;
               }
               else if (commandType == ALPHABETA_SPEED_CHECK) {
                    LARGE_INTEGER frequency2, beginTime2, endTime2;

                    frequency2 = startTimer(&beginTime2, 2);
                    int alphabetaValue = alphabeta(EVAL_DEPTH, currentBoard, -999999, 999999);
                    stopTimer(&endTime2, 2);
                    printf("Alphabeta Value: %d\n", alphabetaValue);
                    std::cout << "Alphabeta timer : " << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms elapsed." << std::endl;
               }
               else if (commandType == PRINT_SAVED_FEN) {
                    for (int i = 0; i < halfMoveCount; i++) {
                         boardToFEN(savedBoard[i]); // Print statement inside boardToFEN() prints the FEN
                    }
               }
          }
          else if (currentBoard.getTurn() == -userColor || spectate == true) {
               
               savedBoard[halfMoveCount] = currentBoard;

               int alphabetaMove[3];
               int alphabetaValue = rootAlphabeta(EVAL_DEPTH, currentBoard, -999999, 999999, alphabetaMove);
               printf("Alphabeta Value: %d\n", alphabetaValue);
               printf("Alphabeta Move: ");
               printMove(alphabetaMove);

               int moveToMake[3];
               for (int i = 0; i < 3; i++) {
                    moveToMake[i] = alphabetaMove[i];
               }

               //  Increment or reset Fifty move count
               //  TODO: Add 50 Move Rule option in move generation / selection
               if (currentBoard.getSquare(moveToMake[1]) == EMPTYSQUARE
                    && currentBoard.getSquare(moveToMake[0]) != WHITEPAWN
                    && currentBoard.getSquare(moveToMake[0]) != BLACKPAWN) {
                    currentBoard.fiftyMoveCountIncrement();
               }
               else { currentBoard.setFiftyMoveCount(0); }

               //  Save castlingCheck for undoMove
               for (int i = 0; i < 4; i++) {
                    savedBoard[halfMoveCount].setCastling(i, currentBoard.getCastling(i));
               }
               //  Update castlingCheck
               if (currentBoard.getSquare(moveToMake[0]) == WHITEROOK && moveToMake[0] == A1) {
                    currentBoard.setCastling(WQCASTLING, false);
               }
               else if (currentBoard.getSquare(moveToMake[0]) == WHITEROOK && moveToMake[0] == H1) {
                    currentBoard.setCastling(WKCASTLING, false);
               }
               else if (currentBoard.getSquare(moveToMake[0]) == BLACKROOK && moveToMake[0] == A8) {
                    currentBoard.setCastling(BQCASTLING, false);
               }
               else if (currentBoard.getSquare(moveToMake[0]) == BLACKROOK && moveToMake[0] == H8) {
                    currentBoard.setCastling(BKCASTLING, false);
               }
               else if (currentBoard.getSquare(moveToMake[0]) == WHITEKING && moveToMake[0] == E1) {
                    currentBoard.setCastling(WKCASTLING, false);
                    currentBoard.setCastling(WQCASTLING, false);
               }
               else if (currentBoard.getSquare(moveToMake[0]) == BLACKKING && moveToMake[0] == E8) {
                    currentBoard.setCastling(BKCASTLING, false);
                    currentBoard.setCastling(BQCASTLING, false);
               }

               //  Update enpassant square
               if (moveToMake[2] == DOUBLEMOVE) {
                    currentBoard.setEnpassantSquare((moveToMake[0] + moveToMake[1]) / 2);
               }
               else { currentBoard.setEnpassantSquare(0); }

               //  Make best move and print board
               savedTerminalValue[halfMoveCount] = makeMove(currentBoard, moveToMake);
               //  Save move for undoMove
               for (int i = 0; i < 3; i++) {
                    savedMove[halfMoveCount][i] = moveToMake[i];
               }
               logtext << currentBoard.getMoveNumber() << ": " << numberToFilerank(moveToMake[0]) << " " << numberToFilerank(moveToMake[1]) << endl;
               
               printSimpleBoard(currentBoard);

               //  Print out move and move number
               if (moveToMake[2] == KINGSIDE_CASTLING) {
                    printf("%d: O-O\n", currentBoard.getMoveNumber());
               }
               else if (moveToMake[2] == QUEENSIDE_CASTLING) {
                    printf("%d: O-O-O\n", currentBoard.getMoveNumber());
               }
               else {
                    printf("%d: ", currentBoard.getMoveNumber());
                    printMove(moveToMake);
               }

               //  Increment move
               if (currentBoard.getTurn() == WHITE) { currentBoard.moveNumberIncrement(); }

               //  Check if game is over
               gamePlaying = !checkGameEnd(currentBoard);
               if (!gamePlaying) { break; }

               //  TODO: Check Threefold repetition
               bool repetition = false;
               for (int i = 0; i < halfMoveCount; i++) {
                    bool sameState = false;
                    for (int j = 0; j < 120; j++) {
                         if (savedBoard[i].getSquare(j) != currentBoard.getSquare(j)) {
                              sameState = false;
                              break;
                         }
                    }
                    for (int j = 0; j < 4; j++) {
                         if (savedBoard[i].getCastling(j) != currentBoard.getCastling(j)) {
                              sameState = false;
                              break;
                         }
                    }
                    if (savedBoard[i].getEnpassantSquare() != currentBoard.getEnpassantSquare()) {
                         sameState = false;
                         break;
                    }
                    if (sameState == true) {
                         repetitionCount[i]++;
                         repetition = true;
                         break;
                    }
               }
               if (repetition == false) {
                    repetitionCount[halfMoveCount] = 1;
               }


               //  75 Move Rule Implement (unless checkmate)
               if (currentBoard.getFiftyMoveCount() >= 75) {
                    gamePlaying = false;
                    gameResult = TIE;
                    break;
               }
               
               // At the very end since multiple things are saved while the computer makes a move
               halfMoveCount++;
          }
     }

     //  Output Game Result
     printf("Game Result: ");
     logtext << "Game Result: ";
     switch (gameResult) {
     case BLACK_WIN:
          printf("0-1\n");
          logtext << "0-1" << endl;
          break;
     case TIE:
          printf("1/2-1/2\n");
          logtext << "1/2-1/2" << endl;
          break;
     case WHITE_WIN:
          printf("1-0\n");
          logtext << "1-0" << endl;
          break;
     case NOT_FINISHED:
          printf("0-0: Game not finished\n");
          logtext << "0-0: Game not finished" << endl;
     }

     //  stop timer
     stopTimer(&endTime, 1);
     //  print elapsed time
     printElapsedTime(beginTime, endTime, frequency, 1);

     logtext << "Total Time: " << elapsedTime(beginTime, endTime, frequency, 1) << "ms" << endl;
}