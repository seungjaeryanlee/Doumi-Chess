/******************************************************************************/
/* uci.cpp                                                                    */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#include <iostream>
#include <sstream>
#include <string>

void main() {
     std::string token;
     std::string command;
     
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
               std::cout << "id name Nageune\n";
               std::cout << "id author Seung Jae (Ryan) Lee\n";
               std::cout << "uciok\n";
          }
     }
}