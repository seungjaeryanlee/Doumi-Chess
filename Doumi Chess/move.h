/******************************************************************************/
/* move.h                                                                     */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#pragma once
#include <array>
#include "defs.h"

class Move {
private:
     int initialSquare;
     int terminalSquare;
     int moveType;
     int capturedPiece;
public:
     // Constructors
     Move();
     Move(int i, int t, int m);

     // Clone Methods
     Move(const Move& originalMove);
     Move(int originalMove[3]);

     // Mutators
     inline void setInitial(const int i) { initialSquare = i; }
     inline void setTerminal(const int t) { terminalSquare = t; }
     inline void setType(const int t) { moveType = t; }
     inline void setCapturedPiece(const int c) { capturedPiece = c; }
     // Accessors
     inline const int getInitial() const { return initialSquare; }
     inline const int getTerminal() const { return terminalSquare; }
     inline const int getType() const { return moveType; }
     inline const int getCapturedPiece() const { return capturedPiece; }
     
     std::string toString(const Move& move);

};
class MoveList {
private:
     std::array<Move, MAX_MOVEGEN_COUNT> movelist;
     int moveCounter;
public:
     // Constructor
     MoveList();

     // Accessor
     inline std::array<Move, MAX_MOVEGEN_COUNT> getList() { return movelist; }
     inline Move getMove(int index) { return movelist[index]; }
     inline int getCounter() { return moveCounter; }

     // Mutator
     void push(const Move& move);
     /// <summary>
     /// This function adds a given move to the movelist.
     /// </summary>
     /// <param name="initial">The initial square of the move in board index notation.</param>
     /// <param name="terminal">The terminal square of the move in board index notation.</param>
     /// <param name="moveType">The type of the move.</param>
     void push(int initial, int terminal, int moveType);
     /// <summary>
     /// This function adds four promotion moves from the initial square to the terminal square to the movelist.
     /// </summary>
     /// <param name="initial">The initial square of the moves in board index notation.</param>
     /// <param name="terminal">The terminal square of the moves in board index notation.</param>
     void pushPromotion(int initial, int terminal);
};

/// <summary>
/// This function converts a position's notation from board index to file rank.
/// </summary>
/// <param name="position">The square in board index notation.</param>
/// <returns>A two-letter string with file rank notation of the given square.</returns>
std::string numberToFilerank(const int position);

/// <summary>
/// This function prints the given move to console in filerank-filerank notation.
/// </summary>
/// <param name="moveNumber">The move number of the move that will be printed to console</param>
/// <param name="move">The move that will be printed to console</param>
std::string printMove(const int moveNumber, const Move& move);

