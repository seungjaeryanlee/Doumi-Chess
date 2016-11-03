/******************************************************************************/
/* uci.cpp                                                                    */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#include <iostream>
#include <sstream>
#include <string>

#include "evaluation.h"
#include "board.h"

void tellUCI(std::string output) {
     std::cout << output;
}


void main() {
     // The line of command read
     std::string command;
     // A word from command
     std::string token;
     
     Board board;
     Board savedBoard[MAX_MOVENUMBER];
     int saveIndex = 0;

     while (1) {
          // 0. INITIALIZE
          token.clear();

          // 1. GET INPUT FROM UCI
          std::getline(std::cin, command);
          std::istringstream is(command);
          is >> std::skipws >> token;

          // 2. ACT BASED ON INPUT
          if (token == "quit") {
               break;
          }
          else if (token == "uci") {
               tellUCI("id name Nageune\n");
               tellUCI("id author Seung Jae (Ryan) Lee\n");
               tellUCI("uciok\n");
               // tellUCI("copyprotection ok\n");
          }
          else if (token == "isready") {
               tellUCI("readyok\n");
          }
          else if (token == "go") {
               Variation PV;
               rootAlphabeta(4, board, &PV, savedBoard, saveIndex);
               tellUCI("bestmove " + moveToString(PV.moves[0]) + "\n");
               // TODO: Check all options
          }
          else if (token == "ucinewgame") {
               board120Setup(board);
          }


          // 3. UNIMPLEMENTED TOKENS
          else if (token == "debug") {}
          else if (token == "setoption") {}
          else if (token == "register") {}
          
          else if (token == "position") {}
          else if (token == "stop") {}
          else if (token == "ponderhit") {}

     }
}