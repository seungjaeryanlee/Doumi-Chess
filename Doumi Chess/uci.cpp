/******************************************************************************/
/* uci.cpp                                                                    */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "move.h"
#include "board.h"
#include "evaluation.h"

void logInput(std::ostream& ucilog, std::string input) {
     ucilog << "I " << input << std::endl;
     ucilog.flush();
}

void tellUCI(std::ostream& ucilog, std::string output) {
     std::cout << output;
     ucilog << "O " << output;
     ucilog.flush();
}

int main() {
     std::ofstream ucilog;
     ucilog.open("uci.log");

     std::string command;  // The line of command read
     std::string token;    // A word from command
     
     Board board;
     Board savedBoard[MAX_MOVENUMBER];
     int saveIndex = 0;

     while (1) {
          // 0. INITIALIZE
          token.clear();

          // 1. GET INPUT FROM UCI
          std::getline(std::cin, command);
          logInput(ucilog, command);
          std::istringstream is(command);
          is >> std::skipws >> token;

          // 2. ACT BASED ON INPUT
          if (token == "quit") {
               break;
          }
          else if (token == "uci") {
               tellUCI(ucilog, "id name Nageune\n");
               tellUCI(ucilog, "id author Seung Jae (Ryan) Lee\n");
               tellUCI(ucilog, "uciok\n");
               // tellUCI("copyprotection ok\n");
          }
          else if (token == "isready") {
               tellUCI(ucilog, "readyok\n");
          }
          else if (token == "go") {

               // FIXME: Check all options
               // FIXME: Implement PV ordering
               Variation PV;
               for (int depth = 1; depth <= 4; depth++) {
                    int score = rootAlphabeta(depth, board, &PV, savedBoard, saveIndex);
                    tellUCI(ucilog, "info depth " + std::to_string(depth) + " score " + std::to_string(score) + " pv ");
                    for (int i = 0; i < depth; i++) {
                         tellUCI(ucilog, PV.moves[i].toString() + " ");
                    }
                    tellUCI(ucilog, "\n");
               }
               tellUCI(ucilog, "bestmove " + PV.moves[0].toString() + "\n");

               
          }
          else if (token == "ucinewgame") {
               board.setup();
          }
          else if (token == "position") {
               std::string option;
               is >> std::skipws >> option;
               
               // Find Starting position
               if (option == "startpos") {
                    board.setup();
               }
               else if (option == "fen") { // FEN is given
                    board.import(option);
               }
               else { return 0; }

               // Check if there are any moves given
               is >> std::skipws >> option;
               if (option == "moves") {
                    // Do given moves to the board
                    std::string moveString;
                    Move move;
                    while (is >> moveString) {
                         // Save Board
                         savedBoard[saveIndex] = board;
                         saveIndex++;

                         // Interpret String
                         move.setInitial(filerankToNumber(moveString[0], moveString[1] - '0'));
                         move.setTerminal(filerankToNumber(moveString[2], moveString[3] - '0'));

                         if (moveString.length() == 4) {
                              // Unless promotion, no need to have correct type
                              move.setType(NORMAL);
                         }
                         else if (moveString[4] == 'n') {
                              move.setType(KNIGHT_PROMOTION);
                         }
                         else if (moveString[4] == 'b') {
                              move.setType(BISHOP_PROMOTION);
                         }
                         else if (moveString[4] == 'r') {
                              move.setType(ROOK_PROMOTION);
                         }
                         else if (moveString[4] == 'q') {
                              move.setType(QUEEN_PROMOTION);
                         }
                         else { return 0; }

                         // Make move on the board
                         board.makeMove(move);
                    }
               }
          }

          // 3. UNIMPLEMENTED TOKENS
          else if (token == "debug") {}
          else if (token == "setoption") {}
          else if (token == "register") {}
          else if (token == "stop") {}
          else if (token == "ponderhit") {}

          // 4. DEBUGGING TOKENS (NOT FOR UCI)
          else if (token == "printfen") {
               std::cout << board.fen() << std::endl;
          }
     }

     return 0;
}