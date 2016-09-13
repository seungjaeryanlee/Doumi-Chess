#pragma once
#include "defs.h"
#include <Windows.h>



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
class MoveList {
private:
     std::array<Move, MAX_MOVEGEN_COUNT> movelist;
     int moveCounter;
public:
     // Construcctor
     MoveList() {
          moveCounter = 0;
     }

     // Accessor
     inline std::array<Move, MAX_MOVEGEN_COUNT> getList() { return movelist; }
     inline Move getMove(int index) { return movelist[index]; }
     inline int getCounter() { return moveCounter; }

     // Mutator
     void addMove(Move& move) {
          movelist[moveCounter] = move;
          moveCounter++;
     }
     void setCounterToZero() { moveCounter = 0; }
};
class Board {
private:
     std::array<int, 120> board;
     std::array<bool, 4> castlingRights;
     color turn;
     int enpassantSquare;
     int halfMoveClock;
     int moveNumber;
     
     bool isEndgame;
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

     //  Other Functions
     void changeTurn() { turn = (color)-turn; }
     void incrementHalfMoveClock() { halfMoveClock++; }
     void decrementHalfMoveClock() { halfMoveClock--; }
     void incrementMoveNumber() { moveNumber++; }
     void decrementMoveNumber() { moveNumber--; }
     
     void updatePieceCount(Move& move, int capturedPiece) {
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
          int score = 0;
          for (int i = 0; i < 8; i++) {
               for (int j = 0; j < 8; j++) {
                    int position120 = ROW*(i + 2) + (j + 1);
                    score += PIECEVALUE[board[position120]];
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

     /*
     void updateScore(Move& move, int capturedPiece) {
          if (capturedPiece != EMPTYSQUARE) {
               score -= PIECEVALUE[capturedPiece];
               score -= PCSQVALUE[capturedPiece][move.getTerminal()];
          }
          if (!isEndgame) {
               score += PCSQVALUE[board[move.getTerminal()]][move.getTerminal()] - PCSQVALUE[board[move.getInitial()]][move.getInitial()];
          }
          else {
               score += PCSQVALUE_ENDGAME[board[move.getTerminal()]][move.getTerminal()] - PCSQVALUE_ENDGAME[board[move.getInitial()]][move.getInitial()];
          }
     }
     //*/


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
/// This function returns the column character (file) of a given square.
/// </summary>
/// <param name="position">The square that the function will give its file</param>
/// <returns>The file of the given square</returns>
char numberToFile(const int position);
/// <summary>
/// This function returns the row number (rank) of a given square.
/// </summary>
/// <param name="position">The square that the function will give its rank</param>
/// <returns>The rank of the given square</returns>
int numberToRank(const int position);
/// <summary>
/// This function converts the file and rank notation to board index of a board. 
/// </summary>
/// <param name="file">The file of the square</param>
/// <param name="rank">The rank of the square</param>
/// <returns>The board index notation of the square</returns>
int filerankToNumber(const char file, const int rank);
/// <summary>
/// This function converts a position's notation from board index to file rank.
/// </summary>
/// <param name="position">The square in board index notation.</param>
/// <returns>A two-letter string with file rank notation of the given square.</returns>
std::string numberToFilerank(const int position);





/*                                   RECURSION                                */
/// <summary>
/// This function finds the number of possible move sequences for given depth and divides them into categories of first few moves given by maxDepth and prints them to console.
/// </summary>
/// <param name="depth">The current depth of the recursion.</param>
/// <param name="maxDepth">The maximum depth the moves will be divided in output</param>
/// <param name="board">The board being checked.</param>
/// <param name="showOutput">Wheter or not to show intermediate output.</param>
/// <returns>The number of possible move sequences of given depth.</returns>
uint64_t divide(const int depth, const int maxDepth, Board& board, const bool showOutput);
/// <summary>
/// This function finds the number of possible move sequences for given depth and divides them into categories of first few moves given by maxDepth amd saves them into a file.
/// </summary>
/// <param name="depth">The current depth of the recursion.</param>
/// <param name="maxDepth">The maximum depth the moves will be divided in output</param>
/// <param name="board">The board being checked.</param>
/// <param name="showOutput">Wheter or not to show intermediate output.</param>
/// <returns>The number of possible move sequences of given depth.</returns>
uint64_t divide2(const int depth, const int maxDepth, Board& board, const bool showOutput);
/// <summary>
/// This function applies the given move to the given board.
/// </summary>
/// <param name="board">The board that the move will apply to.</param>
/// <param name="move">The move to be applied to the board.</param>
/// <returns>The piece that was captured with the move</returns>
int makeMove(Board &board, const Move& move);
/// <summary>
/// This function reverts the given move on the given board.
/// </summary>
/// <param name="board">The board that the move will be reverted in</param>
/// <param name="move">The move that will be reverted</param>
/// <param name="capturedPiece">The piece that was captured by the move</param>
void undoMove(Board &board, const Move& move, const int capturedPiece);


/*                                  MISC                                      */
// TODO: Check if this can be integrated inside makeMove
/// <summary>
/// This function updates the castling rights of the given board if the given move changed it. This is called after the move was applied to the board.
/// </summary>
/// <param name="board">The board that will be checked.</param>
/// <param name="move">The move that was made that could have changed the castling rights.</param>
void updateCastling(Board& board, const Move& move);
/// <summary>
/// This function updates the en passant square of the given board if the given move changed it. This is called after the move was applied to the board.
/// </summary>
/// <param name="board">The board that will be checked.</param>
/// <param name="move">The move that was made that could have changed the en passant square.</param>
void updateEnPassant(Board& board, const Move& move);
/// <summary>
/// This function updates the half move clock of the given board if the given move changed it. This is called after the move was applied to the board.
/// </summary>
/// <param name="board">The board that will be checked.</param>
/// <param name="move">The move that was made that has changed the half move clock.</param>
void updateHalfMoveClock(Board& board, const Move& move);
/// <summary>
/// This function updates the move number of the given board if the given move changed it. This is called after the move was applied to the board.
/// </summary>
/// <param name="board">The board that will be checked.</param>
/// <param name="move">The move that was made that could have changed the move number.</param>
void updateMoveNumber(Board& board);
/// <summary>
/// This function updates the non-board components (casting, enpassant, halfmoveclock, movenumber) of the given board if the given move changed it. This is called after the move was applied to the board.
/// </summary>
/// <param name="board">The board that will be checked.</param>
/// <param name="move">The move that was made that could have changed the board properties.</param>
void updateBoard(Board& board, const Move& move);

/// <summary>
/// This function checks the game state of the given board.
/// </summary>
/// <param name="board">The board that will be checked for its game state.</param>
/// <returns>The game state of the board.</returns>
gameState checkGameState(const Board& board);
/// <summary>
/// This function checks if in the given board a fifty move rule can be declared.
/// </summary>
/// <param name="board">The board that will be checked for fifty move rule.</param>
/// <returns>True if fifty move rule is possible. False if not.</returns>
bool fiftyMoveCheck(const Board& board);
