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

//  Gets a piece and returns the color of the piece
int checkColor(int pieceType);
//  functions for each piece move generation & special move generation

//  Add the input move to the array

//  checks if a move is legal or not

//  receives a FEN string to setup board
void FENboardSetup(std::string FEN);
//  performance test function to test move generation

//  make and undo move (used for recursion
