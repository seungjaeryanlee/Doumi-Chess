/******************************************************************************/
/* main.cpp                                                                   */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include <array>
#include "board.h"
#include "movegen.h"
#include "timer.h"
#include "evaluation.h"
#include "pgn.h"
#include "debug.h"

const int UNDECIDED = 2;

/// <summary>
/// This function prints the basic menu of possible user commands.
/// </summary>
void printMenu() {
     printf("--------------------------------------------------\n");
     printf("%d: Make move\n", MOVE);
     printf("%d: Display Board\n", DISPLAY_BOARD);
     printf("%d: Display FEN\n", DISPLAY_FEN);
     printf("%d: Reset Board\n", BOARD_RESET);
     printf("%d: Undo move\n", UNDO_MOVE);
     printf("%d: Computer Make Move\n", COM_MAKE_MOVE);
     printf("%d: Print Possible Moves\n", PRINT_ALL_MOVES);
     printf("%d: Evaluate Board\n", EVALUATE_BOARD);
     printf("%d: Quit\n", QUIT);
     printf("--------------------------------------------------\n");
     printf("Please choose command: ");
}

/******************************************************************************/
/*                               MAIN FUNCTION                                */
/******************************************************************************/
void main() {
     Board currentBoard;
     MoveList currentBoardMoveList;
     Board savedBoard[MAX_MOVENUMBER];    //  Stores Board for threefold repetition
     int savedCapturedPiece[MAX_MOVENUMBER];  //  Saved values for UNDO_MOVE command
     Move savedMove[MAX_MOVENUMBER];
     int saveIndex = 0;
     result gameResult = NOT_FINISHED;            // Records the result of the game
     int userColor = UNDECIDED;                   // Which color user plays
     bool spectate = false;                       // if true, the game is between two computers
     std::ofstream log;

     currentBoard.setup();
     log.open("log.txt");
     log << "===========================================================================\n";
     log << "                           NAGUENE CHESS  v1.0.0                           \n";
     log << "                            Seungjae (Ryan) Lee                            \n";
     log << "                                    LOG                                    \n";
     log << "===========================================================================\n";
     log << "COM Search Depth: " << EVAL_DEPTH << std::endl;
     
     
     printf("===========================================================================\n");
     printf("                           NAGUENE CHESS  v1.0.0                           \n");
     printf("                            Seungjae (Ryan) Lee                            \n");
     printf("===========================================================================\n");
     printSimpleBoard(currentBoard);
     printf("---------------------------------------------------------------------------\n");
     printf("Engine Search Depth: %d\n", EVAL_DEPTH);
     printf("Castling - WK:%d WQ:%d BK:%d BQ:%d\n", 
          currentBoard.getCastlingRight(WKCASTLING), 
          currentBoard.getCastlingRight(WQCASTLING), 
          currentBoard.getCastlingRight(BKCASTLING), 
          currentBoard.getCastlingRight(BQCASTLING));
     printf("en passant Square: %d\n", currentBoard.getEnpassantSquare());
     printf("Move number: %d\n", currentBoard.getMoveNumber());
     if (currentBoard.getTurn() == WHITE) { printf("Turn: White\n"); }
     else { printf("Turn: Black\n"); }
     std::cout << currentBoard.fen() << std::endl;
     printf("---------------------------------------------------------------------------\n");

     // Begin timer
     Timer programTimer;
     programTimer.start();     

/******************************************************************************/
/*                                 MAIN LOOP                                  */
/******************************************************************************/
     bool correctInput = false;
     std::string userCommand;

     currentBoard.updateEndgame();
     //currentBoard.updatePieceCount();
     
     bool gamePlaying = true;
     while (gamePlaying) {

          //  Detect Checkmate/Stalemate
          switch (checkGameState(currentBoard, savedBoard, saveIndex)) {
          case WHITE_CHECKMATE:
               gameResult = WHITE_WIN;
               gamePlaying = false;
               break;
          case BLACK_CHECKMATE:
               gameResult = BLACK_WIN;
               gamePlaying = false;
               break;
          case STALEMATE_MOVE:
          case STALEMATE_75:
               gamePlaying = false;
               gameResult = TIE;
               break;
          case NOTMATE:
               gamePlaying = true;
               gameResult = TIE;
               break;
          }
          if (!gamePlaying) { break; }
 
          //  Let user determine color to play
          correctInput = false;
          while (!correctInput && userColor == UNDECIDED) {
               printf("Which color would you like to play? (W, B or N): ");
               std::getline(std::cin, userCommand);
               if (userCommand.size() == 0) {
                    printf("You must enter W or B!\n");
                    continue;
               }
               if (userCommand.at(0) == 'W') {
                    userColor = WHITE;
                    correctInput = true;
                    log << "Player (White) vs. COM (Black)" << std::endl;
                    break;
               }
               else if (userCommand.at(0) == 'B') {
                    userColor = BLACK;
                    correctInput = true;
                    log << "COM (White) vs. Player (Black)" << std::endl;
                    break;
               }
               else if (userCommand.at(0) == 'N') {
                    spectate = true;
                    correctInput = true;
                    userColor = NEITHER;
                    log << "COM (White) vs. COM (Black)" << std::endl;
                    break;
               }
               else {
                    printf("Wrong input! Write W, B or N.\n");
                    continue;
               }
          }
          log << "---------------------------------------------------------------------------\n";

/******************************************************************************/
/*                                 GAME LOOP                                  */
/******************************************************************************/

          //  User turn
          if (currentBoard.getTurn() == userColor && spectate == false) {
               
               int initialSquare, terminalSquare;
               int commandType = ERROR_COMMAND;
               
               // Get user command
               correctInput = false;
               while (!correctInput) {
                    printMenu();
                    std::getline(std::cin, userCommand);

                    if (userCommand.size() == 0) {
                         printf("You must enter a number!\n");
                         continue;
                    }
                    if ('A' <= userCommand.at(0) && userCommand.at(0) <= 'F') {
                         commandType = userCommand.at(0) - 'A' + 10;
                         correctInput = true;
                         break;
                    }
                    commandType = userCommand.at(0) - '0';
                    if (1 <= commandType && commandType <= 9) {
                         correctInput = true;
                         break;
                    }
               }
               
               if (commandType == MOVE) {
                    Timer moveTimer;
                    moveTimer.start();

                    savedBoard[saveIndex] = currentBoard;

                    //  Movelist used for legality/movetype check
                    currentBoardMoveList = moveGeneration(currentBoard);

                    // Get user input for move
                    int moveType = NORMAL;
                    correctInput = false;
                    while (!correctInput) {
                         printf("Please enter your move: ");
                         std::getline(std::cin, userCommand);

                         //  Check size
                         if (userCommand.size() < 4) {
                              printf("Wrong format: correct format is [char][int][char][int].\n");
                              continue;
                         }

                         initialSquare = filerankToNumber(userCommand.at(0), userCommand.at(1)-'0');
                         terminalSquare = filerankToNumber(userCommand.at(2), userCommand.at(3)-'0');
                         
                         //  Check if Filerank format is correct
                         if (initialSquare == -1 || terminalSquare == -1) {
                              printf("Wrong format: correct format is [a-h][1-8][a-h][1-8].\n");
                              continue;
                         }

                         //  Check legality & movetype
                         bool legal = false;
                         for (int i = 0; i < currentBoardMoveList.getCounter(); i++) {
                              if (initialSquare == currentBoardMoveList.getMove(i).getInitial() && terminalSquare == currentBoardMoveList.getMove(i).getTerminal()) {
                                   legal = true;
                                   moveType = currentBoardMoveList.getMove(i).getType();
                                   break;
                              }
                         }
                         if (legal == false) {
                              printf("The move is illegal!\n");
                              continue;
                         }
                         else {
                              correctInput = true;
                         }
                    }
                    // Check Promotion
                    if (currentBoard.getSquare(initialSquare) == WHITEPAWN && A8 <= terminalSquare && terminalSquare <= H8 ||
                         currentBoard.getSquare(initialSquare) == BLACKPAWN && A1 <= terminalSquare && terminalSquare <= H1) {
                         correctInput = false;
                         while (!correctInput) {
                              printf("Please pick a piece to promote to (N, B, R, Q): ");
                              std::getline(std::cin, userCommand); // do I want to get the entire command?

                              //  Check size
                              if (userCommand.size() != 1) {
                                   printf("Just one letter: N, B, R, Q\n");
                                   continue;
                              }
                              if (userCommand.at(0) != 'N' && userCommand.at(0) != 'B' && userCommand.at(0) != 'R' && userCommand.at(0) != 'Q') {
                                   printf("Wrong letter: just N, B, R, Q\n");
                                   continue;
                              }

                              switch (userCommand.at(0)) {
                              case 'N':
                                   moveType = KNIGHT_PROMOTION;
                                   correctInput = true;
                                   break;
                              case 'B':
                                   moveType = BISHOP_PROMOTION;
                                   correctInput = true;
                                   break;
                              case 'R':
                                   moveType = ROOK_PROMOTION;
                                   correctInput = true;
                                   break;
                              case 'Q':
                                   moveType = QUEEN_PROMOTION;
                                   correctInput = true;
                                   break;
                              }
                         }
                    }

                    // Check Threefold repetition
                    int repetitionCount = 0;
                    for (int i = 0; i < saveIndex; i++) {
                         if (currentBoard.isEqual_3F(savedBoard[i])) {
                              repetitionCount++;
                         }
                         if (repetitionCount >= 3) { break; }
                    }

                    if (repetitionCount >= 3) {
                         correctInput = false;
                         bool declareTie = false;
                         while (!correctInput) {
                              printf("Declare Threefold Repetition? (Y/N):");
                              std::getline(std::cin, userCommand);
                              if (userCommand.size() == 0 || (userCommand.at(0) != 'Y' && userCommand.at(0) != 'N')) {
                                   printf("Wrong Input!\n");
                                   continue;
                              }
                              else {
                                   correctInput = true;
                                   if (userCommand.at(0) == 'Y') {
                                        declareTie = true;
                                   }
                                   break;
                              }
                         }
                         if (declareTie) {
                              gamePlaying = false;
                              gameResult = TIE;
                              break;
                         }
                    }
                    Move userMove = Move(initialSquare, terminalSquare, moveType);
                    savedCapturedPiece[saveIndex] = currentBoard.makeMove(userMove);
                    savedMove[saveIndex] = userMove;
                    saveIndex++;

                    // Check Fifty Move rule
                    if (currentBoard.fiftyMoveCheck()) {
                         bool correctInput = false, declareTie = false;
                         while (!correctInput) {
                              printf("Declare Fifty Move Rule? (Y/N):");
                              std::getline(std::cin, userCommand);
                              if (userCommand.size() == 0 || (userCommand.at(0) != 'Y' && userCommand.at(0) != 'N')) {
                                   printf("Wrong Input!\n");
                                   continue;
                              }
                              else {
                                   correctInput = true;
                                   if (userCommand.at(0) == 'Y') {
                                        declareTie = true;
                                   }
                                   break;
                              }
                         }
                         if (declareTie) {
                              gamePlaying = false;
                              gameResult = TIE;
                              break;
                         }
                    }

                    if (currentBoard.getTurn() == WHITE) { currentBoard.incrementMoveNumber(); }
                    
                    // add to log file
                    log << currentBoard.getMoveNumber() << ": " << userMove.toString_vertose();

                    printSimpleBoard(currentBoard);
                    moveTimer.stop();
                    std::cout << moveTimer.duration() << " ms for this move.\n";
                    log << moveTimer.duration() << " ms for this move.\n";
                    printf("---------------------------------------------------------------------------\n");

                    continue;
               }
               else if (commandType == DISPLAY_BOARD) {
                    printSimpleBoard(currentBoard);
                    continue;
               }
               else if (commandType == DISPLAY_FEN) {
                    std::cout << currentBoard.fen() << std::endl;
                    continue;
               }
               else if (commandType == BOARD_RESET) {
                    currentBoard.setup();
                    printSimpleBoard(currentBoard);
                    continue;
               }
               else if (commandType == UNDO_MOVE) {
                    if (saveIndex == 0) {
                         printf("No move can be undone!\n");
                         continue;
                    }
                    else {
                         saveIndex--;
                         currentBoard = savedBoard[saveIndex];
                         userColor = -userColor;
                    }
               }
               else if (commandType == COM_MAKE_MOVE) {
                    userColor = -userColor;
                    continue;
               }
               else if (commandType == PRINT_ALL_MOVES) {
                    currentBoardMoveList = moveGeneration(currentBoard);

                    printf("Movecount: %d\n", currentBoardMoveList.getCounter());
                    for (int i = 0; i <  currentBoardMoveList.getCounter(); i++) {
                         std::cout << i + 1 << ": "<< currentBoardMoveList.getMove(i).toString_vertose();
                    }
                    continue;
               }
               else if (commandType == EVALUATE_BOARD) {
                    printf("Naguene's Board Evaluation: %d\n", currentBoard.evaluate());
                    continue;
               }
               else if (commandType == QUIT) {
                    gamePlaying = false;
                    gameResult = NOT_FINISHED;
                    break;
               }
          }
          
          //  Computer turn
          else if (currentBoard.getTurn() == -userColor || spectate == true) {

               Timer moveTimer;

               Variation PV;
               int abValue = rootAlphabeta(EVAL_DEPTH, currentBoard, &PV, savedBoard, saveIndex);
               printf("Alphabeta Value: %d\n", abValue);
               printf("Alphabeta PV: ");
               printVariation(std::cout, PV);
               log << "Alphabeta PV: ";
               printVariation(log, PV);
               std::cout << currentBoard.getMoveNumber() << ": " << PV.moves[0].toString_vertose();
               log << currentBoard.getMoveNumber() << ": " << PV.moves[0].toString_vertose();

               // Make Move, Save and Print
               savedBoard[saveIndex] = currentBoard;
               savedCapturedPiece[saveIndex] = currentBoard.makeMove(PV.moves[0]);
               savedMove[saveIndex] = PV.moves[0];
               saveIndex++;

               printSimpleBoard(currentBoard);

               // Check Fifty move rule
               if (currentBoard.fiftyMoveCheck()) {
                    // If in bad position, declare fifty move rule
                    if (abValue <= STALEMATE_BOUND) {
                         printf("Computer declares Fifty Move Rule.\n");
                         log << "Computer declares Fifty Move Rule." << std::endl;
                         gamePlaying = false;
                         gameResult = TIE;
                         break;
                    }
               }

               // Check Threefold repetition
               int repetitionCount = 0;
               for (int i = 0; i < saveIndex; i++) {
                    if (currentBoard.isEqual_3F(savedBoard[i])) {
                         repetitionCount++;
                    }
                    if (repetitionCount >= 3) { break; }
               }
               if (repetitionCount >= 3) {
                    if (abValue <= STALEMATE_BOUND) {
                         printf("Computer declares Threefold Repetition.\n");
                         log << "Computer declares Threefold Repetition." << std::endl;
                         gamePlaying = false;
                         gameResult = TIE;
                         break;
                    }
               }

               stopTimer(&endTime, 2);
               moveTimer.stop();
               std::cout << moveTimer.duration() << " ms for this move.\n";
               log << moveTimer.duration() << " ms for this move.\n";
               printf("---------------------------------------------------------------------------\n");
          }
     }
     savePGN(gameResult, savedMove, saveIndex, spectate, userColor);

     // Print Game Result
     switch (gameResult) {
     case BLACK_WIN:
          printf("Game Result: 0-1\n");
          log << "Game Result: 0-1" << std::endl;
          break;
     case TIE:
          printf("Game Result: 1/2-1/2\n");
          log << "Game Result: 1/2-1/2" << std::endl;
          break;
     case WHITE_WIN:
          printf("Game Result: 1-0\n");
          log << "Game Result: 1-0" << std::endl;
          break;
     case NOT_FINISHED:
          printf("Game Result: *: Game not finished\n");
          log << "Game Result: *: Game not finished" << std::endl;
     }

     //  Stop timer and print elapsed time
     programTimer.stop();
     std::cout << "Program Duration: " << programTimer.duration() << " ms elapsed." << std::endl;

     log << "Program Duration: " << programTimer.duration() << "ms" << std::endl;

     log.close();
}