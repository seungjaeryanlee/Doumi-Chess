#pragma once
#include "move.h"
#include <string>

/******************************************************************************/
/* MOVE CLASS                                                                 */
/******************************************************************************/
// Constructors
Move::Move() {}
Move::Move(int i, int t, int m) {
     initialSquare = i;
     terminalSquare = t;
     moveType = m;
}

// Clone Methods
Move::Move(const Move& originalMove) {
     initialSquare = originalMove.getInitial();
     terminalSquare = originalMove.getTerminal();
     moveType = originalMove.getType();
}
Move::Move(int originalMove[3]) {
     initialSquare = originalMove[0];
     terminalSquare = originalMove[1];
     moveType = originalMove[2];
}

// Mutators
inline void Move::setInitial(const int i) { initialSquare = i; }
inline void Move::setTerminal(const int t) { terminalSquare = t; }
inline void Move::setType(const int t) { moveType = t; }
inline void Move::setCapturedPiece(const int c) { capturedPiece = c; }
// Accessors
inline const int Move::getInitial() const { return initialSquare; }
inline const int Move::getTerminal() const { return terminalSquare; }
inline const int Move::getType() const { return moveType; }
inline const int Move::getCapturedPiece() const { return capturedPiece; }

inline std::string numberToFilerank(const int position) {
     return numberToFile(position) + std::to_string(numberToRank(position));
}
inline char numberToFile(const int position) {
     return ('a' + position % ROW - 1);
}
inline int numberToRank(const int position) {
     return (10 - position / ROW);
}
std::string printMove(const int moveNumber, const Move& move) {
     std::string output = std::to_string(moveNumber) + ": " + numberToFilerank(move.getInitial()) + " "
          + numberToFilerank(move.getTerminal()) + " ";

     switch (move.getType()) {
     case NORMAL:
          output += "\n";
          break;
     case DOUBLEMOVE:
          output += "Double Move\n";
          break;
     case ENPASSANT:
          output += "En passant\n";
          break;
     case QUEENSIDE_CASTLING:
          output = std::to_string(moveNumber) + ": O-O-O\n";
          break;
     case KINGSIDE_CASTLING:
          output = std::to_string(moveNumber) + ": O-O\n";
          break;
     case KNIGHT_PROMOTION:
          output += "Promotion to Knight\n";
          break;
     case BISHOP_PROMOTION:
          output += "Promotion to Bishop\n";
          break;
     case ROOK_PROMOTION:
          output += "Promotion to Rook\n";
          break;
     case QUEEN_PROMOTION:
          output += "Promotion to Queen\n";
          break;
     case DECLARE_50:
          break;
     }

     return output;
}