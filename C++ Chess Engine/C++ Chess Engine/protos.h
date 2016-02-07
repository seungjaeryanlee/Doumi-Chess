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
     //  Mutators
     void setBoard(const array<int, 120> b) { board = b; }
     void setSquare(const int square, const int value) { board.at(square) = value; }
     void setCastlingArray(const array<bool, 4> cc) { castlingCheck = cc; }
     void setCastling(const int index, const bool value) { castlingCheck.at(index) = value; }
     void setTurn(const int t) { turn = t; }
     void setEnpassantSquare(const int e) { enpassantSquare = e; }
     void setFiftyMoveCount(const int f) { fiftyMoveCount = f; }
     void setMoveNumber(const int m) { moveNumber = m; }

     //  Accessors
     const array<int, 120> getBoard() const { return board; }
     const int getSquare(int square) const { return board.at(square); }
     const array<bool, 4> getCastlingArray() const { return castlingCheck; }
     const bool getCastling(int index) const { return castlingCheck.at(index); }
     const int getTurn() const { return turn; }
     const int getEnpassantSquare() const { return enpassantSquare; }
     const int getFiftyMoveCount() const { return fiftyMoveCount; }
     const int getMoveNumber() const { return moveNumber; }

     //  Other Functions
     void changeTurn() { turn = -turn; }
     void fiftyMoveCountDecrement() { fiftyMoveCount--; }
     void fiftyMoveCountIncrement() { fiftyMoveCount++; }
     void moveNumberDecrement() { moveNumber--; }
     void moveNumberIncrement() { moveNumber++; }
};
class Move {
private:
     int initialSquare;
     int terminalSquare;
     int moveType;
public:
     // Mutators
     void setInitial(const int i) { initialSquare = i; }
     void setTerminal(const int t) { terminalSquare = t; }
     void setType(const int t) { moveType = t; }
     // Accessors
     const int getInitial() const { return initialSquare; }
     const int getTerminal() const { return terminalSquare; }
     const int getType() const { return moveType; }

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
//  negaMax implemented for board evaluation
int negaMax(int depth, int turn, bool castlingCheck[4]);
//  function to call negaMax. bestMoves is the output
int rootNegaMax(int maxDepth, int turn, bool castlingCheck[4], int bestMoves[3]);


/*                                   GAME CYCLE                               */
bool checkGameEnd(Board board);
//  Save Board state for threefold repetition check
void saveCurrentState();



/*                                MOVE GENERATION                             */
void moveGeneration(Board board, int moveList[250][3], int *moveCount);
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
u64 divide(int depth, int maxDepth, Board board, bool showOutput);
u64 divide2(int depth, int maxDepth, Board board, bool showOutput);
// Makes the given move and changes turn
int makeMove(Board &board, int move[3]);
// Undos the given move and changes turn
void undoMove(Board &board, int move[3], int terminalValue);
LARGE_INTEGER startTimer(LARGE_INTEGER *beginTime, int timerIndex);
void stopTimer(LARGE_INTEGER *endTime, int timerIndex);
void printElapsedTime(LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex);
double elapsedTime(LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex);