#pragma once
#include "defs.h"

/*                                  BOARD SETUP                               */
//  This function sets up currentboard[120] for the initial position of pieces.
void board120Setup();
//  receives a FEN string to setup board
void FENboardSetup(std::string FEN);
std::string boardToFEN(int board[120], int turn, bool castlingCheck[4], int enpassantSquare, int halfMoveClock, int moveNumber);
//  This functions prints the board from the parameter.
void printBoard(int board[120]);
//  Gets a piece and returns the color of the piece
int checkColor(int pieceType);
char numberToFile(int position);
int numberToRank(int position);
int filerankToNumber(char file, int rank);





/*                                BOARD EVALUATION                            */
//  Returns evaluation score based on parameter board given.
int boardEvaluation(int board[120]);
//  Gets a position number and returns the row-reversed position number
int reversePosition(int position);
//  Gets position based on 64 and returns position based on 120
int position64to120(int position64);
//  Gets position based on 120 and returns position based on 64
int position120to64(int position120);
//  negaMax implemented for board evaluation
int negaMax(int depth, int turn, bool castlingCheck[4]);
//  miniMax implemented for board evaluation
int blueValue(int depth, int turn, bool castlingCheck[4]);
int redValue(int depth, int turn, bool castlingCheck[4]);


/*                                   GAME CYCLE                               */
bool checkGameEnd(int board[120]);





/*                                MOVE GENERATION                             */
void moveGeneration(int board[120], int turn, int moveList[250][3], int *moveCount, int enpassantSquare, bool castlingCheck[4]);
void pawnMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount);
void knightMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount);
void bishopMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount);
void rookMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount);
void queenMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount);
void kingMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount);

void castlingMoveGeneration(int board[120], int turn, int moveList[250][3], int *moveCount, bool castlingCheck[4]);
void promotionMoveGeneration(int board[120], int turn, int position, int moveList[250][3], int *moveCount);
void enpassantMoveGeneration(int board[120], int turn, int moveList[250][3], int *moveCount, int enpassantSquare);

void addMove(int initial, int terminal, int moveType, int moveList[250][3], int *moveCount);
void addPromotionMove(int initial, int terminal, int moveList[250][3], int *moveCount);

void legalMoves(int board[120], int turn, int moveList[250][3], int moveCount, int legalMoveList[250][3], int *legalMoveCount);
bool squareAttackCheck(int board[120], int position, int turn);





/*                                   RECURSION                                */
u64 divide(int depth, int turn, int maxDepth, bool castlingCheck[4], bool showOutput);
u64 divide2(int depth, int turn, int maxDepth, bool castlingCheck[4], bool showOutput);
int makeMove(int board[120], int move[3]);
void undoMove(int board[120], int move[3], int terminalValue);
LARGE_INTEGER startTimer(LARGE_INTEGER *beginTime, int timerIndex);
void stopTimer(LARGE_INTEGER *endTime, int timerIndex);
void printElapsedTime(LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex);