#include "defs.h"

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
void moveGeneration(int board[120], int turn, int normalMoveList[250][2], int *normalMoveCount);
//  Gets a piece and returns the color of the piece
int checkColor(int pieceType);
//  functions for each piece move generation & special move generation
void pawnMoveGeneration(int board[120], int turn, int position, int normalMoveList[250][2], int *normalMoveCount, int promotionMoveList[88][3], int *promotionMoveCount);
void knightMoveGeneration(int board[120], int turn, int position, int normalMoveList[250][2], int *normalMoveCount);
void bishopMoveGeneration(int board[120], int turn, int position, int normalMoveList[250][2], int *normalMoveCount);
void rookMoveGeneration(int board[120], int turn, int position, int normalMoveList[250][2], int *normalMoveCount);
void queenMoveGeneration(int board[120], int turn, int position, int normalMoveList[250][2], int *normalMoveCount);
void kingMoveGeneration(int board[120], int turn, int position, int normalMoveList[250][2], int *normalMoveCount);
void castlingMoveGeneration(int board[120], int turn, int castlingMoveList[2][2], int *castlingMoveCount);
void promotionMoveGeneration(int board[120], int turn, int position, int promotionMoveList[88][3], int *promotionMoveCount);
//  Add the input move to the array
void addMove(int initial, int terminal, int normalMoveList[250][2], int *normalMoveCount);
void addPromotionMove(int initial, int terminal, int turn);
//  checks if a move is legal or not
void legalMoves(int board[120], int turn, int normalMoveList[250][2], int normalMoveCount, int legalNormalMoveList[250][2], int *legalNormalMoveCount);
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