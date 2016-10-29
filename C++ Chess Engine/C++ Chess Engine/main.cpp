#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include <array>
#include "move.h"
#include "board.h"
#include "defs.h"
#include "movegen.h"
#include "timer.h"
#include "evaluation.h"
#include "pgn.h"

#include "debug.h"
#include "command.h"

/******************************************************************************/
/*                               MAIN FUNCTION                                */
/******************************************************************************/
void main() {
     Board currentBoard;
     MoveList currentBoardMoveList;

     Board savedBoard[MAX_MOVENUMBER];    //  Stores Board and Board States for threefold repetition
     int savedCapturedPiece[MAX_MOVENUMBER];  //  Saved values for UNDO_MOVE command
     Move savedMove[MAX_MOVENUMBER];
     int saveIndex = 0;

     bool gamePlaying = true;
     result gameResult = NOT_FINISHED;            // Records the result of the game
     int userColor = ERRORCODE;                   // Which color user plays
     bool spectate = false;                       // if true, the game is between two computers
     LARGE_INTEGER frequency, beginTime, endTime; //  added for time performance check

     std::ofstream log;
     log.open("log.txt");
     log << "COM Search Depth: " << EVAL_DEPTH << std::endl;

     board120Setup(currentBoard);

     printSimpleBoard(currentBoard);
     printf("--------------------------------------------------\n");
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
     boardToFEN(currentBoard);
     printf("--------------------------------------------------\n");

     // Begin timer
     frequency = startTimer(&beginTime, 1);
     
     bool correctInput = false;
     std::string userCommand;

/******************************************************************************/
/*                                 MAIN LOOP                                  */
/******************************************************************************/
     currentBoard.updateEndgame();
     currentBoard.updatePieceCount();
     while (gamePlaying) {

          //  Detect Checkmate/Stalemate
          switch (checkGameState(currentBoard)) {
          case WHITE_CHECKMATE:
               gameResult = WHITE_WIN;
               gamePlaying = false;
               break;
          case BLACK_CHECKMATE:
               gameResult = BLACK_WIN;
               gamePlaying = false;
               break;
          case STALEMATE_MOVE:
               gamePlaying = false;
               gameResult = TIE;
               break;
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
          while (!correctInput && userColor == ERRORCODE) {
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
                    printDebugMenu();
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
                    LARGE_INTEGER frequency2, beginTime2, endTime2;
                    frequency2 = startTimer(&beginTime2, 2);

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
                              printf("Wrong format: correct format is [a-h][1-8][a-h][1-8].\n");
                              continue;
                         }

                         initialSquare = filerankToNumber(userCommand.at(0), userCommand.at(1)-'0');
                         terminalSquare = filerankToNumber(userCommand.at(2), userCommand.at(3)-'0');
                         
                         //  Check if Filerank format is correct
                         if (initialSquare == ERRORCODE || terminalSquare == ERRORCODE) {
                              printf("Wrong format: correct format is [char][int][char][int].\n");
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
                         if (currentBoard.isAlmostEqual(savedBoard[i])) {
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
                    savedCapturedPiece[saveIndex] = makeMove(currentBoard, userMove);
                    savedMove[saveIndex] = userMove;
                    saveIndex++;

                    // Check Fifty Move rule
                    if (fiftyMoveCheck(currentBoard)) {
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
                    log << printMove(currentBoard.getMoveNumber(), userMove);

                    stopTimer(&endTime2, 2);
                    std::cout << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms for this move.\n";
                    log << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms for this move.\n";

                    continue;
               }
               else if (commandType == DISPLAY_BOARD) {
                    printSimpleBoard(currentBoard);
                    continue;
               }
               else if (commandType == DISPLAY_FEN) {
                    boardToFEN(currentBoard);
                    continue;
               }
               else if (commandType == BOARD_RESET) {
                    board120Setup(currentBoard);
                    printSimpleBoard(currentBoard);
                    continue;
               }
               else if (commandType == PERFT) {
                    correctInput = false;
                    while (!correctInput) {
                         printf("What depth? (1~%d): ", MAX_DEPTH);
                         std::getline(std::cin, userCommand);
                         if (userCommand.size() == 0 || userCommand.at(0) - '0' < 1 || userCommand.at(0) - '0' > MAX_DEPTH) {
                              printf("Wrong Input!\n");
                              continue;
                         }
                         else {
                              printf("Perft (Depth %c): %llu\n", userCommand.at(0), divide(userCommand.at(0) - '0', 0, currentBoard, false));
                              correctInput = true;
                              break;
                         }
                    }
               }
               else if (commandType == QUIT) {
                    gamePlaying = false;
                    gameResult = NOT_FINISHED;
                    break;
               }
               else if (commandType == DIVIDE) {
                    correctInput = false;
                    while (!correctInput) {
                         printf("What depth? (1~%d): ", MAX_DEPTH);
                         std::getline(std::cin, userCommand);
                         if (userCommand.size() == 0 || userCommand.at(0) - '0' < 1 || userCommand.at(0) - '0' > MAX_DEPTH) {
                              printf("Wrong Input!\n");
                              continue;
                         }
                         else {
                              printf("Divide (Depth %c): %llu\n", userCommand.at(0), divide(userCommand.at(0) - '0', userCommand.at(0) - '0', currentBoard, true));
                              correctInput = true;
                              break;
                         }
                    }
               }
               else if (commandType == UNDO_MOVE) {
                    if (savedCapturedPiece[saveIndex] == ERRORCODE || saveIndex == 0) {
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
                         std::cout << printMove(i + 1, currentBoardMoveList.getMove(i));
                    }
                    continue;
               }
               else if (commandType == ALPHABETA_COMPARE) {
                    printf("Comparison is no longer possible. Sorry!\n");
                    continue;
               }
               else if (commandType == EVALUATE_BOARD) {
                    printf("Current Board Evaluation: %d\n", currentBoard.boardEvaluation());
                    continue;
               }
               else if (commandType == ALPHABETA_SPEED_CHECK) {
                    LARGE_INTEGER frequency2, beginTime2, endTime2;

                    frequency2 = startTimer(&beginTime2, 2);
                    int alphabetaValue = alphabeta(6, currentBoard, DEFAULT_ALPHA, DEFAULT_BETA, savedBoard, saveIndex);
                    stopTimer(&endTime2, 2);
                    printf("Alphabeta Value: %d\n", alphabetaValue);
                    std::cout << "Alphabeta timer : " << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms elapsed." << std::endl;
                    std::ofstream speedlog;
                    speedlog.open("speed.log");

                    speedlog << "Alphabeta Timer for Depth 6: " << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms." << std::endl;
                    speedlog.close();
               }
               else if (commandType == PRINT_SAVED_FEN) {
                    for (int i = 0; i < saveIndex; i++) {
                         boardToFEN(savedBoard[i]); // Print statement inside boardToFEN() prints the FEN
                    }
               }
               else if (commandType == DEBUG) {
                    // COMMIT 4445572
                    // MoveGen 6300 calls: 769 ms, BoardEval 4500 calls: 23 ms
                    // COMMIT 5edfb72
                    // PAWN 3 ms KNIGHT 8 ms BISHOP 15 ms ROOK 26ms QUEEN 28ms KING 10ms
                    // COMMIT ?
                    // ATTACKCHECK 13 ms MAKEMOVE 3 UNDOMOVE 

                    FENboardSetup(currentBoard, "8/8/8/4B3/8/3Q4/8/7K w - - 0 1");

                    MoveList moveList;
                    LARGE_INTEGER frequency2, beginTime2, endTime2;

                    frequency2 = startTimer(&beginTime2, 2);
                    for (int i = 0; i < 6300; i++) {
                         moveGeneration(currentBoard);
                    }
                    stopTimer(&endTime2, 2);
                    std::cout << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms for 6300 moveGen.\n";

                    frequency2 = startTimer(&beginTime2, 2);
                    for (int i = 0; i < 6300; i++) {
                         moveList.setCounterToZero();
                         bishopMoveGeneration(currentBoard, E5, moveList);
                    }
                    stopTimer(&endTime2, 2);
                    std::cout << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms for 6300 bishopMoveGen.\n";

                    frequency2 = startTimer(&beginTime2, 2);
                    for (int i = 0; i < 6300; i++) {
                         moveList.setCounterToZero();
                         queenMoveGeneration(currentBoard, D3, moveList);
                    }
                    stopTimer(&endTime2, 2);
                    std::cout << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms for 6300 queenMoveGen.\n";

                    
                    

               }
          }
          
          //  Computer turn
          else if (currentBoard.getTurn() == -userColor || spectate == true) {
               printf("IsEndgame: %d\n", currentBoard.getEndgame());
               
               // Reset Debug
               // TODO: Enable this again
               //moveGenCounter = 0;
               //boardEvalCounter = 0;

               LARGE_INTEGER frequency, beginTime, endTime;
               frequency = startTimer(&beginTime, 2);

               Move abMove;
               int abValue = rootAlphabeta(EVAL_DEPTH, currentBoard, DEFAULT_ALPHA, DEFAULT_BETA, abMove, savedBoard, saveIndex);
               printf("Alphabeta Value: %d\n", abValue);
               std::cout << "Alphabeta Move: " << printMove(currentBoard.getMoveNumber(), abMove);

               // Make Move, Save and Print
               savedBoard[saveIndex] = currentBoard;
               savedCapturedPiece[saveIndex] = makeMove(currentBoard, abMove);
               savedMove[saveIndex] = abMove;
               saveIndex++;

               printSimpleBoard(currentBoard);
               std::cout << printMove(currentBoard.getMoveNumber(), abMove);
               log << printMove(currentBoard.getMoveNumber(), abMove);

               //  TODO: Add 50 Move Rule option in move generation / selection?               
               // Check Fifty move rule
               if (fiftyMoveCheck(currentBoard)) {
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
                    if (currentBoard.isAlmostEqual(savedBoard[i])) {
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
               std::cout << elapsedTime(beginTime, endTime, frequency, 2) << " ms for this move.\n";
               log << elapsedTime(beginTime, endTime, frequency, 2) << " ms for this move.\n";

               // TODO: Enable this again
               //printf("Move Gen Call Count: %d\n", moveGenCounter);
               //printf("Board Eval Call Count: %d\n", boardEvalCounter);

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
     stopTimer(&endTime, 1);
     printElapsedTime(beginTime, endTime, frequency, 1);
     log << "Total Time: " << elapsedTime(beginTime, endTime, frequency, 1) << "ms" << std::endl;

     log.close();
}