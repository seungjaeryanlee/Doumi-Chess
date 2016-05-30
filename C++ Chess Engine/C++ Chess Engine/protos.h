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
     
     // Default Constructor
     Board() {}
     /*
     Board() {
          for (int i = 0; i < 120; i++) {
               board[i] = ERRORSQUARE;
          }
          for (int i = 2; i < 10; i++) {
               for (int j = 1; j < 9; j++) {
                    board[i * 8 + j] = EMPTYSQUARE;
               }
          }

          turn = WHITE;
          enpassantSquare = 0;
          fiftyMoveCount = 0;
          moveNumber = 1;
     }
     */

     // Constructor adding all content at once
     Board(array<int, 120> b, array<bool, 4> cc, int t, int e, int f, int m) {
          board = b;
          castlingCheck = cc;
          turn = t;
          enpassantSquare = e;
          fiftyMoveCount = f;
          moveNumber = m;
     }
     
     //  Clone Method
     Board(const Board& originalBoard) {
          board = originalBoard.getBoard();
          castlingCheck = originalBoard.getCastlingArray();
          turn = originalBoard.getTurn();
          enpassantSquare = originalBoard.getEnpassantSquare();
          fiftyMoveCount = originalBoard.getFiftyMoveCount();
          moveNumber = originalBoard.getMoveNumber();
     }

     //  isEqual Method
     bool isEqual(const Board& thatBoard) {
          if (this == &thatBoard) {
               return true;
          }
          if (board == thatBoard.board &&
               castlingCheck == thatBoard.castlingCheck &&
               turn == thatBoard.turn &&
               enpassantSquare == thatBoard.enpassantSquare &&
               fiftyMoveCount == thatBoard.fiftyMoveCount &&
               moveNumber && thatBoard.moveNumber) {
               return true;
          }
          else {
               return false;
          }
     }

     //  isAlmostEqual Method (for threefold repetition checking)
     bool isAlmostEqual(const Board& thatBoard) {
          if (this == &thatBoard) {
               return true;
          }
          if (board == thatBoard.board &&
               castlingCheck == thatBoard.castlingCheck &&
               enpassantSquare == thatBoard.enpassantSquare) {
               return true;
          }
          else {
               return false;
          }
     }

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
     // Constructors
     Move() {}
     Move(int i, int t, int m) {
          initialSquare = i;
          terminalSquare = t;
          moveType = m;
     }

     // Clone Methods
     Move(const Move& originalMove) {
          initialSquare = originalMove.getInitial();
          terminalSquare = originalMove.getTerminal();
          moveType = originalMove.getType();
     }
     Move(int originalMove[3]) {
          initialSquare = originalMove[0];
          terminalSquare = originalMove[1];
          moveType = originalMove[2];
     }
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
void FENboardSetup(const string FEN);
string boardToFEN(const Board& board);
//  This functions prints the board from the parameter.
void printBoard(const Board& board);
//  This functions prints simple version of the board from the parameter.
void printSimpleBoard(const Board& board);
//  Gets a piece and returns the color of the piece
int checkColor(int pieceType);
char numberToFile(int position);
int numberToRank(int position);
int filerankToNumber(char file, int rank);
std::string numberToFilerank(int position);
void printMove(int move[3]);



/*                                BOARD EVALUATION                            */
//  Returns evaluation score based on parameter board given.
int boardEvaluation(const Board& board);
//  Gets a position number and returns the row-reversed position number
int reversePosition(int position);
//  negaMax implemented for board evaluation
int negaMax(int depth, int turn, bool castlingCheck[4]);
//  function to call negaMax. bestMoves is the output
int rootNegaMax(int maxDepth, int turn, bool castlingCheck[4], Move bestMove);
//  negaMax with alphaBeta pruning implemented for board evaluation
int alphabeta(int depth, Board& board, int alpha, int beta);
int rootAlphabeta(int maxDepth, Board board, int alpha, int beta, Move bestMove);




/*                                   GAME CYCLE                               */
bool checkGameEnd(const Board& board);




/*                                MOVE GENERATION                             */
void moveGeneration(Board& board, int moveList[250][3], int *moveCount);
void pawnMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount);
void knightMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount);
void bishopMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount);
void rookMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount);
void queenMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount);
void kingMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount);

void moveGeneration(Board& board, Move moveList[250], int *moveCount);
void pawnMoveGeneration(Board& board, int position, Move moveList[250], int *moveCount);
void knightMoveGeneration(Board& board, int position, Move moveList[250], int *moveCount);
void bishopMoveGeneration(Board& board, int position, Move moveList[250], int *moveCount);
void rookMoveGeneration(Board& board, int position, Move moveList[250], int *moveCount);
void queenMoveGeneration(Board& board, int position, Move moveList[250], int *moveCount);
void kingMoveGeneration(Board& board, int position, Move moveList[250], int *moveCount);

void castlingMoveGeneration(Board& board, int moveList[250][3], int *moveCount);
void promotionMoveGeneration(Board& board, int position, int moveList[250][3], int *moveCount);
void enpassantMoveGeneration(Board& board, int moveList[250][3], int *moveCount);

void castlingMoveGeneration(Board& board, Move moveList[250], int *moveCount);
void promotionMoveGeneration(Board& board, int position, Move moveList[250], int *moveCount);
void enpassantMoveGeneration(Board& board, Move moveList[250], int *moveCount);

void addMove(int initial, int terminal, int moveType, int moveList[250][3], int *moveCount);
void addMove(int initial, int terminal, int moveType, Move moveList[250], int *moveCount);
void addPromotionMove(int initial, int terminal, int moveList[250][3], int *moveCount);
void addPromotionMove(int initial, int terminal, Move moveList[250], int *moveCount);

void legalMoves(Board board, int moveList[250][3], int moveCount, int legalMoveList[250][3], int *legalMoveCount);
void legalMoves(Board board, Move moveList[250], int moveCount, Move legalMoveList[250], int *legalMoveCount);
bool squareAttackCheck(Board board, int position);





/*                                   RECURSION                                */
u64 divide(int depth, int maxDepth, Board& board, bool showOutput);
u64 divide2(int depth, int maxDepth, Board& board, bool showOutput);
// Makes the given move and changes turn
int makeMove(Board &board, int move[3]);
int makeMove(Board &board, Move move);
// Undos the given move and changes turn
void undoMove(Board &board, int move[3], int terminalValue);
void undoMove(Board &board, Move move, int terminalValue);
LARGE_INTEGER startTimer(LARGE_INTEGER *beginTime, int timerIndex);
void stopTimer(LARGE_INTEGER *endTime, int timerIndex);
void printElapsedTime(LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex);
double elapsedTime(LARGE_INTEGER beginTime, LARGE_INTEGER endTime, LARGE_INTEGER frequency, int timerIndex);

/*                                  MISC                                      */
// Helper function that updates castling array inside board based on given move
// TODO: Check if this can be integrated inside makeMove
void castlingUpdate(Board& board, const Move& move);
// Checks if the board is at the end state: returns -1 if false, return board value otherwise
int isTerminalNode(Board& board);
     