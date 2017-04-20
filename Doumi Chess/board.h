/******************************************************************************/
/* board.h                                                                    */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#pragma once
#include <array>
#include "move.h"

class Board {
private:
     std::array<int, 120> board;
     std::array<bool, 4> castlingRights;
     color turn;
     int enpassantSquare;
     int halfMoveClock;
     int moveNumber;

     bool isEndgame = false;
     //std::array<int, 14> pieceCount;

public:
     // Default Constructor
     Board();

     // Constructor adding all content at once
     Board(std::array<int, 120> b, std::array<bool, 4> cc, color t, int e, int f, int m);

     //  Clone Method
     Board(const Board& originalBoard);
     
     //  equality for threefold repetition checking
     bool isEqual_3F(const Board& thatBoard);

     //  Mutators
     inline void setBoard(const std::array<int, 120> b) { board = b; }
     inline void setSquare(const int square, const int value) { board.at(square) = value; }
     inline void setCastlingRights(const std::array<bool, 4> cc) { castlingRights = cc; }
     inline void setCastlingRight(const int index, const bool value) { castlingRights.at(index) = value; }
     inline void setTurn(const color t) { turn = t; }
     inline void setEnpassantSquare(const int e) { enpassantSquare = e; }
     inline void setHalfMoveClock(const int f) { halfMoveClock = f; }
     inline void setMoveNumber(const int m) { moveNumber = m; }
     inline void setEndgame(bool e) { isEndgame = e; }
     //void setPieceCount(const std::array<int, 14> pc) { pieceCount = pc; }

     //  Accessors
     inline const std::array<int, 120> getBoard() const { return board; }
     inline const int getSquare(int square) const { return board.at(square); }
     inline const std::array<bool, 4> getCastlingRights() const { return castlingRights; }
     inline const bool getCastlingRight(int index) const { return castlingRights.at(index); }
     inline const color getTurn() const { return turn; }
     inline const int getEnpassantSquare() const { return enpassantSquare; }
     inline const int getHalfMoveClock() const { return halfMoveClock; }
     inline const int getMoveNumber() const { return moveNumber; }
     inline const bool getEndgame() const { return isEndgame; }
     //const std::array<int, 14> getPieceCount() const { return pieceCount; }

     //  Other Functions
     inline void changeTurn() { turn = (color)-turn; }
     inline void incrementHalfMoveClock() { halfMoveClock++; }
     inline void decrementHalfMoveClock() { halfMoveClock--; }
     inline void incrementMoveNumber() { moveNumber++; }
     inline void decrementMoveNumber() { moveNumber--; }

     void updateEndgame();
     void updateEndgame(Move move);
    
     // void updatePieceCount();

     /// <summary>
     /// This function returns evaluation score of the board using piece values and PCSQ tables. Positive score signifies white's advantage.
     /// </summary>
     /// <returns>The score of the board</returns>
     int evaluate();

     /// <summary>
     /// This function sets the board to the initial state.
     /// </summary>
     void setup();

     /// <summary>
     /// This function sets the board according to the FEN.
     /// </summary>
     /// <param name="FEN">The desired board in FEN notation.</param>
     void import(const std::string FEN);

     /// <summary>
     /// This function converts the board to FEN notation.
     /// </summary>
     /// <returns>FEN notation of the board.</returns>
     std::string boardToFEN();
};

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
//TODO: Re-implement and use
void undoMove(Board &board, const Move& move, const int capturedPiece);


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
// TODO: UPDATE
void updateBoard(Board& board, const Move& move, const int capturedPiece);

/// <summary>
/// This function checks if in the given board a fifty move rule can be declared.
/// </summary>
/// <param name="board">The board that will be checked for fifty move rule.</param>
/// <returns>True if fifty move rule is possible. False if not.</returns>
bool fiftyMoveCheck(const Board& board);

