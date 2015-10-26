/*                                    DEFINITION                              */
#define PAWNVALUE 100
#define KNIGHTVALUE 300
#define BISHOPVALUE 300
#define ROOKVALUE 500
#define QUEENVALUE 900
#define KINGVALUE 9999
#define ROW 10
#define COLUMN 1
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
//  Generates all moves possible by the current turn
void moveGeneration(int board[120], int turn);
//  Gets a piece and returns the color of the piece
int checkColor(int pieceType);
//  functions for each piece move generation
void pawnMoveGeneration(int board[120], int turn, int position);
void knightMoveGeneration(int board[120], int turn, int position);
void bishopMoveGeneration(int board[120], int turn, int position);
void rookMoveGeneration(int board[120], int turn, int position);
void queenMoveGeneration(int board[120], int turn, int position);
void kingMoveGeneration(int board[120], int turn, int position); 
void castlingMoveGeneration(int board[120], int turn);
void promotionMoveGeneration(int board[120], int turn);
//  Add the input move to the array
void addMove(int initial, int terminal);


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
//  First number: initial square, Second number: terminal square
//  TODO: Check upper bound of moves
int allMoves[1000][2];
int moveCount = 0;

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
void moveGeneration(int board[120], int turn) {
     if (turn == WHITE) {
          for (int i = 0; i < 120; i++) {
               switch (board[i]) {
               case WHITEPAWN:
                    pawnMoveGeneration(board, turn, i);
                    break;
               case WHITEKNIGHT:
                    knightMoveGeneration(board, turn, i);
                    break;
               case WHITEBISHOP:
                    bishopMoveGeneration(board, turn, i);
                    break;
               case WHITEROOK:
                    rookMoveGeneration(board, turn, i);
                    break;
               case WHITEQUEEN:
                    queenMoveGeneration(board, turn, i);
                    break;
               case WHITEKING:
                    kingMoveGeneration(board, turn, i);
                    break;
               }
          }
     }
     if (turn == BLACK) {
          for (int i = 0; i < 120; i++) {
               switch (board[i]) {
               case BLACKPAWN:
                    pawnMoveGeneration(board, turn, i);
                    break;
               case BLACKKNIGHT:
                    knightMoveGeneration(board, turn, i);
                    break;
               case BLACKBISHOP:
                    bishopMoveGeneration(board, turn, i);
                    break;
               case BLACKROOK:
                    rookMoveGeneration(board, turn, i);
                    break;
               case BLACKQUEEN:
                    queenMoveGeneration(board, turn, i);
                    break;
               case BLACKKING:
                    kingMoveGeneration(board, turn, i);
                    break;
               }
          }
     }
     
     //  TODO: Add Castling
     //  TODO: Add Enpassant
     //  TODO: Add Promotion

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
void pawnMoveGeneration(int board[120], int turn, int position) {
     if (turn == WHITE) {
          //  Advance 1 square
          if (board[position - ROW] == EMPTYSQUARE) {
               addMove(position, position - ROW);
                    //  Advance 2 squares
                    if (A2 <= position && position <= H2 && board[position - 2 * ROW] == EMPTYSQUARE) {
                         addMove(position, position - 2 * ROW);
                    }
          }

          //  attack diagonals
          if (checkColor(board[position - ROW - COLUMN]) == BLACK) {
               addMove(position, position - ROW - COLUMN);
          }
          if (checkColor(board[position - ROW + COLUMN]) == BLACK) {
               addMove(position, position - ROW + COLUMN);
          }
          //  TODO: enpassant Generation
     }
     if (turn == BLACK) {
          //  Advance 1 square
          if (board[position + ROW] == EMPTYSQUARE) {
               addMove(position, position + ROW);
               //  Advance 2 squares
               if (A7 <= position && position <= H7 && board[position + 2 * ROW] == EMPTYSQUARE) {
                    addMove(position, position + 2 * ROW);
               }
          }

          //  attack diagonals
          if (checkColor(board[position + ROW - COLUMN]) == WHITE) {
               addMove(position, position - ROW - COLUMN);
          }
          if (checkColor(board[position + ROW + COLUMN]) == WHITE) {
               addMove(position, position - ROW + COLUMN);
          }
     }
     
}
void knightMoveGeneration(int board[120], int turn, int position) {
     if (turn == WHITE) {
          if (checkColor(board[position + ROW + 2 * COLUMN]) == BLACK ||
              board[position + ROW + 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position + ROW + 2 * COLUMN);
          }
          if (checkColor(board[position + ROW - 2 * COLUMN]) == BLACK ||
               board[position + ROW - 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position + ROW - 2 * COLUMN);
          }
          if (checkColor(board[position - ROW + 2 * COLUMN]) == BLACK ||
               board[position - ROW + 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position - ROW + 2 * COLUMN);
          }
          if (checkColor(board[position - ROW - 2 * COLUMN]) == BLACK ||
               board[position - ROW - 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position - ROW - 2 * COLUMN);
          }
          if (checkColor(board[position + 2 * ROW + COLUMN]) == BLACK ||
               board[position + 2 * ROW + COLUMN] == EMPTYSQUARE) {
               addMove(position, position + 2 * ROW + COLUMN);
          }
          if (checkColor(board[position + 2 * ROW - COLUMN]) == BLACK ||
               board[position + 2 * ROW - COLUMN] == EMPTYSQUARE) {
               addMove(position, position + 2 * ROW - COLUMN);
          }
          if (checkColor(board[position - 2 * ROW + COLUMN]) == BLACK ||
               board[position - 2 * ROW + COLUMN] == EMPTYSQUARE) {
               addMove(position, position - 2 * ROW + COLUMN);
          }
          if (checkColor(board[position - 2 * ROW - COLUMN]) == BLACK ||
               board[position - 2 * ROW - COLUMN] == EMPTYSQUARE) {
               addMove(position, position - 2 * ROW - COLUMN);
          }
     }
     if (turn == BLACK) {
          if (checkColor(board[position + ROW + 2 * COLUMN]) == WHITE ||
               board[position + ROW + 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position + ROW + 2 * COLUMN);
          }
          if (checkColor(board[position + ROW - 2 * COLUMN]) == WHITE ||
               board[position + ROW - 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position + ROW - 2 * COLUMN);
          }
          if (checkColor(board[position - ROW + 2 * COLUMN]) == WHITE ||
               board[position - ROW + 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position - ROW + 2 * COLUMN);
          }
          if (checkColor(board[position - ROW - 2 * COLUMN]) == WHITE ||
               board[position - ROW - 2 * COLUMN] == EMPTYSQUARE) {
               addMove(position, position - ROW - 2 * COLUMN);
          }
          if (checkColor(board[position + 2 * ROW + COLUMN]) == WHITE ||
               board[position + 2 * ROW + COLUMN] == EMPTYSQUARE) {
               addMove(position, position + 2 * ROW + COLUMN);
          }
          if (checkColor(board[position + 2 * ROW - COLUMN]) == WHITE ||
               board[position + 2 * ROW - COLUMN] == EMPTYSQUARE) {
               addMove(position, position + 2 * ROW - COLUMN);
          }
          if (checkColor(board[position - 2 * ROW + COLUMN]) == WHITE ||
               board[position - 2 * ROW + COLUMN] == EMPTYSQUARE) {
               addMove(position, position - 2 * ROW + COLUMN);
          }
          if (checkColor(board[position - 2 * ROW - COLUMN]) == WHITE ||
               board[position - 2 * ROW - COLUMN] == EMPTYSQUARE) {
               addMove(position, position - 2 * ROW - COLUMN);
          }
     }
}
void bishopMoveGeneration(int board[120], int turn, int position) {}
void rookMoveGeneration(int board[120], int turn, int position) {}
void queenMoveGeneration(int board[120], int turn, int position) {}
void kingMoveGeneration(int board[120], int turn, int position) {}
void castlingMoveGeneration(int board[120], int turn) {}
void promotionMoveGeneration(int board[120], int turn) {}
void addMove(int initial, int terminal) {
     allMoves[moveCount][0] = initial;
     allMoves[moveCount][1] = terminal;
     moveCount++;
}

void main() {
     //  Initialize Board
     //  board120Setup();
     
     //  Testing knightMoveGeneration
     currentBoard[51] = WHITEKNIGHT;
     currentBoard[64] = WHITEKNIGHT;
     currentBoard[53] = BLACKKNIGHT;
     currentBoard[62] = WHITEKNIGHT;
     currentBoard[52] = WHITEKNIGHT;
     
     printBoard(currentBoard);

     int evaluationScore;
     int currentTurn = WHITE;
     while (gamePlaying) {

          //  evaluationScore = updateEvaluation(currentBoard);
          //  printf("Evaluation Score: %d\n\n", evaluationScore);


          //  Generate Moves
          moveGeneration(currentBoard, currentTurn);
          //  Print all moves
          for (int i = 0; i < moveCount; i++) {
               printf("%d to %d\n", allMoves[i][0], allMoves[i][1]);
          }
          printf("\nTotal Moves: %d\n", moveCount);



          // TODO: Check Endgame

          //  This should be deleted for non-test cases
          gamePlaying = false;

          //  Change turns
          if (currentTurn == WHITE) {
               currentTurn = BLACK;
          }
          else {
               currentTurn = WHITE;
          }
     }
}