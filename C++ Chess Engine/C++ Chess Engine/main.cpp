
#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include "protos.h"
#include "defs.h"

using namespace std;

/******************************************************************************/
/*                                 GLOBAL VARIABLE                            */
/******************************************************************************/
int currentBoard[120];
bool gamePlaying = true;
bool endGame = false;
//  PCSQ Tables from 
//  https://chessprogramming.wikispaces.com/Simplified+evaluation+function
int PAWN_PCSQTable[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
     50, 50, 50, 50, 50, 50, 50, 50,
     10, 10, 20, 30, 30, 20, 10, 10,
      5,  5, 10, 25, 25, 10,  5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5, -5,-10,  0,  0,-10, -5,  5,
      5, 10, 10,-20,-20, 10, 10,  5,
      0,  0,  0,  0,  0,  0,  0,  0
};
int KNIGHT_PCSQTable[64] = {
     -50,-40,-30,-30,-30,-30,-40,-50,
     -40,-20,  0,  0,  0,  0,-20,-40,
     -30,  0, 10, 15, 15, 10,  0,-30,
     -30,  5, 15, 20, 20, 15,  5,-30,
     -30,  0, 15, 20, 20, 15,  0,-30,
     -30,  5, 10, 15, 15, 10,  5,-30,
     -40,-20,  0,  5,  5,  0,-20,-40,
     -50,-40,-30,-30,-30,-30,-40,-50
};
int BISHOP_PCSQTable[64] = {
     -20,-10,-10,-10,-10,-10,-10,-20,
     -10,  0,  0,  0,  0,  0,  0,-10,
     -10,  0,  5, 10, 10,  5,  0,-10,
     -10,  5,  5, 10, 10,  5,  5,-10,
     -10,  0, 10, 10, 10, 10,  0,-10,
     -10, 10, 10, 10, 10, 10, 10,-10,
     -10,  5,  0,  0,  0,  0,  5,-10,
     -20,-10,-10,-10,-10,-10,-10,-20
};
int ROOK_PCSQTable[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10, 10, 10, 10, 10,  5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
      0,  0,  0,  5,  5,  0,  0,  0
};
int QUEEN_PCSQTable[64] = {
     -20,-10,-10, -5, -5,-10,-10,-20,
     -10,  0,  0,  0,  0,  0,  0,-10,
     -10,  0,  5,  5,  5,  5,  0,-10,
      -5,  0,  5,  5,  5,  5,  0, -5,
       0,  0,  5,  5,  5,  5,  0, -5,
     -10,  5,  5,  5,  5,  5,  0,-10,
     -10,  0,  5,  0,  0,  0,  0,-10,
     -20,-10,-10, -5, -5,-10,-10,-20
};
int KING_PCSQTable[64] = {
     -30,-40,-40,-50,-50,-40,-40,-30,
     -30,-40,-40,-50,-50,-40,-40,-30,
     -30,-40,-40,-50,-50,-40,-40,-30,
     -30,-40,-40,-50,-50,-40,-40,-30,
     -20,-30,-30,-40,-40,-30,-30,-20,
     -10,-20,-20,-20,-20,-20,-20,-10,
      20, 20,  0,  0,  0,  0, 20, 20,
      20, 30, 10,  0,  0, 10, 30, 20
};
int KING_PCSQTable_ENDGAME[64] = {
     -50,-40,-30,-20,-20,-30,-40,-50,
     -30,-20,-10,  0,  0,-10,-20,-30,
     -30,-10, 20, 30, 30, 20,-10,-30,
     -30,-10, 30, 40, 40, 30,-10,-30,
     -30,-10, 30, 40, 40, 30,-10,-30,
     -30,-10, 20, 30, 30, 20,-10,-30,
     -30,-30,  0,  0,  0,  0,-30,-30,
     -50,-30,-30,-30,-30,-30,-30,-50
};
//  true if king/rook did not move, false if it moved
bool whiteKingsideCastling = true, whiteQueensideCastling = true,
blackKingsideCastling = true, blackQueensideCastling = true;
//  clock for fifty move rule
int halfMoveClock = 0;
//  Current Move Number, starts at 1
int moveNumber = 1;
//  WHITE or BLACK
int currentTurn = WHITE; 
//  0 if double move did not happen, square value (ex. F3) otherwise
int enpassantSquare = 0; 
//  move[3]: initial, terminal, moveType
int currentBoardMoveList[MAX_MOVEGEN_COUNT][3];
int currentBoardMoveCount;
int currentBoardLegalMoveList[MAX_MOVEGEN_COUNT][3];
int currentBoardLegalMoveCount;
int depthAllMoveList[MAXIMUM_DEPTH + 1][MAX_MOVEGEN_COUNT][3];
int depthAllMoveCount[MAXIMUM_DEPTH + 1];
int depthEnpassantSquare[MAXIMUM_DEPTH + 1];
int depthLegalMoveList[MAXIMUM_DEPTH + 1][MAX_MOVEGEN_COUNT][3];
int depthLegalMoveCount[MAXIMUM_DEPTH + 1];
//  added for time performance check
LARGE_INTEGER frequency, beginTime, endTime;
//  for storing best moves
int depthBestMoves[MAXIMUM_DEPTH + 1][3];
//  Counter to check Fifty Move rule and 75 Move Rule
int fiftyMoveCount = 0;
//  Records the result of the game
int gameResult = 0;
//  Stores Board and Board States for threefold repetition
int savedBoard[MAX_MOVENUMBER + 1][120];
int savedCastling[MAX_MOVENUMBER + 1][4];
int savedEnpassant[MAX_MOVENUMBER + 1];

/******************************************************************************/
/*                                  FUNCTIONS                                 */
/******************************************************************************/

/*                             BOARD SETUP FUNCTIONS                          */
void board120Setup() {
     //  set turn to WHITE
     currentTurn = WHITE;
     
     //  Add Empty Squares
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               currentBoard[(i + 2) * 10 + j + 1] = EMPTYSQUARE;
          }
     }
     
     //  Add Error Squares
     for (int j = 0; j < 10; j++) {
          currentBoard[j] = ERRORSQUARE;
          currentBoard[10 + j] = ERRORSQUARE;
          currentBoard[100 + j] = ERRORSQUARE;
          currentBoard[110 + j] = ERRORSQUARE;
     }
     for (int j = 0; j < 12; j++) {
          currentBoard[10 * j] = ERRORSQUARE;
          currentBoard[10 * j + 9] = ERRORSQUARE;
     }
     
     //  Add Non-Pawn Pieces
     currentBoard[A8] = BLACKROOK;
     currentBoard[B8] = BLACKKNIGHT;
     currentBoard[C8] = BLACKBISHOP;
     currentBoard[D8] = BLACKQUEEN;
     currentBoard[E8] = BLACKKING;
     currentBoard[F8] = BLACKBISHOP;
     currentBoard[G8] = BLACKKNIGHT;
     currentBoard[H8] = BLACKROOK;

     currentBoard[A1] = WHITEROOK;
     currentBoard[B1] = WHITEKNIGHT;
     currentBoard[C1] = WHITEBISHOP;
     currentBoard[D1] = WHITEQUEEN;
     currentBoard[E1] = WHITEKING;
     currentBoard[F1] = WHITEBISHOP;
     currentBoard[G1] = WHITEKNIGHT;
     currentBoard[H1] = WHITEROOK;

     //  Add Pawn Pieces
     for (int i = 0; i < 8; i++) {
          currentBoard[A2+i] = WHITEPAWN;
          currentBoard[A7+i] = BLACKPAWN;
     }
}
void FENboardSetup(int board[120], std::string FEN) {
     whiteKingsideCastling = false;
     whiteQueensideCastling = false;
     blackKingsideCastling = false;
     blackQueensideCastling = false;
     enpassantSquare = 0;

     for (int i = 0; i < 10; i++) {
          currentBoard[i] = ERRORSQUARE;
          currentBoard[ROW + i] = ERRORSQUARE;
          currentBoard[10 * ROW + i] = ERRORSQUARE;
          currentBoard[11 * ROW + i] = ERRORSQUARE;
     }

     for (int i = 0; i < 12; i++) {
          currentBoard[i*ROW] = ERRORSQUARE;
          currentBoard[i*ROW + 9] = ERRORSQUARE;
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
     if (FEN.at(i) == 'w') { currentTurn = WHITE; }
     else { currentTurn = BLACK; }

     i += 2;
     if (FEN.at(i) != '-') {
          while (FEN.at(i) != ' ') {
               if (FEN.at(i) == 'K') {
                    whiteKingsideCastling = true;
               }
               if (FEN.at(i) == 'Q') {
                    whiteQueensideCastling = true;
               }
               if (FEN.at(i) == 'k') {
                    blackKingsideCastling = true;
               }
               if (FEN.at(i) == 'q') {
                    blackQueensideCastling = true;
               }
               i++;
          }
          i++;
     }
     else { i += 2; }

     if (FEN.at(i) != '-') {
          //  get enpassant square
          enpassantSquare = COLUMN*(FEN.at(i) - 'a' + 1);
          i++;
          enpassantSquare += ROW*('9' - FEN.at(i) + 1);
     }

     i += 2;
     halfMoveClock = FEN.at(i) - '0';

     i += 2;
     moveNumber = FEN.at(i) - '0';


}
string boardToFEN(int board[120], int turn,
     bool WKCastling, bool WQCastling, bool BKCastling, bool BQCastling,
     int enpassantSquare, int halfMoveClock, int moveNumber) {
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
     FEN += ('0' + halfMoveClock);
     FEN += ' ';
     FEN += ('0' + moveNumber);

     cout << FEN << endl;
     return FEN;

}
void printBoard(int board[120]) {
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

/*                             EVALUATION FUNCTIONS                           */
int boardEvaluation(int board[120]) {
     int score = 0;
     for (int i = 0; i < 120; i++) {
          switch (board[i]) {
          case WHITEPAWN:
               score += PAWNVALUE;
               score += PAWN_PCSQTable[position120to64(i)];
               break;
          case WHITEKNIGHT:
               score += KNIGHTVALUE;
               score += KNIGHT_PCSQTable[position120to64(i)];
               break;
          case WHITEBISHOP:
               score += BISHOPVALUE;
               score += BISHOP_PCSQTable[position120to64(i)];
               break;
          case WHITEROOK:
               score += ROOKVALUE;
               score += ROOK_PCSQTable[position120to64(i)];
               break;
          case WHITEQUEEN:
               score += QUEENVALUE;
               score += QUEEN_PCSQTable[position120to64(i)];
               break;
          case WHITEKING:
               score += KINGVALUE;
               if (endGame) {
                    score += KING_PCSQTable_ENDGAME[position120to64(i)];
               }
               else {
                    score += KING_PCSQTable[position120to64(i)];
               }
               break;
          case BLACKPAWN:
               score -= PAWNVALUE;
               score -= PAWN_PCSQTable[position120to64(reversePosition(i))];
               break;
          case BLACKKNIGHT:
               score -= KNIGHTVALUE;
               score -= KNIGHT_PCSQTable[position120to64(reversePosition(i))];
               break;
          case BLACKBISHOP:
               score -= BISHOPVALUE;
               score -= BISHOP_PCSQTable[position120to64(reversePosition(i))];
               break;
          case BLACKROOK:
               score -= ROOKVALUE;
               score -= ROOK_PCSQTable[position120to64(reversePosition(i))];
               break;
          case BLACKQUEEN:
               score -= QUEENVALUE;
               score -= QUEEN_PCSQTable[position120to64(reversePosition(i))];
               break;
          case BLACKKING:
               score -= KINGVALUE;
               if (endGame) {
                    score -= KING_PCSQTable_ENDGAME[position120to64(reversePosition(i))];
               }
               else {
                    score -= KING_PCSQTable[position120to64(reversePosition(i))];
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
int position64to120(int position64) {
     int row = position64 / 8, column = position64 % 8;

     return (row + 2) * 10 + column + 1;

}
int position120to64(int position120) {
     int row = position120 / 10 - 2, column = position120 % 10 - 1;

     return row * 8 + column;
}

//  implementation of minimax using pseudocode from this link:
//  http://www.hamedahmadi.com/gametree/
int blueValue(int depth, int turn, bool castlingCheck[4]) {
     if (depth <= 0) {
          return turn * boardEvaluation(currentBoard);
     }
     //  TODO: Use infinity for safety
     int max_Score = -100000;
     int score;
     
     depthAllMoveCount[depth] = 0;
     depthLegalMoveCount[depth] = 0;
     depthEnpassantSquare[depth - 1] = 0;

     int terminalValue;
     bool copyCastlingCheck[4];

     // MOVEGEN
     moveGeneration(currentBoard, turn, depthAllMoveList[depth], &depthAllMoveCount[depth], depthEnpassantSquare[depth], castlingCheck);
     // CHECK FOR LEGALS
     legalMoves(currentBoard, turn, depthAllMoveList[depth], depthAllMoveCount[depth], depthLegalMoveList[depth], &depthLegalMoveCount[depth]);


     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {
     
          //  defensive copy of castlingCheck
          for (int j = 0; j < 4; j++) { copyCastlingCheck[j] = castlingCheck[j]; }

          if (currentBoard[depthLegalMoveList[depth][i][0]] == WHITEKING) {
               copyCastlingCheck[WKCASTLING] = false;
               copyCastlingCheck[WQCASTLING] = false;
          }
          if (currentBoard[depthLegalMoveList[depth][i][0]] == BLACKKING) {
               copyCastlingCheck[BKCASTLING] = false;
               copyCastlingCheck[BQCASTLING] = false;
          }
          if (currentBoard[depthLegalMoveList[depth][i][0]] == WHITEROOK) {
               if (depthLegalMoveList[depth][i][0] == A1) {
                    copyCastlingCheck[WQCASTLING] = false;
               }
               if (depthLegalMoveList[depth][i][0] == H1) {
                    copyCastlingCheck[WKCASTLING] = false;
               }
          }
          if (currentBoard[depthLegalMoveList[depth][i][0]] == BLACKROOK) {
               if (depthLegalMoveList[depth][i][0] == A8) {
                    copyCastlingCheck[BQCASTLING] = false;
               }
               if (depthLegalMoveList[depth][i][0] == H8) {
                    copyCastlingCheck[BKCASTLING] = false;
               }
          }

          terminalValue = makeMove(currentBoard, depthLegalMoveList[depth][i]);
     
          if (depthLegalMoveList[depth][i][2] == DOUBLEMOVE) {
               depthEnpassantSquare[depth - 1] = terminalValue;
               //  this terminal value is actually enpassantSquare
          }
          else { // if not, revert it back to 0
               depthEnpassantSquare[depth - 1] = 0;
          }

          score = redValue(depth - 1, -turn, copyCastlingCheck);
          // printf("blueValue Score Print %d\n", score); // TODO: Delete this after debugging
          
          if (score > max_Score) {
               max_Score = score;
               //  TODO: save all moves leading to this
               depthBestMoves[depth][0] = depthLegalMoveList[depth][i][0];
               depthBestMoves[depth][1] = depthLegalMoveList[depth][i][1];
               depthBestMoves[depth][2] = depthLegalMoveList[depth][i][2];
          }

          undoMove(currentBoard, depthLegalMoveList[depth][i], terminalValue);
     }
     
     return max_Score;
}
int redValue(int depth, int turn, bool castlingCheck[4]) {
     if (depth <= 0) {
          return turn * boardEvaluation(currentBoard);
     }
     //  TODO: Use infinity for safety
     int min_Score = 100000;
     int score;

     depthAllMoveCount[depth] = 0;
     depthLegalMoveCount[depth] = 0;
     depthEnpassantSquare[depth - 1] = 0;

     int terminalValue;
     bool copyCastlingCheck[4];

     // MOVEGEN
     moveGeneration(currentBoard, turn, depthAllMoveList[depth], &depthAllMoveCount[depth], depthEnpassantSquare[depth], castlingCheck);
     // CHECK FOR LEGALS
     legalMoves(currentBoard, turn, depthAllMoveList[depth], depthAllMoveCount[depth], depthLegalMoveList[depth], &depthLegalMoveCount[depth]);


     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {
          //  defensive copy of castlingCheck
          for (int j = 0; j < 4; j++) { copyCastlingCheck[j] = castlingCheck[j]; }

          if (currentBoard[depthLegalMoveList[depth][i][0]] == WHITEKING) {
               copyCastlingCheck[WKCASTLING] = false;
               copyCastlingCheck[WQCASTLING] = false;
          }
          if (currentBoard[depthLegalMoveList[depth][i][0]] == BLACKKING) {
               copyCastlingCheck[BKCASTLING] = false;
               copyCastlingCheck[BQCASTLING] = false;
          }
          if (currentBoard[depthLegalMoveList[depth][i][0]] == WHITEROOK) {
               if (depthLegalMoveList[depth][i][0] == A1) {
                    copyCastlingCheck[WQCASTLING] = false;
               }
               if (depthLegalMoveList[depth][i][0] == H1) {
                    copyCastlingCheck[WKCASTLING] = false;
               }
          }
          if (currentBoard[depthLegalMoveList[depth][i][0]] == BLACKROOK) {
               if (depthLegalMoveList[depth][i][0] == A8) {
                    copyCastlingCheck[BQCASTLING] = false;
               }
               if (depthLegalMoveList[depth][i][0] == H8) {
                    copyCastlingCheck[BKCASTLING] = false;
               }
          }

          terminalValue = makeMove(currentBoard, depthLegalMoveList[depth][i]);

          if (depthLegalMoveList[depth][i][2] == DOUBLEMOVE) {
               depthEnpassantSquare[depth - 1] = terminalValue;
               //  this terminal value is actually enpassantSquare
          }
          else { // if not, revert it back to 0
               depthEnpassantSquare[depth - 1] = 0;
          }

          score = blueValue(depth - 1, -turn, copyCastlingCheck);
          if (score < min_Score) {
               min_Score = score;
               depthBestMoves[depth][0] = depthLegalMoveList[depth][i][0];
               depthBestMoves[depth][1] = depthLegalMoveList[depth][i][1];
               depthBestMoves[depth][2] = depthLegalMoveList[depth][i][2];
          }

          undoMove(currentBoard, depthLegalMoveList[depth][i], terminalValue);
     }

     return min_Score;
}


/*                             GAME CYCLE FUNCTIONS                           */
bool checkGameEnd(int board[120]) {
     bool whiteKing = false, blackKing = false;
     for (int i = 0; i < 120; i++) {
          if (board[i] == WHITEKING) { whiteKing = true; }
          if (board[i] == BLACKKING) { blackKing = true; }
     }
     return !(whiteKing && blackKing);
}


/*                           MOVE GENERATION FUNCTIONS                        */
void moveGeneration(int board[120], int turn, int moveList[250][3], int *moveCount, int enpassantSquare, bool castlingCheck[4]) {
     castlingMoveGeneration(board, turn, moveList, moveCount, castlingCheck);
     enpassantMoveGeneration(board, turn, moveList, moveCount, enpassantSquare);

     if (turn == WHITE) {
          for (int i = 0; i < 120; i++) {
               switch (board[i]) {
               case WHITEPAWN:
                    pawnMoveGeneration(board, turn, i, moveList, moveCount);
                    break;
               case WHITEKNIGHT:
                    knightMoveGeneration(board, turn, i, moveList, moveCount);
                    break;
               case WHITEBISHOP:
                    bishopMoveGeneration(board, turn, i, moveList, moveCount);
                    break;
               case WHITEROOK:
                    rookMoveGeneration(board, turn, i, moveList, moveCount);
                    break;
               case WHITEQUEEN:
                    queenMoveGeneration(board, turn, i, moveList, moveCount);
                    break;
               case WHITEKING:
                    kingMoveGeneration(board, turn, i, moveList, moveCount);
                    break;
               }
          }
     }
     if (turn == BLACK) {
          for (int i = 0; i < 120; i++) {
               switch (board[i]) {
               case BLACKPAWN:
                    pawnMoveGeneration(board, turn, i, moveList, moveCount);
                    break;
               case BLACKKNIGHT:
                    knightMoveGeneration(board, turn, i, moveList, moveCount);
                    break;
               case BLACKBISHOP:
                    bishopMoveGeneration(board, turn, i, moveList, moveCount);
                    break;
               case BLACKROOK:
                    rookMoveGeneration(board, turn, i, moveList, moveCount);
                    break;
               case BLACKQUEEN:
                    queenMoveGeneration(board, turn, i, moveList, moveCount);
                    break;
               case BLACKKING:
                    kingMoveGeneration(board, turn, i, moveList, moveCount);
                    break;
               }
          }
     }
}
void pawnMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount) {
     if (turn == WHITE) {
          //  if on the last row before promotion, just call promotion
          if (A7 <= position && position <= H7) {
               promotionMoveGeneration(board, turn, position, moveList, moveCount);
               return;
          }

          //  Advance 1 square
          if (board[position - ROW] == EMPTYSQUARE) {
               addMove(position, position - ROW, NORMAL, moveList, moveCount);
               //  Advance 2 squares
               if (A2 <= position && position <= H2 &&
                    board[position - 2 * ROW] == EMPTYSQUARE) {
                    addMove(position, position - 2 * ROW, DOUBLEMOVE, moveList, moveCount);
               }
          }

          //  attack diagonals
          if (checkColor(board[position - ROW - COLUMN]) == BLACK) {
               addMove(position, position - ROW - COLUMN, NORMAL, moveList, moveCount);
          }
          if (checkColor(board[position - ROW + COLUMN]) == BLACK) {
               addMove(position, position - ROW + COLUMN, NORMAL, moveList, moveCount);
          }
     }
     if (turn == BLACK) {
          //  if on the last row before promotion, just call promotion
          if (A2 <= position && position <= H2) {
               promotionMoveGeneration(board, turn, position, moveList, moveCount);
               return;
          }

          //  Advance 1 square
          if (board[position + ROW] == EMPTYSQUARE) {
               addMove(position, position + ROW, NORMAL, moveList, moveCount);
               //  Advance 2 squares
               if (A7 <= position && position <= H7 &&
                    board[position + 2 * ROW] == EMPTYSQUARE) {
                    addMove(position, position + 2 * ROW, DOUBLEMOVE, moveList, moveCount);
               }
          }

          //  attack diagonals
          if (checkColor(board[position + ROW - COLUMN]) == WHITE) {
               addMove(position, position + ROW - COLUMN, NORMAL, moveList, moveCount);
          }
          if (checkColor(board[position + ROW + COLUMN]) == WHITE) {
               addMove(position, position + ROW + COLUMN, NORMAL, moveList, moveCount);
          }
     }
}
void knightMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount) {
     if (checkColor(board[position + ROW + 2 * COLUMN]) == -turn ||
          board[position + ROW + 2 * COLUMN] == EMPTYSQUARE) {
          addMove(position, position + ROW + 2 * COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position + ROW - 2 * COLUMN]) == -turn ||
          board[position + ROW - 2 * COLUMN] == EMPTYSQUARE) {
          addMove(position, position + ROW - 2 * COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position - ROW + 2 * COLUMN]) == -turn ||
          board[position - ROW + 2 * COLUMN] == EMPTYSQUARE) {
          addMove(position, position - ROW + 2 * COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position - ROW - 2 * COLUMN]) == -turn ||
          board[position - ROW - 2 * COLUMN] == EMPTYSQUARE) {
          addMove(position, position - ROW - 2 * COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position + 2 * ROW + COLUMN]) == -turn ||
          board[position + 2 * ROW + COLUMN] == EMPTYSQUARE) {
          addMove(position, position + 2 * ROW + COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position + 2 * ROW - COLUMN]) == -turn ||
          board[position + 2 * ROW - COLUMN] == EMPTYSQUARE) {
          addMove(position, position + 2 * ROW - COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position - 2 * ROW + COLUMN]) == -turn ||
          board[position - 2 * ROW + COLUMN] == EMPTYSQUARE) {
          addMove(position, position - 2 * ROW + COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position - 2 * ROW - COLUMN]) == -turn ||
          board[position - 2 * ROW - COLUMN] == EMPTYSQUARE) {
          addMove(position, position - 2 * ROW - COLUMN, NORMAL, moveList, moveCount);
     }
}
void bishopMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount) {
     bool topright = true, downright = true, downleft = true, topleft = true;
     for (int i = 1; i < 8; i++) {
          if (topright == true &&
               (checkColor(board[position - i*ROW + i*COLUMN]) == -turn ||
                    board[position - i*ROW + i*COLUMN] == EMPTYSQUARE)) {
               addMove(position, position - i*ROW + i*COLUMN, NORMAL, moveList, moveCount);
               if (checkColor(board[position - i*ROW + i*COLUMN]) == -turn) {
                    topright = false;
               }
          }
          else { topright = false; }

          if (downright == true &&
               (checkColor(board[position + i*ROW + i*COLUMN]) == -turn ||
                    board[position + i*ROW + i*COLUMN] == EMPTYSQUARE)) {
               addMove(position, position + i*ROW + i*COLUMN, NORMAL, moveList, moveCount);
               if (checkColor(board[position + i*ROW + i*COLUMN]) == -turn) {
                    downright = false;
               }
          }
          else { downright = false; }

          if (downleft == true &&
               (checkColor(board[position + i*ROW - i*COLUMN]) == -turn ||
                    board[position + i*ROW - i*COLUMN] == EMPTYSQUARE)) {
               addMove(position, position + i*ROW - i*COLUMN, NORMAL, moveList, moveCount);
               if (checkColor(board[position + i*ROW - i*COLUMN]) == -turn) {
                    downleft = false;
               }
          }
          else { downleft = false; }

          if (topleft == true &&
               (checkColor(board[position - i*ROW - i*COLUMN]) == -turn ||
                    board[position - i*ROW - i*COLUMN] == EMPTYSQUARE)) {
               addMove(position, position - i*ROW - i*COLUMN, NORMAL, moveList, moveCount);
               if (checkColor(board[position - i*ROW - i*COLUMN]) == -turn) {
                    topleft = false;
               }
          }
          else { topleft = false; }
     }
}
void rookMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount) {
     bool top = true, right = true, down = true, left = true;

     for (int i = 1; i < 8; i++) {
          if (top == true &&
               (checkColor(board[position - i*ROW]) == -turn ||
                    board[position - i*ROW] == EMPTYSQUARE)) {
               addMove(position, position - i*ROW, NORMAL, moveList, moveCount);
               if (checkColor(board[position - i*ROW]) == -turn) {
                    top = false;
               }
          }
          else { top = false; }

          if (right == true &&
               (checkColor(board[position + i*COLUMN]) == -turn ||
                    board[position + i*COLUMN] == EMPTYSQUARE)) {
               addMove(position, position + i*COLUMN, NORMAL, moveList, moveCount);
               if (checkColor(board[position + i*COLUMN]) == -turn) {
                    right = false;
               }
          }
          else { right = false; }

          if (down == true &&
               (checkColor(board[position + i*ROW]) == -turn ||
                    board[position + i*ROW] == EMPTYSQUARE)) {
               addMove(position, position + i*ROW, NORMAL, moveList, moveCount);
               if (checkColor(board[position + i*ROW]) == -turn) {
                    down = false;
               }
          }
          else { down = false; }

          if (left == true &&
               (checkColor(board[position - i*COLUMN]) == -turn ||
                    board[position - i*COLUMN] == EMPTYSQUARE)) {
               addMove(position, position - i*COLUMN, NORMAL, moveList, moveCount);
               if (checkColor(board[position - i*COLUMN]) == -turn) {
                    left = false;
               }
          }
          else { left = false; }
     }
}
void queenMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount) {
     rookMoveGeneration(board, turn, position, moveList, moveCount);
     bishopMoveGeneration(board, turn, position, moveList, moveCount);
}
void kingMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount) {
     if (checkColor(board[position + ROW]) == -turn ||
          board[position + ROW] == EMPTYSQUARE) {
          addMove(position, position + ROW, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position - ROW]) == -turn ||
          board[position - ROW] == EMPTYSQUARE) {
          addMove(position, position - ROW, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position + COLUMN]) == -turn ||
          board[position + COLUMN] == EMPTYSQUARE) {
          addMove(position, position + COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position - COLUMN]) == -turn ||
          board[position - COLUMN] == EMPTYSQUARE) {
          addMove(position, position - COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position + ROW + COLUMN]) == -turn ||
          board[position + ROW + COLUMN] == EMPTYSQUARE) {
          addMove(position, position + ROW + COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position + ROW - COLUMN]) == -turn ||
          board[position + ROW - COLUMN] == EMPTYSQUARE) {
          addMove(position, position + ROW - COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position - ROW + COLUMN]) == -turn ||
          board[position - ROW + COLUMN] == EMPTYSQUARE) {
          addMove(position, position - ROW + COLUMN, NORMAL, moveList, moveCount);
     }
     if (checkColor(board[position - ROW - COLUMN]) == -turn ||
          board[position - ROW - COLUMN] == EMPTYSQUARE) {
          addMove(position, position - ROW - COLUMN, NORMAL, moveList, moveCount);
     }
}

void castlingMoveGeneration(int board[120], int turn, int moveList[250][3], int *moveCount, bool castlingCheck[4]) {

     if (turn == WHITE) {
          if (castlingCheck[WKCASTLING] &&                             //  neither piece moved
               board[E1] == WHITEKING && board[H1] == WHITEROOK &&     //  both pieces exists on board
               board[F1] == EMPTYSQUARE && board[G1] == EMPTYSQUARE && //  between them are empty
               squareAttackCheck(board, E1, WHITE) == false &&         //  not in check
               squareAttackCheck(board, F1, WHITE) == false &&         //  not attacked while moving
               squareAttackCheck(board, G1, WHITE) == false) {
              
               addMove(E1, G1, KINGSIDE_CASTLING, moveList, moveCount);
          }
          if (castlingCheck[WQCASTLING] && board[B1] == EMPTYSQUARE &&
               board[C1] == EMPTYSQUARE && board[D1] == EMPTYSQUARE &&
               board[E1] == WHITEKING && board[A1] == WHITEROOK &&
               squareAttackCheck(board, E1, WHITE) == false &&
               squareAttackCheck(board, C1, WHITE) == false &&         
               squareAttackCheck(board, D1, WHITE) == false) {
               addMove(E1, C1, QUEENSIDE_CASTLING, moveList, moveCount);
          }

     }
     if (turn == BLACK) {
          if (castlingCheck[BKCASTLING] &&                             //  neither piece moved
               board[E8] == BLACKKING && board[H8] == BLACKROOK &&     //  both pieces exists on board
               board[F8] == EMPTYSQUARE && board[G8] == EMPTYSQUARE && //  between them are empty
               squareAttackCheck(board, E8, BLACK) == false &&         //  not in check
               squareAttackCheck(board, F8, BLACK) == false &&         //  not attacked while moving
               squareAttackCheck(board, G8, BLACK) == false) {
               
               addMove(E8, G8, KINGSIDE_CASTLING, moveList, moveCount);
          }
          if (castlingCheck[BQCASTLING] && board[B8] == EMPTYSQUARE &&
               board[C8] == EMPTYSQUARE && board[D8] == EMPTYSQUARE &&
               board[E8] == BLACKKING && board[A8] == BLACKROOK &&
               squareAttackCheck(board, E8, BLACK) == false &&
               squareAttackCheck(board, C8, BLACK) == false &&
               squareAttackCheck(board, D8, BLACK) == false) {
               addMove(E8, C8, QUEENSIDE_CASTLING, moveList, moveCount);
          }
     }
}
void promotionMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount) {
     if (turn == WHITE) {
          if (checkColor(board[position - ROW - COLUMN]) == -turn) {
               addPromotionMove(position, position - ROW - COLUMN, moveList, moveCount);
          }
          if (checkColor(board[position - ROW + COLUMN]) == -turn) {
               addPromotionMove(position, position - ROW + COLUMN, moveList, moveCount);
          }
          if (board[position - ROW] == EMPTYSQUARE) {
               addPromotionMove(position, position - ROW, moveList, moveCount);
          }
     }
     if (turn == BLACK) {
          if (checkColor(board[position + ROW - COLUMN]) == -turn) {
               addPromotionMove(position, position + ROW - COLUMN, moveList, moveCount);
          }
          if (checkColor(board[position + ROW + COLUMN]) == -turn) {
               addPromotionMove(position, position + ROW + COLUMN, moveList, moveCount);
          }
          if (board[position + ROW] == EMPTYSQUARE) {
               addPromotionMove(position, position + ROW, moveList, moveCount);
          }
     }
}
void enpassantMoveGeneration(int board[120], int turn, int moveList[250][3], int *moveCount, int enpassantSquare) {
     if (enpassantSquare == 0) { return; }

     if (turn == WHITE) {
          if (board[enpassantSquare + ROW + COLUMN] == WHITEPAWN) {
               addMove(enpassantSquare + ROW + COLUMN, enpassantSquare, ENPASSANT, moveList, moveCount);
          }
          if (board[enpassantSquare + ROW - COLUMN] == WHITEPAWN) {
               addMove(enpassantSquare + ROW - COLUMN, enpassantSquare, ENPASSANT, moveList, moveCount);
          }
     }
     if (turn == BLACK) {
          if (board[enpassantSquare - ROW + COLUMN] == BLACKPAWN) {
               addMove(enpassantSquare - ROW + COLUMN, enpassantSquare, ENPASSANT, moveList, moveCount);
          }
          if (board[enpassantSquare - ROW - COLUMN] == BLACKPAWN) {
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

void legalMoves(int board[120], int turn, int moveList[250][3], int moveCount, int legalMoveList[250][3], int *legalMoveCount) {
     //  find king position
     int kingPosition = 0, changedKingPosition = 0;
     int terminalValue;
     for (int i = 0; i < 120; i++) {
          if (turn == WHITE && board[i] == WHITEKING ||
               turn == BLACK && board[i] == BLACKKING) {
               kingPosition = i;
               break;
          }
     }

     for (int i = 0; i < moveCount; i++) {
          //  check if king will be moved
          if (board[moveList[i][0]] == WHITEKING || board[moveList[i][0]] == BLACKKING) {
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

          //  if king is safe
          if (!squareAttackCheck(board, changedKingPosition, turn)) {
               legalMoveList[*legalMoveCount][0] = moveList[i][0];
               legalMoveList[*legalMoveCount][1] = moveList[i][1];
               legalMoveList[*legalMoveCount][2] = moveList[i][2];
               *legalMoveCount += 1;
          }

          //  undo move
          undoMove(board, moveList[i], terminalValue);
     }
}
bool squareAttackCheck(int board[120], int position, int turn) {
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

          //  5. queen: added to bishop and rook
          //  6. king: is it needed?

          return false;
     }

     if (turn == BLACK) {
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

          //  5. queen: added to bishop and rook
          //  6. king: is it needed?

          return false;
     }

     printf("squareAttackCheck unreachable error\n");
     return true;

}


/*                             RECURSION FUNCTIONS                             */
u64 divide(int depth, int turn, int maxDepth, bool castlingCheck[4], bool showOutput) {

     if (depth == 0) { return 1; }

     depthAllMoveCount[depth] = 0;
     depthLegalMoveCount[depth] = 0;
     depthEnpassantSquare[depth - 1] = 0;

     u64 node = 0, individualNode = 0;
     int terminalValue;
     bool copyCastlingCheck[4];

     // MOVEGEN
     moveGeneration(currentBoard, turn, depthAllMoveList[depth], &depthAllMoveCount[depth], depthEnpassantSquare[depth], castlingCheck);
     // CHECK FOR LEGALS
     legalMoves(currentBoard, turn, depthAllMoveList[depth], depthAllMoveCount[depth], depthLegalMoveList[depth], &depthLegalMoveCount[depth]);

     if (depth == 1) { return depthLegalMoveCount[depth]; }

     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {
          for (int j = 0; j < 4; j++) { copyCastlingCheck[j] = castlingCheck[j]; }

          if (currentBoard[depthLegalMoveList[depth][i][0]] == WHITEKING) {
               copyCastlingCheck[WKCASTLING] = false;
               copyCastlingCheck[WQCASTLING] = false;
          }
          if (currentBoard[depthLegalMoveList[depth][i][0]] == BLACKKING) {
               copyCastlingCheck[BKCASTLING] = false;
               copyCastlingCheck[BQCASTLING] = false;
          }
          if (currentBoard[depthLegalMoveList[depth][i][0]] == WHITEROOK) {
               if (depthLegalMoveList[depth][i][0] == A1) {
                    copyCastlingCheck[WQCASTLING] = false;
               }
               if (depthLegalMoveList[depth][i][0] == H1) {
                    copyCastlingCheck[WKCASTLING] = false;
               }
          }
          if (currentBoard[depthLegalMoveList[depth][i][0]] == BLACKROOK) {
               if (depthLegalMoveList[depth][i][0] == A8) {
                    copyCastlingCheck[BQCASTLING] = false;
               }
               if (depthLegalMoveList[depth][i][0] == H8) {
                    copyCastlingCheck[BKCASTLING] = false;
               }
          }

          terminalValue = makeMove(currentBoard, depthLegalMoveList[depth][i]);

          if (depthLegalMoveList[depth][i][2] == DOUBLEMOVE) {
               depthEnpassantSquare[depth - 1] = terminalValue;
               //  this terminal value is actually enpassantSquare
          }
          else { // if not, revert it back to 0
               depthEnpassantSquare[depth - 1] = 0;
          }

          node += divide(depth - 1, -turn, maxDepth, copyCastlingCheck, showOutput);
          if (showOutput) {
               individualNode = divide(depth - 1, -turn, maxDepth, copyCastlingCheck, false);
          }
          
          if (depth >= maxDepth && showOutput) {
               for (int i = 0; i < 3-depth; i++) { printf("  "); }
               printf("%c%d%c%d: %llu", numberToFile(depthLegalMoveList[depth][i][0]), numberToRank(depthLegalMoveList[depth][i][0]),
                    numberToFile(depthLegalMoveList[depth][i][1]), numberToRank(depthLegalMoveList[depth][i][1]), individualNode);
               printf("\n");
          }

          undoMove(currentBoard, depthLegalMoveList[depth][i], terminalValue);
     }
     return node;

}
u64 divide2(int depth, int turn, int maxDepth, bool castlingCheck[4], bool showOutput) {

     if (depth == 0) { return 1; }

     //  output text file for large output
     ofstream output;
     output.open("output.txt");

     depthAllMoveCount[depth] = 0;
     depthLegalMoveCount[depth] = 0;
     depthEnpassantSquare[depth - 1] = 0;

     u64 node = 0, individualNode = 0;
     int terminalValue;
     bool copyCastlingCheck[4];

     // MOVEGEN
     moveGeneration(currentBoard, turn, depthAllMoveList[depth], &depthAllMoveCount[depth], depthEnpassantSquare[depth], castlingCheck);
     // CHECK FOR LEGALS
     legalMoves(currentBoard, turn, depthAllMoveList[depth], depthAllMoveCount[depth], depthLegalMoveList[depth], &depthLegalMoveCount[depth]);

     //if (depth == 1) { return depthLegalMoveCount[depth]; }

     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {
          for (int j = 0; j < 4; j++) { copyCastlingCheck[j] = castlingCheck[j]; }

          if (currentBoard[depthLegalMoveList[depth][i][0]] == WHITEKING) {
               copyCastlingCheck[WKCASTLING] = false;
               copyCastlingCheck[WQCASTLING] = false;
          }
          if (currentBoard[depthLegalMoveList[depth][i][0]] == BLACKKING) {
               copyCastlingCheck[BKCASTLING] = false;
               copyCastlingCheck[BQCASTLING] = false;
          }
          if (currentBoard[depthLegalMoveList[depth][i][0]] == WHITEROOK) {
               if (depthLegalMoveList[depth][i][0] == A1) {
                    copyCastlingCheck[WQCASTLING] = false;
               }
               if (depthLegalMoveList[depth][i][0] == H1) {
                    copyCastlingCheck[WKCASTLING] = false;
               }
          }
          if (currentBoard[depthLegalMoveList[depth][i][0]] == BLACKROOK) {
               if (depthLegalMoveList[depth][i][0] == A8) {
                    copyCastlingCheck[BQCASTLING] = false;
               }
               if (depthLegalMoveList[depth][i][0] == H8) {
                    copyCastlingCheck[BKCASTLING] = false;
               }
          }

          terminalValue = makeMove(currentBoard, depthLegalMoveList[depth][i]);

          if (depthLegalMoveList[depth][i][2] == DOUBLEMOVE) {
               depthEnpassantSquare[depth - 1] = terminalValue;
               //  this terminal value is actually enpassantSquare
          }
          else { // if not, revert it back to 0
               depthEnpassantSquare[depth - 1] = 0;
          }

          node += divide(depth - 1, -turn, maxDepth, copyCastlingCheck, showOutput);
          if (showOutput) {
               individualNode = divide(depth - 1, -turn, maxDepth, copyCastlingCheck, false);
          }

          if (depth >= maxDepth && showOutput) {
               output << numberToFile(depthLegalMoveList[depth][i][0]) << numberToRank(depthLegalMoveList[depth][i][0]) <<
                    numberToFile(depthLegalMoveList[depth][i][1]) << numberToRank(depthLegalMoveList[depth][i][1]) << ": " << individualNode << std::endl;
          }

          undoMove(currentBoard, depthLegalMoveList[depth][i], terminalValue);
     }
     return node;
     output.close();
}

int makeMove(int board[120], int move[3]) {
     int terminalValue;
     int initial = move[0], terminal = move[1], moveType = move[2];
     if (moveType == NORMAL) {
          terminalValue = board[terminal];
          board[terminal] = board[initial];
          board[initial] = EMPTYSQUARE;
          return terminalValue;
     }
     if (moveType == DOUBLEMOVE) {
          board[terminal] = board[initial];
          board[initial] = EMPTYSQUARE;
          return (terminal+initial)/2;
     }
     else if (moveType == QUEENSIDE_CASTLING) {
          //  move king
          board[terminal] = board[initial];
          board[initial] = EMPTYSQUARE;
          //  move rook
          board[terminal + COLUMN] = board[initial - 4 * COLUMN];
          board[initial - 4 * COLUMN] = EMPTYSQUARE;
          //  castling does not involve capture
          return 0;
     }
     else if (moveType == KINGSIDE_CASTLING) {
          //  move king
          board[terminal] = board[initial];
          board[initial] = EMPTYSQUARE;
          //  move rook
          board[terminal - COLUMN] = board[terminal + COLUMN];
          board[terminal + COLUMN] = EMPTYSQUARE;
          //  castling does not involve capture
          return 0;
     }
     else if (moveType == KNIGHT_PROMOTION) {
          terminalValue = board[terminal];

          //  white turn
          if (board[initial] == WHITEPAWN) {
               board[terminal] = WHITEKNIGHT;
          }
          //  black turn
          else {
               board[terminal] = BLACKKNIGHT;
          }
          board[initial] = EMPTYSQUARE;
          return terminalValue;
     }
     else if (moveType == BISHOP_PROMOTION) {
          terminalValue = board[terminal];

          //  white turn
          if (board[initial] == WHITEPAWN) {
               board[terminal] = WHITEBISHOP;
          }
          //  black turn
          else {
               board[terminal] = BLACKBISHOP;
          }
          board[initial] = EMPTYSQUARE;
          return terminalValue;
     }
     else if (moveType == ROOK_PROMOTION) {
          terminalValue = board[terminal];

          //  white turn
          if (board[initial] == WHITEPAWN) {
               board[terminal] = WHITEROOK;
          }
          //  black turn
          else {
               board[terminal] = BLACKROOK;
          }
          board[initial] = EMPTYSQUARE;
          return terminalValue;
     }
     else if (moveType == QUEEN_PROMOTION) {
          terminalValue = board[terminal];

          //  white turn
          if (board[initial] == WHITEPAWN) {
               board[terminal] = WHITEQUEEN;
          }
          //  black turn
          else {
               board[terminal] = BLACKQUEEN;
          }
          board[initial] = EMPTYSQUARE;
          return terminalValue;
     }
     else if (moveType == ENPASSANT) {
          //  White turn
          if (board[initial] == WHITEPAWN) {
               board[terminal] = board[initial];
               board[initial] = EMPTYSQUARE;
               board[terminal + ROW] = EMPTYSQUARE;
               return BLACKPAWN;
          }
          //  Black turn
          else {
               board[terminal] = board[initial];
               board[initial] = EMPTYSQUARE;
               board[terminal - ROW] = EMPTYSQUARE;
               return WHITEPAWN;
          }
     }
     else {
          printf("makeMove unreachable error\n");
          return 0;
     }
}
void undoMove(int board[120], int move[3], int terminalValue) {
     int initial = move[0], terminal = move[1], moveType = move[2];
     if (moveType == NORMAL) {
          board[initial] = board[terminal];
          board[terminal] = terminalValue;
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
          board[terminal + COLUMN] = board[terminal - COLUMN];
          board[terminal - COLUMN] = EMPTYSQUARE;
     }
     else if (moveType == KNIGHT_PROMOTION || moveType == BISHOP_PROMOTION ||
          moveType == ROOK_PROMOTION || moveType == QUEEN_PROMOTION) {
          //  white turn
          if (checkColor(board[terminal]) == WHITE) {
               board[terminal] = terminalValue;
               board[initial] = WHITEPAWN;
          }
          //  black turn
          else {
               board[terminal] = terminalValue;
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



/******************************************************************************/
/*                               MAIN FUNCTION                                */
/******************************************************************************/
void main() {

     //  Initialize Board
     board120Setup();
     //  FENboardSetup(currentBoard, "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
     

     //  FEN source:
     //  https://chessprogramming.wikispaces.com/Perft+Results
     //  - Position 1: Perft 6 Correct
     //  - Position 2: Perft 5 Correct
     //  - Position 3: Perft 6 Correct, Perft 7 makeMove unreachable error, doesn't end
     //  - Position 4: Perft 5 Correct
     //  - Position 5: Perft 5 Correct
     //  - Position 6: Perft 5 Correct

     printBoard(currentBoard);
     printf("--------------------------------------------------\n");
     printf("White Kingside Castling: %d\n", whiteKingsideCastling);
     printf("White Queenside Castling: %d\n", whiteQueensideCastling);
     printf("Black Kingside Castling: %d\n", blackKingsideCastling);
     printf("Black Queenside Castling: %d\n", blackQueensideCastling);
     printf("En passant Square: %d\n", enpassantSquare);
     printf("Move number: %d\n", moveNumber);
     if (currentTurn == WHITE) { printf("Turn: White\n"); }
     else { printf("Turn: Black\n"); }
     boardToFEN(currentBoard, currentTurn, whiteKingsideCastling, whiteQueensideCastling, blackKingsideCastling, blackQueensideCastling, enpassantSquare, halfMoveClock, moveNumber);
     printf("--------------------------------------------------\n");

     

     bool castlingCheck[4];
     castlingCheck[WKCASTLING] = whiteKingsideCastling;
     castlingCheck[WQCASTLING] = whiteQueensideCastling;
     castlingCheck[BKCASTLING] = blackKingsideCastling;
     castlingCheck[BQCASTLING] = blackQueensideCastling;

     //  begin timer
     int timerIndex = 1;
     frequency = startTimer(&beginTime, timerIndex);

     //  Game Loop
     ///*
     while (gamePlaying) {
     
          // copy ep Square: needs to be done before any recursion
          depthEnpassantSquare[EVAL_DEPTH] = enpassantSquare; 

          printf("Current Board Eval: %d\n", boardEvaluation(currentBoard));
          int minimaxValue = blueValue(EVAL_DEPTH, currentTurn, castlingCheck);
          printf("Minimax Value: %d\n", minimaxValue);

          // Print best moves and result
          for (int i = EVAL_DEPTH; i > 0; i--) {
               printf("%d: %c%d %c%d (%d)\n", EVAL_DEPTH + 1 - i, numberToFile(depthBestMoves[i][0]), numberToRank(depthBestMoves[i][0]), numberToFile(depthBestMoves[i][1]), numberToRank(depthBestMoves[i][1]), depthBestMoves[i][2]);
          }
          
          //  Increment or reset Fifty move count
          //  TODO: Add 50 Move Rule option in move generation / selection
          if (currentBoard[depthBestMoves[EVAL_DEPTH][1]] == EMPTYSQUARE
               && currentBoard[depthBestMoves[EVAL_DEPTH][0]] != WHITEPAWN
               && currentBoard[depthBestMoves[EVAL_DEPTH][0]] != BLACKPAWN) {
               fiftyMoveCount++;
          }
          else { fiftyMoveCount = 0; }

          //  Make best move and print board
          makeMove(currentBoard, depthBestMoves[EVAL_DEPTH]);
          printBoard(currentBoard);

          //  Update enpassant square
          if (depthBestMoves[EVAL_DEPTH][2] == DOUBLEMOVE) {
               enpassantSquare = (depthBestMoves[EVAL_DEPTH][0] + depthBestMoves[EVAL_DEPTH][1]) / 2;
          }
          else { enpassantSquare = 0; }

          //  Check endgame
          if (!endGame) {
               //  if no queens are on the board
               int queenCount = 0;
               for (int i = 0; i < 120; i++) {
                    if (currentBoard[i] == WHITEQUEEN || currentBoard[i] == BLACKQUEEN) {
                         queenCount++;
                    }
               }
               if (queenCount == 0) {
                    endGame = true;
               }
          }
          if (!endGame) { printf("NOT ENDGAME\n"); }
          else { printf("ENDGAME\n"); }

          //  Print out move and move number
          printf("%d: %c%d %c%d (%d)\n", moveNumber, numberToFile(depthBestMoves[EVAL_DEPTH][0]), numberToRank(depthBestMoves[EVAL_DEPTH][0]), numberToFile(depthBestMoves[EVAL_DEPTH][1]), numberToRank(depthBestMoves[EVAL_DEPTH][1]), depthBestMoves[EVAL_DEPTH][2]);

          //  Change turns and increment move
          currentTurn = -currentTurn;
          if (currentTurn == WHITE) { moveNumber++; }

          //  Check if game is over
          gamePlaying = !checkGameEnd(currentBoard);
          if (!gamePlaying) { break; }

          //  TODO: Check Threefold repetition

          
          //  75 Move Rule Implement (unless checkmate)
          if (fiftyMoveCount >= 75) {
               gamePlaying = false;
               gameResult = TIE;
               break;
          }
     }

     //  Output Game Result
     printf("Game Result : ");
     switch (gameResult) {
     case BLACK_WIN:
          printf("0-1\n");
          break;
     case TIE:
          printf("1/2-1/2\n");
          break;
     case WHITE_WIN:
          printf("1-0\n");
          break;
     }
     //*/
 
     

     
     // PERFT TEST
     /*
     printf("PERFT TEST (DEPTH 1) : %llu \n", divide(1, currentTurn, 0, castlingCheck, false));
     printf("PERFT TEST (DEPTH 2) : %llu \n", divide(2, currentTurn, 0, castlingCheck, false));
     printf("PERFT TEST (DEPTH 3) : %llu \n", divide(3, currentTurn, 0, castlingCheck, false));
     printf("PERFT TEST (DEPTH 4) : %llu \n", divide(4, currentTurn, 0, castlingCheck, false));
     printf("PERFT TEST (DEPTH 5) : %llu \n", divide(5, currentTurn, 0, castlingCheck, false));
     printf("PERFT TEST (DEPTH 6) : %llu \n", divide(6, currentTurn, 0, castlingCheck, false));
     printf("PERFT TEST (DEPTH 7) : %llu \n", divide(7, currentTurn, 0, castlingCheck, false));
     */

     //  stop timer
     stopTimer(&endTime, timerIndex);
     //  print elapsed time
     printElapsedTime(beginTime, endTime, frequency, timerIndex);
}