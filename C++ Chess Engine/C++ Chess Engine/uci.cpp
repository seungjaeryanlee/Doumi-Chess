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
          else if (token == "position") {
               std::string option;
               is >> std::skipws >> option;
               
               // Find Starting position
               if (option == "startpos") {
                    board120Setup(board);
               }
               else if (option == "fen") { // FEN is given
                    FENboardSetup(board, option);
               }
               else { return; }

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
                              // TODO: Give correct moveType?
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
                         else { return; }

                         // Make move on the board
                         makeMove(board, move);
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
               boardToFEN(board);
          }

     }
}