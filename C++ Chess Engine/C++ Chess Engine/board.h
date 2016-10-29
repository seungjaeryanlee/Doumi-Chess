#pragma once
#include <array>
#include "defs.h"
#include "move.h"

void boardEvalCalled();

class Board {
private:
     std::array<int, 120> board;
     std::array<bool, 4> castlingRights;
     color turn;
     int enpassantSquare;
     int halfMoveClock;
     int moveNumber;

     bool isEndgame = false;
     std::array<int, 14> pieceCount;

public:

     // Default Constructor
     Board() {}

     // Constructor adding all content at once
     Board(std::array<int, 120> b, std::array<bool, 4> cc, color t, int e, int f, int m) {
          board = b;
          castlingRights = cc;
          turn = t;
          enpassantSquare = e;
          halfMoveClock = f;
          moveNumber = m;
     }

     //  Clone Method
     Board(const Board& originalBoard) {
          board = originalBoard.getBoard();
          castlingRights = originalBoard.getCastlingRights();
          turn = originalBoard.getTurn();
          enpassantSquare = originalBoard.getEnpassantSquare();
          halfMoveClock = originalBoard.getHalfMoveClock();
          moveNumber = originalBoard.getMoveNumber();
     }

     //  isEqual Method
     bool isEqual(const Board& thatBoard) {
          if (this == &thatBoard) {
               return true;
          }
          if (board == thatBoard.board &&
               castlingRights == thatBoard.castlingRights &&
               turn == thatBoard.turn &&
               enpassantSquare == thatBoard.enpassantSquare &&
               halfMoveClock == thatBoard.halfMoveClock &&
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
               castlingRights == thatBoard.castlingRights &&
               enpassantSquare == thatBoard.enpassantSquare) {
               return true;
          }
          else {
               return false;
          }
     }

     //  Mutators
     void setBoard(const std::array<int, 120> b) { board = b; }
     void setSquare(const int square, const int value) { board.at(square) = value; }
     void setCastlingRights(const std::array<bool, 4> cc) { castlingRights = cc; }
     void setCastlingRight(const int index, const bool value) { castlingRights.at(index) = value; }
     void setTurn(const color t) { turn = t; }
     void setEnpassantSquare(const int e) { enpassantSquare = e; }
     void setHalfMoveClock(const int f) { halfMoveClock = f; }
     void setMoveNumber(const int m) { moveNumber = m; }
     void setEndgame(bool e) { isEndgame = e; }
     void setPieceCount(const std::array<int, 14> pc) { pieceCount = pc; }
     void updateEndgame() {
          if (isEndgame) { return; }
          else {
               int queenCount = 0;
               for (int i = 0; i < 120; i++) {
                    if (board[i] == WHITEQUEEN || board[i] == BLACKQUEEN) {
                         queenCount++;
                    }
               }
               if (queenCount == 0) { isEndgame = true; }
               else { isEndgame = false; }
          }
     }
     void updateEndgame(Move move) {
          if (isEndgame) { return; }
          if (move.getTerminal() != WHITEQUEEN && move.getTerminal() != BLACKQUEEN) { return; }
          else {
               int queenCount = 0;
               for (int i = 0; i < 120; i++) {
                    if (board[i] == WHITEQUEEN || board[i] == BLACKQUEEN) {
                         queenCount++;
                    }
               }
               if (queenCount == 0) { isEndgame = true; }
               else { isEndgame = false; }
          }
     }

     //  Accessors
     const std::array<int, 120> getBoard() const { return board; }
     const int getSquare(int square) const { return board.at(square); }
     const std::array<bool, 4> getCastlingRights() const { return castlingRights; }
     const bool getCastlingRight(int index) const { return castlingRights.at(index); }
     const color getTurn() const { return turn; }
     const int getEnpassantSquare() const { return enpassantSquare; }
     const int getHalfMoveClock() const { return halfMoveClock; }
     const int getMoveNumber() const { return moveNumber; }
     const bool getEndgame() const { return isEndgame; }
     const std::array<int, 14> getPieceCount() const { return pieceCount; }

     //  Other Functions
     void changeTurn() { turn = (color)-turn; }
     void incrementHalfMoveClock() { halfMoveClock++; }
     void decrementHalfMoveClock() { halfMoveClock--; }
     void incrementMoveNumber() { moveNumber++; }
     void decrementMoveNumber() { moveNumber--; }

     void updatePieceCount() {
          for (int i = 0; i < 14; i++) {
               pieceCount[i] = 0;
          }
          for (int i = 0; i < 8; i++) {
               for (int j = 0; j < 8; j++) {
                    int position120 = ROW*(i + 2) + (j + 1);
                    pieceCount[board[position120]]++;
               }
          }
     }
     void updatePieceCount(const Move& move, const int capturedPiece) {
          if (capturedPiece != EMPTYSQUARE) {
               pieceCount[capturedPiece]--;
               return;
          }
          else if (move.getType() == KNIGHT_PROMOTION) {
               if (move.getTerminal() == WHITEKNIGHT) {
                    pieceCount[WHITEPAWN]--;
                    pieceCount[WHITEKNIGHT]++;
                    return;
               }
               else {
                    pieceCount[BLACKPAWN]--;
                    pieceCount[BLACKKNIGHT]++;
                    return;
               }
          }
          else if (move.getType() == KNIGHT_PROMOTION) {
               if (move.getTerminal() == WHITEKNIGHT) {
                    pieceCount[WHITEPAWN]--;
                    pieceCount[WHITEKNIGHT]++;
                    return;
               }
               else {
                    pieceCount[BLACKPAWN]--;
                    pieceCount[BLACKKNIGHT]++;
                    return;
               }
          }
          else if (move.getType() == BISHOP_PROMOTION) {
               if (move.getTerminal() == WHITEBISHOP) {
                    pieceCount[WHITEPAWN]--;
                    pieceCount[WHITEBISHOP]++;
                    return;
               }
               else {
                    pieceCount[BLACKPAWN]--;
                    pieceCount[BLACKBISHOP]++;
                    return;
               }
          }
          else if (move.getType() == ROOK_PROMOTION) {
               if (move.getTerminal() == WHITEROOK) {
                    pieceCount[WHITEPAWN]--;
                    pieceCount[WHITEROOK]++;
                    return;
               }
               else {
                    pieceCount[BLACKPAWN]--;
                    pieceCount[BLACKROOK]++;
                    return;
               }
          }
          else if (move.getType() == QUEEN_PROMOTION) {
               if (move.getTerminal() == WHITEQUEEN) {
                    pieceCount[WHITEPAWN]--;
                    pieceCount[WHITEQUEEN]++;
               }
               else {
                    pieceCount[BLACKPAWN]--;
                    pieceCount[BLACKQUEEN]++;
               }
          }
     }

     /// <summary>
     /// This function returns evaluation score of the board using piece values and PCSQ tables. Positive score signifies white's advantage.
     /// </summary>
     /// <returns>The score of the board</returns>
     int boardEvaluation() {
          boardEvalCalled();
          int score = 0;
          score += (pieceCount[WHITEPAWN] - pieceCount[BLACKPAWN])*PAWNVALUE
               + (pieceCount[WHITEKNIGHT] - pieceCount[BLACKKNIGHT])*KNIGHTVALUE
               + (pieceCount[WHITEBISHOP] - pieceCount[BLACKBISHOP])*BISHOPVALUE
               + (pieceCount[WHITEROOK] - pieceCount[BLACKROOK])*ROOKVALUE
               + (pieceCount[WHITEQUEEN] - pieceCount[BLACKQUEEN])*QUEENVALUE
               + (pieceCount[WHITEKING] - pieceCount[BLACKKING])*KINGVALUE;

          for (int i = 0; i < 8; i++) {
               for (int j = 0; j < 8; j++) {
                    int position120 = ROW*(i + 2) + (j + 1);
                    if (!isEndgame) {
                         score += PCSQVALUE[board[position120]][position120];
                    }
                    else {
                         score += PCSQVALUE_ENDGAME[board[position120]][position120];
                    }
               }
          }
          return score;
     }
};


/*                                  BOARD SETUP                               */
/// <summary>
/// This function sets the given board to the initial state.
/// </summary>
/// <param name="board">The board that will be changed to initial state.</param>
void board120Setup(Board& board);
/// <summary>
/// This function sets the given board according to the FEN.
/// </summary>
/// <param name="board">The board that will be changed to initial state.</param>
/// <param name="FEN">The desired board in FEN notation.</param>
void FENboardSetup(Board& board, const std::string FEN);
/// <summary>
/// This function converts the given board to FEN notation.
/// </summary>
/// <param name="board">The board that will be converted to FEN notation.</param>
/// <returns>FEN notation of the given board.</returns>
std::string boardToFEN(const Board& board);


/// <summary>
/// This function returns the color of the given piece type
/// </summary>
/// <param name="pieceType">The piece that will be checked its color</param>
/// <returns>The color of the given piece</returns>
int checkColor(const int pieceType);

/// <summary>
/// This function converts the file and rank notation to board index of a board. 
/// </summary>
/// <param name="file">The file of the square</param>
/// <param name="rank">The rank of the square</param>
/// <returns>The board index notation of the square</returns>
int filerankToNumber(const char file, const int rank);


/// <summary>
/// This function checks whether the king is being attacked or not.
/// </summary>
/// <param name="board">The board that will be checked.</param>
/// <param name="kingPos">The position of the king</param>
/// <returns>True if </returns>
bool isSquareAttacked(const Board& board, int kingPos);

/// <summary>
/// This function prints the given board to console.
/// </summary>
/// <param name="board">The board that will be printed to console.</param>
void printBoard(const Board& board);
/// <summary>
/// This function prints the given board to console excluding the error squares.
/// </summary>
/// <param name="board">The board that will be printed to console.</param>
void printSimpleBoard(const Board& board);