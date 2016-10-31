#pragma once
#include "move.h"
#include <string>


std::string numberToFilerank(const int position) {
     return numberToFile(position) + std::to_string(numberToRank(position));
}
char numberToFile(const int position) {
     return ('a' + position % ROW - 1);
}
int numberToRank(const int position) {
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