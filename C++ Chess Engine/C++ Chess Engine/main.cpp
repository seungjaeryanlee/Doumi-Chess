/*                                    DEFINITION                              */
#define PAWNVALUE 0
#define KNIGHTVALUE 0
#define BISHOPVALUE 0
#define ROOKVALUE 0
#define QUEENVALUE 0
#define KINGVALUE 0
#include <stdio.h>

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
          currentBoard[A2+i] = WHITEROOK;
          currentBoard[A7+i] = BLACKROOK;
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

void main() {
     //  Initialize Board
     //  board120Setup();
     
     //  Testing updateEvaluation with PCSQ Table
     currentBoard[21] = WHITEPAWN;
     printf("PCSQTable value of 21 is: %d\n", 
          PAWN_PCSQTable[position120to64(21)]);
     currentBoard[22] = WHITEBISHOP;
     printf("PCSQTable value of 22 is: %d\n", 
          BISHOP_PCSQTable[position120to64(22)]);
     currentBoard[23] = WHITEKNIGHT;
     printf("PCSQTable value of 23 is: %d\n", 
          KNIGHT_PCSQTable[position120to64(23)]);
     currentBoard[24] = WHITEROOK;
     printf("PCSQTable value of 24 is: %d\n", 
          ROOK_PCSQTable[position120to64(24)]);
     currentBoard[25] = WHITEQUEEN;
     printf("PCSQTable value of 25 is: %d\n", 
          QUEEN_PCSQTable[position120to64(25)]);
     currentBoard[26] = WHITEKING;
     printf("PCSQTable value of 26 is: %d\n", 
          KING_PCSQTable[position120to64(26)]);
     
     
     currentBoard[91] = BLACKPAWN;
     printf("PCSQTable value of 91 is: %d\n", 
          PAWN_PCSQTable[position120to64(reversePosition(91))]);
     currentBoard[92] = BLACKBISHOP;
     printf("PCSQTable value of 92 is: %d\n",
          BISHOP_PCSQTable[position120to64(reversePosition(92))]);
     currentBoard[93] = BLACKKNIGHT;
     printf("PCSQTable value of 93 is: %d\n",
          KNIGHT_PCSQTable[position120to64(reversePosition(93))]);
     currentBoard[94] = BLACKROOK;
     printf("PCSQTable value of 94 is: %d\n",
          ROOK_PCSQTable[position120to64(reversePosition(94))]);
     currentBoard[95] = BLACKQUEEN;
     printf("PCSQTable value of 95 is: %d\n",
          QUEEN_PCSQTable[position120to64(reversePosition(95))]);
     currentBoard[96] = BLACKKING;
     printf("PCSQTable value of 96 is: %d\n",
          KING_PCSQTable[position120to64(reversePosition(96))]);
     

     printBoard(currentBoard);

     while (gamePlaying) {
          int evaluationScore;
          evaluationScore = updateEvaluation(currentBoard);

          printf("Evaluation Score: %d\n\n", evaluationScore);

          //  Testing position120to64
          for (int i = 0; i < 120; i++) {
               printf("%3d ", position120to64(i));
               if (i % 10 == 9) {
                    printf("\n");
               }
          }


          // TODO: Check Endgame

          //  This should be deleted for non-test cases
          gamePlaying = false;
     }
}