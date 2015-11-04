/*                                    DEFINITION                              */
#define PAWNVALUE 100
#define KNIGHTVALUE 300
#define BISHOPVALUE 300
#define ROOKVALUE 500
#define QUEENVALUE 900
#define KINGVALUE 9999
#define ROW 10
#define COLUMN 1
#define MAXIMUM_DEPTH 6
#include <stdio.h>
#include <string>
typedef unsigned long long u64;

/*                              FUNCTION DECLARATION                          */
//  This function sets up currentboard[120] for the initial position of pieces.
void board120Setup();
//  This functions prints the board from the parameter.
void printBoard(int board[120]);
//  Returns evaluation score based on parameter board given.
void updateEvaluaton(int board[120]);
//  Gets a position number and returns the reversed position number
int reversePosition(int position);
//  Gets position based on 64 and returns position based on 120
int position64to120(int position64);
//  Gets position based on 120 and returns position based on 64
int position120to64(int position120);
//  Generates all moves possible by the current turn
void moveGeneration(int board[120], int turn, int normalMoveList[1000][2], int *normalMoveCount);
//  Gets a piece and returns the color of the piece
int checkColor(int pieceType);
//  functions for each piece move generation & special move generation
void pawnMoveGeneration(int board[120], int turn, int position, int normalMoveList[1000][2], int *normalMoveCount);
void knightMoveGeneration(int board[120], int turn, int position, int normalMoveList[1000][2], int *normalMoveCount);
void bishopMoveGeneration(int board[120], int turn, int position, int normalMoveList[1000][2], int *normalMoveCount);
void rookMoveGeneration(int board[120], int turn, int position, int normalMoveList[1000][2], int *normalMoveCount);
void queenMoveGeneration(int board[120], int turn, int position, int normalMoveList[1000][2], int *normalMoveCount);
void kingMoveGeneration(int board[120], int turn, int position, int normalMoveList[1000][2], int *normalMoveCount);
void castlingMoveGeneration(int board[120], int turn);
void promotionMoveGeneration(int board[120], int turn, int position);
//  Add the input move to the array
void addMove(int initial, int terminal, int normalMoveList[1000][2], int *normalMoveCount);
void addPromotionMove(int initial, int terminal, int turn);
//  checks if a move is legal or not
void legalMoves(int board[120], int turn);
//  receives a FEN string to setup board
void FENboardSetup(std::string FEN);
//  performance test function to test move generation
u64 perft(int depth);
//  make and undo move (used for recursion
int makeMove(int board[120], int move[2]);
void undoMove(int board[120], int move[2], int terminalValue);
void makeCastlingMove(int board[120], int move[2]);
void undoCastlingMove(int board[120], int move[2]);
int makePromotionMove(int board[120], int move[2]);
void undoPromotionMove(int board[120], int move[2], int terminalValue);
void makeEnpassantgMove(int board[120], int move[2]);
void undoEnPassantMove(int board[120], int move[2]);
//  test Output used for debugging
void testOutput();

/*                                   ENUMERATION                              */
enum squareType {
     EMPTYSQUARE,
     WHITEPAWN, WHITEKNIGHT, WHITEBISHOP, WHITEROOK, WHITEQUEEN, WHITEKING,
     BLACKPAWN, BLACKKNIGHT, BLACKBISHOP, BLACKROOK, BLACKQUEEN, BLACKKING,
     ERRORSQUARE
};
enum fileRank120 {
     A8=21, B8, C8, D8, E8, F8, G8, H8,
     A7=31, B7, C7, D7, E7, F7, G7, H7,
     A6=41, B6, C6, D6, E6, F6, G6, H6,
     A5=51, B5, C5, D5, E5, F5, G5, H5,
     A4=61, B4, C4, D4, E4, F4, G4, H4,
     A3=71, B3, C3, D3, E3, F3, G3, H3,
     A2=81, B2, C2, D2, E2, F2, G2, H2,
     A1=91, B1, C1, D1, E1, F1, G1, H1
};
enum color {WHITE, BLACK, NEITHER};

/*                                 GLOBAL VARIABLE                            */
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
int currentTurn; 
//  First number: initial square, Second number: terminal square
int allNormalMoves[1000][2];
//  initial, terminal, piecetype
int promotionMoves[88][3]; 
int normalMoveCount = 0;
int promotionMoveCount = 0;
//  0 if double move did not happen, square value (ex. F3) otherwise
int enpassantSquare = 0; 
int enpassantMoves[2][2];
int enpassantMoveCount = 0;
int castlingMoves[2][2];
int castlingMoveCount = 0;
//  All moves after checked legal
int allLegalNormalMoves[1000][2];
int legalNormalMoveCount = 0;
//  added for recursion
int depthNormalMoveList[MAXIMUM_DEPTH + 1][1000][2];
int depthNormalMoveCount[MAXIMUM_DEPTH + 1];
int depthPromotionMoveList[MAXIMUM_DEPTH + 1][2][2];
int depthPromotionMoveCount[MAXIMUM_DEPTH + 1];
int depthEnpassantMoveList[MAXIMUM_DEPTH + 1][2][2];
int depthEnpassantMoveCount[MAXIMUM_DEPTH + 1];
int depthCastlingMoveList[MAXIMUM_DEPTH + 1][2][2];
int depthCastlingMoveCount[MAXIMUM_DEPTH + 1];
int depthLegalMoveList[MAXIMUM_DEPTH + 1][1000][2];
int depthLegalMoveCount[MAXIMUM_DEPTH + 1];

/*                                    FUNCTION                                */
void board120Setup() {
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
int updateEvaluation(int board[120]) {
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
void moveGeneration(int board[120], int turn, int normalMoveList[1000][2], int *normalMoveCount) {
     //  normalMoveCount = 0;
     promotionMoveCount = 0;
     enpassantMoveCount = 0;
     
     if (turn == WHITE) {
          for (int i = 0; i < 120; i++) {
               switch (board[i]) {
               case WHITEPAWN:
                    pawnMoveGeneration(board, turn, i, normalMoveList, normalMoveCount);
                    break;
               case WHITEKNIGHT:
                    knightMoveGeneration(board, turn, i, normalMoveList, normalMoveCount);
                    break;
               case WHITEBISHOP:
                    bishopMoveGeneration(board, turn, i, normalMoveList, normalMoveCount);
                    break;
               case WHITEROOK:
                    rookMoveGeneration(board, turn, i, normalMoveList, normalMoveCount);
                    break;
               case WHITEQUEEN:
                    queenMoveGeneration(board, turn, i, normalMoveList, normalMoveCount);
                    break;
               case WHITEKING:
                    kingMoveGeneration(board, turn, i, normalMoveList, normalMoveCount);
                    break;
               }
          }
     }
     if (turn == BLACK) {
          for (int i = 0; i < 120; i++) {
               switch (board[i]) {
               case BLACKPAWN:
                    pawnMoveGeneration(board, turn, i, normalMoveList, normalMoveCount);
                    break;
               case BLACKKNIGHT:
                    knightMoveGeneration(board, turn, i, normalMoveList, normalMoveCount);
                    break;
               case BLACKBISHOP:
                    bishopMoveGeneration(board, turn, i, normalMoveList, normalMoveCount);
                    break;
               case BLACKROOK:
                    rookMoveGeneration(board, turn, i, normalMoveList, normalMoveCount);
                    break;
               case BLACKQUEEN:
                    queenMoveGeneration(board, turn, i, normalMoveList, normalMoveCount);
                    break;
               case BLACKKING:
                    kingMoveGeneration(board, turn, i, normalMoveList, normalMoveCount);
                    break;
               }
          }
     }
     
     castlingMoveGeneration(board, turn);
}
int checkColor(int pieceType) {
     if (WHITEPAWN <= pieceType && pieceType <= WHITEKING) {
          return WHITE;
     }
     if (BLACKPAWN <= pieceType && pieceType <= BLACKKING) {
          return BLACK;
     }
     if (pieceType == EMPTYSQUARE || pieceType == ERRORSQUARE) {
          return NEITHER;
     }
}
void pawnMoveGeneration(int board[120], int turn, int position, int normalMoveList[1000][2], int *normalMoveCount) {
     if (turn == WHITE) {
          //  if on the last row before promotion, just call promotion
          if (A7 <= position && position <= H7) {
               promotionMoveGeneration(board, turn, position);
               return;
          }

          //  Advance 1 square
          if (board[position - ROW] == EMPTYSQUARE) {
               addMove(position, position - ROW, normalMoveList, normalMoveCount);
               //  Advance 2 squares
               if (A2 <= position && position <= H2 &&
                    board[position - 2 * ROW] == EMPTYSQUARE) {
                    addMove(position, position - 2 * ROW, normalMoveList, normalMoveCount);
               }
          }

          //  attack diagonals
          if (checkColor(board[position - ROW - COLUMN]) == BLACK) {
               addMove(position, position - ROW - COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - ROW + COLUMN]) == BLACK) {
               addMove(position, position - ROW + COLUMN, normalMoveList, normalMoveCount);
          }
         
          //  en passant
          if (enpassantSquare == position - ROW - COLUMN) {
               enpassantMoves[enpassantMoveCount][0] = position;
               enpassantMoves[enpassantMoveCount][1] = enpassantSquare;
               enpassantMoveCount++;
          }
          if (enpassantSquare == position - ROW + COLUMN) {
               enpassantMoves[enpassantMoveCount][0] = position;
               enpassantMoves[enpassantMoveCount][1] = enpassantSquare;
               enpassantMoveCount++;
          }
     }
     if (turn == BLACK) {
          //  if on the last row before promotion, just call promotion
          if (A2 <= position && position <= H2) {
               promotionMoveGeneration(board, turn, position);
               return;
          }
          
          //  Advance 1 square
          if (board[position + ROW] == EMPTYSQUARE) {
               addMove(position, position + ROW, normalMoveList, normalMoveCount);
               //  Advance 2 squares
               if (A7 <= position && position <= H7 &&
                    board[position + 2 * ROW] == EMPTYSQUARE) {
                    addMove(position, position + 2 * ROW, normalMoveList, normalMoveCount);
               }
          }

          //  attack diagonals
          if (checkColor(board[position + ROW - COLUMN]) == WHITE) {
               addMove(position, position - ROW - COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + ROW + COLUMN]) == WHITE) {
               addMove(position, position - ROW + COLUMN, normalMoveList, normalMoveCount);
          }

          //  en passant
          if (enpassantSquare == position + ROW - COLUMN) {
               enpassantMoves[enpassantMoveCount][0] = position;
               enpassantMoves[enpassantMoveCount][1] = enpassantSquare;
               enpassantMoveCount++;
          }
          if (enpassantSquare == position + ROW + COLUMN) {
               enpassantMoves[enpassantMoveCount][0] = position;
               enpassantMoves[enpassantMoveCount][1] = enpassantSquare;
               enpassantMoveCount++;
          }
     }
}
void knightMoveGeneration(int board[120], int turn, int position, int normalMoveList[1000][2], int *normalMoveCount) {
     if (turn == WHITE) {
          if (checkColor(board[position + ROW + 2 * COLUMN]) == BLACK ||
              board[position + ROW + 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position + ROW + 2 * COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + ROW - 2 * COLUMN]) == BLACK ||
               board[position + ROW - 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position + ROW - 2 * COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - ROW + 2 * COLUMN]) == BLACK ||
               board[position - ROW + 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position - ROW + 2 * COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - ROW - 2 * COLUMN]) == BLACK ||
               board[position - ROW - 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position - ROW - 2 * COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + 2 * ROW + COLUMN]) == BLACK ||
               board[position + 2 * ROW + COLUMN] == EMPTYSQUARE) {
               addMove(position, position + 2 * ROW + COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + 2 * ROW - COLUMN]) == BLACK ||
               board[position + 2 * ROW - COLUMN] == EMPTYSQUARE) {
               addMove(position, position + 2 * ROW - COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - 2 * ROW + COLUMN]) == BLACK ||
               board[position - 2 * ROW + COLUMN] == EMPTYSQUARE) {
               addMove(position, position - 2 * ROW + COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - 2 * ROW - COLUMN]) == BLACK ||
               board[position - 2 * ROW - COLUMN] == EMPTYSQUARE) {
               addMove(position, position - 2 * ROW - COLUMN, normalMoveList, normalMoveCount);
          }
     }
     if (turn == BLACK) {
          if (checkColor(board[position + ROW + 2 * COLUMN]) == WHITE ||
               board[position + ROW + 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position + ROW + 2 * COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + ROW - 2 * COLUMN]) == WHITE ||
               board[position + ROW - 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position + ROW - 2 * COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - ROW + 2 * COLUMN]) == WHITE ||
               board[position - ROW + 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position - ROW + 2 * COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - ROW - 2 * COLUMN]) == WHITE ||
               board[position - ROW - 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position - ROW - 2 * COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + 2 * ROW + COLUMN]) == WHITE ||
               board[position + 2 * ROW + COLUMN] == EMPTYSQUARE) {
               addMove(position, position + 2 * ROW + COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + 2 * ROW - COLUMN]) == WHITE ||
               board[position + 2 * ROW - COLUMN] == EMPTYSQUARE) {
               addMove(position, position + 2 * ROW - COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - 2 * ROW + COLUMN]) == WHITE ||
               board[position - 2 * ROW + COLUMN] == EMPTYSQUARE) {
               addMove(position, position - 2 * ROW + COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - 2 * ROW - COLUMN]) == WHITE ||
               board[position - 2 * ROW - COLUMN] == EMPTYSQUARE) {
               addMove(position, position - 2 * ROW - COLUMN, normalMoveList, normalMoveCount);
          }
     }
}
void bishopMoveGeneration(int board[120], int turn, int position, int normalMoveList[1000][2], int *normalMoveCount) {
     bool topright = true, downright = true, downleft = true, topleft = true;
     if (turn == WHITE) {
          for (int i = 1; i < 8; i++) {

               if (topright == true &&
                    (checkColor(board[position - i*ROW + i*COLUMN]) == BLACK ||
                         board[position -  i*ROW + i*COLUMN] == EMPTYSQUARE)) {
                    addMove(position, position - i*ROW + i*COLUMN, normalMoveList, normalMoveCount);
                    if (checkColor(board[position - i*ROW + i*COLUMN]) == BLACK) {
                         topright = false;
                    }
               }
               else { topright = false; }

               if (downright == true &&
                    (checkColor(board[position + i*ROW + i*COLUMN]) == BLACK ||
                         board[position + i*ROW + i*COLUMN] == EMPTYSQUARE)) {
                    addMove(position, position + i*ROW + i*COLUMN, normalMoveList, normalMoveCount);
                    if (checkColor(board[position + i*ROW + i*COLUMN]) == BLACK) {
                         downright = false;
                    }
               }
               else { downright = false; }

               if (downleft == true &&
                    (checkColor(board[position + i*ROW - i*COLUMN]) == BLACK ||
                         board[position + i*ROW - i*COLUMN] == EMPTYSQUARE)) {
                    addMove(position, position + i*ROW - i*COLUMN, normalMoveList, normalMoveCount);
                    if (checkColor(board[position + i*ROW - i*COLUMN]) == BLACK) {
                         downleft = false;
                    }
               }
               else { downleft = false; }

               if (topleft == true &&
                    (checkColor(board[position - i*ROW - i*COLUMN]) == BLACK ||
                         board[position - i*ROW - i*COLUMN] == EMPTYSQUARE)) {
                    addMove(position, position - i*ROW - i*COLUMN, normalMoveList, normalMoveCount);
                    if (checkColor(board[position - i*ROW - i*COLUMN]) == BLACK) {
                         topleft = false;
                    }
               }
               else { topleft = false; }
          }
     }
     if (turn == BLACK) {
          for (int i = 1; i < 8; i++) {

               if (topright == true &&
                    (checkColor(board[position - i*ROW + i*COLUMN]) == WHITE ||
                         board[position - i*ROW + i*COLUMN] == EMPTYSQUARE)) {
                    addMove(position, position - i*ROW + i*COLUMN, normalMoveList, normalMoveCount);
                    if (checkColor(board[position - i*ROW + i*COLUMN]) == WHITE) {
                         topright = false;
                    }
               }
               else { topright = false; }

               if (downright == true &&
                    (checkColor(board[position + i*ROW + i*COLUMN]) == WHITE ||
                         board[position + i*ROW + i*COLUMN] == EMPTYSQUARE)) {
                    addMove(position, position + i*ROW + i*COLUMN, normalMoveList, normalMoveCount);
                    if (checkColor(board[position + i*ROW + i*COLUMN]) == WHITE) {
                         downright = false;
                    }
               }
               else { downright = false; }

               if (downleft == true &&
                    (checkColor(board[position + i*ROW - i*COLUMN]) == WHITE ||
                         board[position + i*ROW - i*COLUMN] == EMPTYSQUARE)) {
                    addMove(position, position + i*ROW - i*COLUMN, normalMoveList, normalMoveCount);
                    if (checkColor(board[position + i*ROW - i*COLUMN]) == WHITE) {
                         downleft = false;
                    }
               }
               else { downleft = false; }

               if (topleft == true &&
                    (checkColor(board[position - i*ROW - i*COLUMN]) == WHITE ||
                         board[position - i*ROW - i*COLUMN] == EMPTYSQUARE)) {
                    addMove(position, position - i*ROW - i*COLUMN, normalMoveList, normalMoveCount);
                    if (checkColor(board[position - i*ROW - i*COLUMN]) == WHITE) {
                         topleft = false;
                    }
               }
               else { topleft = false; }
          }
     }
}
void rookMoveGeneration(int board[120], int turn, int position, int normalMoveList[1000][2], int *normalMoveCount) {
     bool top = true, right = true, down = true, left = true;
     
     if (turn == WHITE) {
          for (int i = 1; i < 8; i++) {
               if (top == true &&
                    (checkColor(board[position - i*ROW]) == BLACK ||
                         board[position - i*ROW] == EMPTYSQUARE)) {
                    addMove(position, position - i*ROW, normalMoveList, normalMoveCount);
                    if (checkColor(board[position - i*ROW]) == BLACK) {
                         top = false;
                    }
               }
               else { top = false; }

               if (right == true &&
                    (checkColor(board[position + i*COLUMN]) == BLACK ||
                         board[position + i*COLUMN] == EMPTYSQUARE)) {
                    addMove(position, position + i*COLUMN, normalMoveList, normalMoveCount);
                    if (checkColor(board[position + i*COLUMN]) == BLACK) {
                         right = false;
                    }
               }
               else { right = false; }

               if (down == true &&
                    (checkColor(board[position + i*ROW]) == BLACK ||
                         board[position + i*ROW] == EMPTYSQUARE)) {
                    addMove(position, position + i*ROW, normalMoveList, normalMoveCount);
                    if (checkColor(board[position + i*ROW]) == BLACK) {
                         down = false;
                    }
               }
               else { down = false; }

               if (left == true &&
                    (checkColor(board[position - i*COLUMN]) == BLACK ||
                         board[position - i*COLUMN] == EMPTYSQUARE)) {
                    addMove(position, position - i*COLUMN, normalMoveList, normalMoveCount);
                    if (checkColor(board[position - i*COLUMN]) == BLACK) {
                         left = false;
                    }
               }
               else { left = false; }
          }
     }
     if (turn == BLACK) {
          for (int i = 1; i < 8; i++) {
               if (top == true &&
                    (checkColor(board[position - i*ROW]) == WHITE ||
                         board[position - i*ROW] == EMPTYSQUARE)) {
                    addMove(position, position - i*ROW, normalMoveList, normalMoveCount);
                    if (checkColor(board[position - i*ROW]) == WHITE) {
                         top = false;
                    }
               }
               else { top = false; }

               if (right == true &&
                    (checkColor(board[position + i*COLUMN]) == WHITE ||
                         board[position + i*COLUMN] == EMPTYSQUARE)) {
                    addMove(position, position + i*COLUMN, normalMoveList, normalMoveCount);
                    if (checkColor(board[position + i*COLUMN]) == WHITE) {
                         right = false;
                    }
               }
               else { right = false; }

               if (down == true &&
                    (checkColor(board[position + i*ROW]) == WHITE ||
                         board[position + i*ROW] == EMPTYSQUARE)) {
                    addMove(position, position + i*ROW, normalMoveList, normalMoveCount);
                    if (checkColor(board[position + i*ROW]) == WHITE) {
                         down = false;
                    }
               }
               else { down = false; }

               if (left == true &&
                    (checkColor(board[position - i*COLUMN]) == WHITE ||
                         board[position - i*COLUMN] == EMPTYSQUARE)) {
                    addMove(position, position - i*COLUMN, normalMoveList, normalMoveCount);
                    if (checkColor(board[position - i*COLUMN]) == WHITE) {
                         left = false;
                    }
               }
               else { left = false; }
          }
     }
}
void queenMoveGeneration(int board[120], int turn, int position, int normalMoveList[1000][2], int *normalMoveCount) {
     rookMoveGeneration(board, turn, position, normalMoveList, normalMoveCount);
     bishopMoveGeneration(board, turn, position, normalMoveList, normalMoveCount);
}
void kingMoveGeneration(int board[120], int turn, int position, int normalMoveList[1000][2], int *normalMoveCount) {
     if (turn == WHITE) {
          if (checkColor(board[position + ROW]) == BLACK || 
              board[position + ROW] == EMPTYSQUARE) {
               addMove(position, position + ROW, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - ROW]) == BLACK ||
              board[position - ROW] == EMPTYSQUARE) {
               addMove(position, position - ROW, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + COLUMN]) == BLACK ||
              board[position + COLUMN] == EMPTYSQUARE) {
               addMove(position, position + COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - COLUMN]) == BLACK ||
              board[position - COLUMN] == EMPTYSQUARE) {
               addMove(position, position - COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + ROW + COLUMN]) == BLACK ||
              board[position + ROW + COLUMN] == EMPTYSQUARE) {
               addMove(position, position + ROW + COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + ROW - COLUMN]) == BLACK ||
              board[position + ROW - COLUMN] == EMPTYSQUARE) {
               addMove(position, position + ROW - COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - ROW + COLUMN]) == BLACK ||
              board[position - ROW + COLUMN] == EMPTYSQUARE) {
               addMove(position, position - ROW + COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - ROW - COLUMN]) == BLACK ||
              board[position - ROW - COLUMN] == EMPTYSQUARE) {
               addMove(position, position - ROW - COLUMN, normalMoveList, normalMoveCount);
          }
     }
     if (turn == BLACK) {
          if (checkColor(board[position + ROW]) == WHITE ||
               board[position + ROW] == EMPTYSQUARE) {
               addMove(position, position + ROW, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - ROW]) == WHITE ||
               board[position - ROW] == EMPTYSQUARE) {
               addMove(position, position - ROW, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + COLUMN]) == WHITE ||
               board[position + COLUMN] == EMPTYSQUARE) {
               addMove(position, position + COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - COLUMN]) == WHITE ||
               board[position - COLUMN] == EMPTYSQUARE) {
               addMove(position, position - COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + ROW + COLUMN]) == WHITE ||
               board[position + ROW + COLUMN] == EMPTYSQUARE) {
               addMove(position, position + ROW + COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position + ROW - COLUMN]) == WHITE ||
               board[position + ROW - COLUMN] == EMPTYSQUARE) {
               addMove(position, position + ROW - COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - ROW + COLUMN]) == WHITE ||
               board[position - ROW + COLUMN] == EMPTYSQUARE) {
               addMove(position, position - ROW + COLUMN, normalMoveList, normalMoveCount);
          }
          if (checkColor(board[position - ROW - COLUMN]) == WHITE ||
               board[position - ROW - COLUMN] == EMPTYSQUARE) {
               addMove(position, position - ROW - COLUMN, normalMoveList, normalMoveCount);
          }
     }
}
void castlingMoveGeneration(int board[120], int turn) {
     //  TODO: Check if king is in check during move
     
     if (turn == WHITE) {
          if (whiteKingsideCastling && 
               board[F1] == EMPTYSQUARE && board[G1] == EMPTYSQUARE) {
               castlingMoves[castlingMoveCount][0] = E1;
               castlingMoves[castlingMoveCount][1] = H1;
               castlingMoveCount++;
          }
          if (whiteQueensideCastling && board[B1] == EMPTYSQUARE &&
               board[C1] == EMPTYSQUARE && board[D1] == EMPTYSQUARE) {
               castlingMoves[castlingMoveCount][0] = E1;
               castlingMoves[castlingMoveCount][1] = A1;
               castlingMoveCount++;
          }

     }
     if (turn == BLACK) {
          if (whiteKingsideCastling &&
               board[F8] == EMPTYSQUARE && board[G8] == EMPTYSQUARE) {
               castlingMoves[castlingMoveCount][0] = E8;
               castlingMoves[castlingMoveCount][1] = H8;
               castlingMoveCount++;
          }
          if (whiteQueensideCastling && board[B8] == EMPTYSQUARE &&
               board[C8] == EMPTYSQUARE && board[D8] == EMPTYSQUARE) {
               castlingMoves[castlingMoveCount][0] = E8;
               castlingMoves[castlingMoveCount][1] = A8;
               castlingMoveCount++;
          }
     }
}
void promotionMoveGeneration(int board[120], int turn, int position) {
     if (turn == WHITE) {
          if (checkColor(board[position - ROW - COLUMN]) == BLACK) {
               addPromotionMove(position, position - ROW - COLUMN, turn);
          }
          if (checkColor(board[position - ROW + COLUMN]) == BLACK) {
               addPromotionMove(position, position - ROW + COLUMN, turn);
          }
          if (board[position - ROW] == EMPTYSQUARE) {
               addPromotionMove(position, position - ROW, turn);
          }
     }
     if (turn == BLACK) {
          if (checkColor(board[position + ROW + COLUMN]) == WHITE) {
               addPromotionMove(position, position + ROW + COLUMN, turn);
          }
          if (checkColor(board[position + ROW - COLUMN]) == WHITE) {
               addPromotionMove(position, position + ROW - COLUMN, turn);
          }
          if (board[position + ROW] == EMPTYSQUARE) {
               addPromotionMove(position, position + ROW, turn);
          }
     }
}
void addMove(int initial, int terminal, int normalMoveList[1000][2], int *normalMoveCount) {
     normalMoveList[*normalMoveCount][0] = initial;
     normalMoveList[*normalMoveCount][1] = terminal;
     *normalMoveCount++;
}
void addPromotionMove(int initial, int terminal, int turn) {
     promotionMoves[promotionMoveCount][0] = initial;
     promotionMoves[promotionMoveCount][1] = terminal;
     promotionMoves[promotionMoveCount + 1][0] = initial;
     promotionMoves[promotionMoveCount + 1][1] = terminal;
     promotionMoves[promotionMoveCount + 2][0] = initial;
     promotionMoves[promotionMoveCount + 2][1] = terminal;
     promotionMoves[promotionMoveCount + 3][0] = initial;
     promotionMoves[promotionMoveCount + 3][1] = terminal;
     if (turn == WHITE) {
          promotionMoves[promotionMoveCount][2] = WHITEKNIGHT;
          promotionMoves[promotionMoveCount + 1][2] = WHITEBISHOP;
          promotionMoves[promotionMoveCount + 2][2] = WHITEQUEEN;
          promotionMoves[promotionMoveCount + 3][2] = WHITEKING;
     }
     if (turn == BLACK) {
          promotionMoves[promotionMoveCount][2] = BLACKKNIGHT;
          promotionMoves[promotionMoveCount + 1][2] = BLACKBISHOP;
          promotionMoves[promotionMoveCount + 2][2] = BLACKQUEEN;
          promotionMoves[promotionMoveCount + 3][2] = BLACKKING;
     }
     promotionMoveCount += 4;
}
void legalMoves(int board[120], int turn) {
     int initialPosition, terminalPosition;
     int terminalValue;
     int kingSquare;
     legalNormalMoveCount = 0;
     bool legal = true;

     //  find the king's location
     for (int i = 0; i < 120; i++) {
          if ((turn == WHITE && board[i] == WHITEKING) || (turn == BLACK && board[i] == BLACKKING)) {
               kingSquare = i;
               break;
          }
     }
     for (int i = 0; i < normalMoveCount; i++) {
          initialPosition = allNormalMoves[i][0];
          terminalPosition = allNormalMoves[i][1];
          terminalValue = EMPTYSQUARE;
          legal = true;
          
          //  make move
          terminalValue = board[terminalPosition];
          board[terminalPosition] = board[initialPosition];
          board[initialPosition] = EMPTYSQUARE;

          if (turn == WHITE && terminalValue == WHITEKING) {
               kingSquare = terminalPosition;
          }
          if (turn == BLACK && terminalValue == BLACKKING) {
               kingSquare = terminalPosition;
          }

          //  check for checks (no pun intended)
          //  turns should be reversed since move is made
          if (turn == WHITE) {
               //  1. pawn
               if (board[kingSquare - ROW - COLUMN] == BLACKPAWN || 
                    board[kingSquare - ROW - COLUMN] == BLACKPAWN) {
                    legal = false;
               }
               //  2. knight
               if (board[kingSquare - ROW - 2*COLUMN] == BLACKKNIGHT ||
                    board[kingSquare - ROW + 2 * COLUMN] == BLACKKNIGHT ||
                    board[kingSquare + ROW - 2 * COLUMN] == BLACKKNIGHT ||
                    board[kingSquare + ROW + 2 * COLUMN] == BLACKKNIGHT ||
                    board[kingSquare - 2 * ROW - COLUMN] == BLACKKNIGHT ||
                    board[kingSquare - 2 * ROW + COLUMN] == BLACKKNIGHT || 
                    board[kingSquare + 2 * ROW - COLUMN] == BLACKKNIGHT || 
                    board[kingSquare + 2 * ROW + COLUMN] == BLACKKNIGHT) {
                    legal = false;
               }
               //  3. bishop
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare - i*ROW - i*COLUMN] == BLACKBISHOP ||
                         board[kingSquare - i*ROW - i*COLUMN] == BLACKQUEEN) {
                         legal = false;
                    }
                    
                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare - i*ROW - i*COLUMN] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare - i*ROW - i*COLUMN] == ERRORSQUARE) {
                         break;
                    }
               }
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare - i*ROW + i*COLUMN] == BLACKBISHOP ||
                         board[kingSquare - i*ROW + i*COLUMN] == BLACKQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare - i*ROW + i*COLUMN] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare - i*ROW + i*COLUMN] == ERRORSQUARE) {
                         break;
                    }
               }
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare + i*ROW - i*COLUMN] == BLACKBISHOP || 
                         board[kingSquare + i*ROW - i*COLUMN] == BLACKQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare + i*ROW - i*COLUMN] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare + i*ROW - i*COLUMN] == ERRORSQUARE) {
                         break;
                    }
               }
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare + i*ROW + i*COLUMN] == BLACKBISHOP ||
                         board[kingSquare + i*ROW + i*COLUMN] == BLACKQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare + i*ROW + i*COLUMN] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare + i*ROW + i*COLUMN] == ERRORSQUARE) {
                         break;
                    }
               }
               //  4. rook
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare - i*ROW] == BLACKROOK ||
                         board[kingSquare - i*ROW] == BLACKQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare - i*ROW] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare - i*ROW] == ERRORSQUARE) {
                         break;
                    }
               }
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare + i*ROW] == BLACKROOK ||
                         board[kingSquare + i*ROW] == BLACKQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare + i*ROW] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare + i*ROW] == ERRORSQUARE) {
                         break;
                    }
               }
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare - i*COLUMN] == BLACKROOK ||
                         board[kingSquare - i*COLUMN] == BLACKQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare - i*COLUMN] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare - i*COLUMN] == ERRORSQUARE) {
                         break;
                    }
               }
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare + i*COLUMN] == BLACKROOK ||
                         board[kingSquare + i*COLUMN] == BLACKQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare + i*COLUMN] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare + i*COLUMN] == ERRORSQUARE) {
                         break;
                    }
               }
               
               //  5. queen: added to bishop and rook
               //  6. king: is it needed?

               //  add this move to legal move array
               if (legal) {
                    allLegalNormalMoves[legalNormalMoveCount][0] = initialPosition;
                    allLegalNormalMoves[legalNormalMoveCount][1] = terminalPosition;
                    legalNormalMoveCount++;
               }
          }

          if (turn == BLACK) {
               //  1. pawn
               if (board[kingSquare - ROW - COLUMN] == WHITEPAWN ||
                    board[kingSquare - ROW - COLUMN] == WHITEPAWN) {
                    legal = false;
               }
               //  2. knight
               if (board[kingSquare - ROW - 2 * COLUMN] == WHITEKNIGHT ||
                    board[kingSquare - ROW + 2 * COLUMN] == WHITEKNIGHT ||
                    board[kingSquare + ROW - 2 * COLUMN] == WHITEKNIGHT ||
                    board[kingSquare + ROW + 2 * COLUMN] == WHITEKNIGHT ||
                    board[kingSquare - 2 * ROW - COLUMN] == WHITEKNIGHT ||
                    board[kingSquare - 2 * ROW + COLUMN] == WHITEKNIGHT ||
                    board[kingSquare + 2 * ROW - COLUMN] == WHITEKNIGHT ||
                    board[kingSquare + 2 * ROW + COLUMN] == WHITEKNIGHT) {
                    legal = false;
               }
               //  3. bishop
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare - i*ROW - i*COLUMN] == WHITEBISHOP ||
                         board[kingSquare - i*ROW - i*COLUMN] == WHITEQUEEN) {
                         legal = false;
                         break;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare - i*ROW - i*COLUMN] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare - i*ROW - i*COLUMN] == ERRORSQUARE) {
                         break;
                    }
               }
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare - i*ROW + i*COLUMN] == WHITEBISHOP ||
                         board[kingSquare - i*ROW + i*COLUMN] == WHITEQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare - i*ROW + i*COLUMN] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare - i*ROW + i*COLUMN] == ERRORSQUARE) {
                         break;
                    }
               }
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare + i*ROW - i*COLUMN] == WHITEBISHOP ||
                         board[kingSquare + i*ROW - i*COLUMN] == WHITEQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare + i*ROW - i*COLUMN] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare + i*ROW - i*COLUMN] == ERRORSQUARE) {
                         break;
                    }
               }
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare + i*ROW + i*COLUMN] == WHITEBISHOP ||
                         board[kingSquare + i*ROW + i*COLUMN] == WHITEQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare + i*ROW + i*COLUMN] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare + i*ROW + i*COLUMN] == ERRORSQUARE) {
                         break;
                    }
               }
               //  4. rook
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare - i*ROW] == WHITEROOK ||
                         board[kingSquare - i*ROW] == WHITEQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare - i*ROW] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare - i*ROW] == ERRORSQUARE) {
                         break;
                    }
               }
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare + i*ROW] == WHITEROOK ||
                         board[kingSquare + i*ROW] == WHITEQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare + i*ROW] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare + i*ROW] == ERRORSQUARE) {
                         break;
                    }
               }
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare - i*COLUMN] == WHITEROOK ||
                         board[kingSquare - i*COLUMN] == WHITEQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare - i*COLUMN] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare - i*COLUMN] == ERRORSQUARE) {
                         break;
                    }
               }
               for (int i = 1; i < 8; i++) {
                    if (board[kingSquare + i*COLUMN] == WHITEROOK ||
                         board[kingSquare + i*COLUMN] == WHITEQUEEN) {
                         legal = false;
                    }

                    //  if some other piece blocks it, no more serach is necessary
                    else if (board[kingSquare + i*COLUMN] != EMPTYSQUARE) {
                         break;
                    }

                    //  also when it reaches the end of the board
                    else if (board[kingSquare + i*COLUMN] == ERRORSQUARE) {
                         break;
                    }
               }

               //  5. queen: added to bishop and rook
               //  6. king: is it needed?

               //  add this move to legal move array
               if (legal) {
                    allLegalNormalMoves[legalNormalMoveCount][0] = initialPosition;
                    allLegalNormalMoves[legalNormalMoveCount][1] = terminalPosition;
                    legalNormalMoveCount++;
               }
          }

          //  unmake move
          board[initialPosition] = board[terminalPosition];
          board[terminalPosition] = terminalValue;

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
     int i;
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
     }
     else { i += 2; }

     if (FEN.at(i) != '-') {
          //  get enpassant square
          enpassantSquare = ROW*(FEN.at(i) - 'a' + 2);
          i++;
          enpassantSquare += FEN.at(i)-'0';
     }

     i += 2;
     halfMoveClock = FEN.at(i) - '0';
     
     i += 2;
     moveNumber = FEN.at(i) - '0';
     

}
u64 perft(int depth) {
     u64 node = 0;
     int terminalValue;
     if (depth == 0) { return 1; }
     //  MOVEGEN
     // CHECK FOR LEGALS
     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {
          terminalValue = makeMove(currentBoard, depthLegalMoveList[depth][i]);
          node += perft(depth - 1);
          undoMove(currentBoard, depthLegalMoveList[depth][i], terminalValue);
     }
     //  TODO: Add Castling Moves
     //  TODO: Add Promotion Moves
     //  TODO: Add enpassant Moves
     return node;
}
int makeMove(int board[120], int move[2]) {
     int terminalValue;
     int initial = move[0], terminal = move[1];
     terminalValue = board[terminal];
     board[terminal] = board[initial];
     board[initial] = EMPTYSQUARE;
     return terminalValue;
     //  TODO: ENPASSANT CHECK
     //  TODO: CASTLING CHECK
}
void undoMove(int board[120], int move[2], int terminalValue) {
     int initial = move[0], terminal = move[1];
     board[initial] = board[terminal];
     board[terminal] = terminalValue;
}
void makeCastlingMove(int board[120], int move[2]) {}
void undoCastlingMove(int board[120], int move[2]) {}
int makePromotionMove(int board[120], int move[2]) { return 1; }
void undoPromotionMove(int board[120], int move[2], int terminalValue) {}
void makeEnpassantgMove(int board[120], int move[2]) {}
void undoEnPassantMove(int board[120], int move[2]) {}

void testOutput() {
     //  Generate Moves
     moveGeneration(currentBoard, currentTurn, allNormalMoves, &normalMoveCount);
     //  Print all moves
     for (int i = 0; i < normalMoveCount; i++) {
          printf("%d to %d\n", allNormalMoves[i][0], allNormalMoves[i][1]);
     }
     printf("Total Normal Moves: %d\n", normalMoveCount);
     printf("--------------------------------------------------\n");
     legalMoves(currentBoard, currentTurn);
     for (int i = 0; i < legalNormalMoveCount; i++) {
          printf("%d to %d\n", allLegalNormalMoves[i][0], allLegalNormalMoves[i][1]);
     }
     printf("Legal Normal Moves: %d\n", legalNormalMoveCount);
     printBoard(currentBoard);

     for (int i = 0; i < promotionMoveCount; i++) {
          printf("%d to %d: Piece Change to %d\n",
               promotionMoves[i][0], promotionMoves[i][1], promotionMoves[i][2]);
     }
     printf("\nTotal Promotion Moves: %d", promotionMoveCount);

     for (int i = 0; i < enpassantMoveCount; i++) {
          printf("%d to %d\n",
               enpassantMoves[i][0], enpassantMoves[i][1]);
     }
     printf("\nTotal Enpassant Moves: %d", enpassantMoveCount);

     for (int i = 0; i < castlingMoveCount; i++) {
          printf("%d to %d\n",
               castlingMoves[i][0], castlingMoves[i][1]);
     }
     printf("\nTotal Castling Moves: %d\n", castlingMoveCount);
}

//  TODO: Create separate function for enpassantMoveGeneration


void main() {
     //  Initialize Board
     //  board120Setup();
     
     //  FEN source:
     //  http://www.chesskit.com/training/fenkit/index.php?page=p9&d=Page%209
     //  turn has been edited
     FENboardSetup(currentBoard, "rn6/kp3p1p/pb6/N1B5/8/7P/5PP1/2R3K1 b - - 0 1");

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
     printf("--------------------------------------------------\n");

     //  int evaluationScore;
     currentTurn = BLACK;
     while (gamePlaying) {

          //  evaluationScore = updateEvaluation(currentBoard);
          //  printf("Evaluation Score: %d\n\n", evaluationScore);


          //  Generate Moves
          moveGeneration(currentBoard, currentTurn, allNormalMoves, &normalMoveCount);
          //  Print all moves
          printf("Total Normal Moves: %d\n", normalMoveCount);
          legalMoves(currentBoard, currentTurn);
          printf("Legal Normal Moves: %d\n", legalNormalMoveCount);
          for (int i = 0; i < promotionMoveCount; i++) {
               printf("%d to %d: Piece Change to %d\n",
                    promotionMoves[i][0], promotionMoves[i][1], promotionMoves[i][2]);
          }
          printf("Total Promotion Moves: %d", promotionMoveCount);
          for (int i = 0; i < enpassantMoveCount; i++) {
               printf("%d to %d\n",
                    enpassantMoves[i][0], enpassantMoves[i][1]);
          }
          printf("\nTotal Enpassant Moves: %d", enpassantMoveCount);
          for (int i = 0; i < castlingMoveCount; i++) {
               printf("%d to %d\n",
                    castlingMoves[i][0], castlingMoves[i][1]);
          }
          printf("\nTotal Castling Moves: %d\n", castlingMoveCount);
          printf("--------------------------------------------------\n");

          if (!endGame) {
               //  if no queens are on the board
               int queenCount=0;
               for (int i = 0; i < 120; i++) {
                    if (currentBoard[i] == WHITEQUEEN || currentBoard[i] == BLACKQUEEN) {
                         queenCount++;
                    }
               }
               if (queenCount == 0) {
                    endGame = true;
               }
          }
          printf("Endgame: %d\n", endGame);
          
          //  TODO: Make Best Move
          
          //  Change turns
          if (currentTurn == WHITE) { currentTurn = BLACK; }
          else { 
               currentTurn = WHITE; 
               moveNumber++;
          }

          //  This should be deleted for non-test cases
          gamePlaying = false;

          //  TODO: Check Fifty move rule
          
     }
}