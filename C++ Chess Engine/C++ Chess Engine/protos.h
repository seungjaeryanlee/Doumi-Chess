#pragma once
#include "defs.h"

using namespace std;

class Board {
private:
     array<int, 120> board;
     array<bool, 4> castlingCheck;
     int turn;
     int enpassantSquare;
     int fiftyMoveCount;
     int moveNumber;

public:
     // Sets
     void setBoard(array<int, 120> b) { board = b; }
     void setSquare(int square, int value) { board.at(square) = value; }
     void setCastlingArray(array<bool, 4> cc) { castlingCheck = cc; }
     void setCastling(int index, bool value) { castlingCheck.at(index) = value; }
     void setTurn(int t) { turn = t; }
     void setEnpassantSquare(int e) { enpassantSquare = e; }
     void setFiftyMoveCount(int f) { fiftyMoveCount = f; }
     void setMoveNumber(int m) { moveNumber = m; }

     //  Accessors
     array<int, 120> getBoard() { return board; }
     int getSquare(int square) { return board.at(square); }
     array<bool, 4> getCastlingArray() { return castlingCheck; }
     bool getCastling(int index) { return castlingCheck.at(index); }
     int getTurn() { return turn; }
     int getEnpassantSquare() { return enpassantSquare; }
     int getFiftyMoveCount() { return fiftyMoveCount; }
     int getMoveNumber() { return moveNumber; }
};
class Move {
private:
     int initialSquare;
     int terminalSquare;
     int moveType;
public:
     // Sets
     void setInitial(int i) { initialSquare = i; }
     void setTerminal(int t) { terminalSquare = t; }
     void setType(int t) { moveType = t; }
     // Accessors
     int getInitial() { return initialSquare; }
     int getTerminal() { return terminalSquare; }
     int getType() { return moveType; }

};

/*                                  BOARD SETUP                               */
//  This function sets up currentboard[120] for the initial position of pieces.
void board120Setup();
//  receives a FEN string to setup board
void FENboardSetup(string FEN);
string boardToFEN(Board board);
//  This functions prints the board from the parameter.
void printBoard(Board board);
//  This functions prints simple version of the board from the parameter.
void printSimpleBoard(Board board);
//  Gets a piece and returns the color of the piece
int checkColor(int pieceType);
char numberToFile(int position);
int numberToRank(int position);
int filerankToNumber(char file, int rank);
std::string numberToFilerank(int position);
void printMove(int move[3]);



/*                                BOARD EVALUATION                            */
//  Returns evaluation score based on parameter board given.
int boardEvaluation(Board board);
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
//  negaMax implemented
int negaMax(int depth, int turn, bool castlingCheck[4]);
//  function to call negaMax. bestMoves is the output
int rootNegaMax(int maxDepth, int turn, bool castlingCheck[4], int bestMoves[3]);


/*                                   GAME CYCLE                               */
bool checkGameEnd(Board board);
//  Save Board state for threefold repetition check
void saveCurrentState();



/*                                MOVE GENERATION                             */
void moveGeneration(Board board, int moveList[250][3], int *moveCount, int enpassantSquare);
void pawnMoveGeneration(Board board, int position, int moveList[250][3], int *moveCount);
void knightMoveGeneration(Board board, int position, int moveList[250][3], int *moveCount);
void bishopMoveGeneration(Board board, int position, int moveList[250][3], int *moveCount);
void rookMoveGeneration(Board board, int position, int moveList[250][3], int *moveCount);
void queenMoveGeneration(Board board, int position, int moveList[250][3], int *moveCount);
void kingMoveGeneration(Board board, int position, int moveList[250][3], int *moveCount);

void castlingMoveGeneration(Board board, int moveList[250][3], int *moveCount);
void promotionMoveGeneration(Board board, int position, int moveList[250][3], int *moveCount);
void enpassantMoveGeneration(Board board, int moveList[250][3], int *moveCount);

void addMove(int initial, int terminal, int moveType, int moveList[250][3], int *moveCount);
void addPromotionMove(int initial, int terminal, int moveList[250][3], int *moveCount);

void legalMoves(Board board, int moveList[250][3], int moveCount, int legalMoveList[250][3], int *legalMoveCount);
bool squareAttackCheck(Board board, int position);





/*                                   RECURSION                                */
u64 divide(int depth, int turn, int maxDepth, bool castlingCheck[4], bool showOutput);
u64 divide2(int depth, int turn, int maxDepth, bool castlingCheck[4], bool showOutput);
int makeMove(Board board, int move[3]);
void undoMove(Board board, int move[3], int terminalValue);
LARGE_INTEGER startTimer(LARGE_INTEGER *beginTime, int timerIndex);
void stopTimer(LARGE_INTEGER *endTime, int timerIndex);
void printElapsedTime(LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex);
double elapsedTime(LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex);