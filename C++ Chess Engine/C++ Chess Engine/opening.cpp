#include "opening.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <random>

Move checkOpeningBook(const Board& board) {
     std::ifstream book("opening.book");
     std::string fen;
     std::string moves;
     while (std::getline(book, fen) && std::getline(book, moves)) {
          std::istringstream is(moves);
          
          // TODO: Disable output on boardToFEN
          if (fen == boardToFEN(board)) {
               int variationCount;
               is >> variationCount;

               std::mt19937 rng;
               rng.seed(std::random_device()());
               std::uniform_int_distribution<std::mt19937::result_type> distribution(1, variationCount);

               int chosenIndex = distribution(rng);
               std::string move;
               for (int i = 0; i < chosenIndex; i++) {
                    is >> move;
               }
               return stringToMove(move);
          }

     }
     // If no move available, return illegal move
     return Move(-1,-1,-1);
}

Move stringToMove(std::string string) {
     Move move;
     move.setInitial(filerankToNumber(string[0], string[1] - '0'));
     move.setTerminal(filerankToNumber(string[2], string[3] - '0'));
     // TODO: should be not needed
     move.setType(NORMAL);
     return move;
}