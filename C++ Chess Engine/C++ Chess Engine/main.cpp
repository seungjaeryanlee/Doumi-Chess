#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include <climits>
#include <array>
#include <ctime>
#include <iomanip>
#include "protos.h"
#include "defs.h"
#include "movegen.h"
#include "timer.h"
#include "evaluation.h"
#include "pgn.h"



/******************************************************************************/
/*                                  FUNCTIONS                                 */
/******************************************************************************/



void printDebugMenu() {
     printf("--------------------------------------------------\n");
     printf("%d: Make move\n", MOVE);
     printf("%d: Display Board\n", DISPLAY_BOARD);
     printf("%d: Display FEN\n", DISPLAY_FEN);
     printf("%d: Reset Board\n", BOARD_RESET);
     printf("%d: Perft Test\n", PERFT);
     printf("%d: Quit\n", QUIT);
     printf("%d: Divide Perft Test\n", DIVIDE);
     printf("%d: Undo move\n", UNDO_MOVE);
     printf("%d: Computer Make Move\n", COM_MAKE_MOVE);
     printf("A: Print Possible Moves\n");
     printf("B: Negamax vs. Negamax + Alphabeta Pruning\n");
     printf("C: Evaluate Board\n");
     printf("D: Alphabeta Speed Check\n");
     printf("E: Print Saved FEN\n");
     printf("--------------------------------------------------\n");
     printf("Please choose command: ");
}
void printMenu() {
     printf("--------------------------------------------------\n");
     printf("%d: Make move\n", MOVE);
     printf("%d: Display Board\n", DISPLAY_BOARD);
     printf("%d: Display FEN\n", DISPLAY_FEN);
     printf("%d: Reset Board\n", BOARD_RESET);
     printf("%d: Quit\n", QUIT);
     printf("%d: Undo move\n", UNDO_MOVE);
     printf("%d: Computer Make Move\n", COM_MAKE_MOVE);
     printf("A: Print Possible Moves\n");
     printf("C: Evaluate Board\n");
     printf("--------------------------------------------------\n");
     printf("Please choose command: ");
}

int moveGenCounter = 0;

void moveGenCalled() {
     moveGenCounter++;
}


/*                             RECURSION FUNCTIONS                             */
uint64_t divide(const int depth, const int maxDepth, Board& board, const bool showOutput) {

     if (depth == 0) { return 1; }

     uint64_t node = 0, individualNode = 0;
     int capturedPiece;

     MoveList moveList = moveGeneration(board);
     std::array<bool, 4> castlingRights = board.getCastlingRights();
     int enpassantSquare = board.getEnpassantSquare();
     int halfMoveClock = board.getHalfMoveClock();
     int moveNumber = board.getMoveNumber();
     bool isEndgame = board.getEndgame();

     if (depth == 1) { return moveList.getCounter(); }

     for (int i = 0; i <moveList.getCounter(); i++) {

          int initial = moveList.getMove(i).getInitial();
          int terminal = moveList.getMove(i).getTerminal();
                   
          capturedPiece = makeMove(board, moveList.getMove(i));
          
          node += divide(depth - 1, maxDepth, board, showOutput);
          if (showOutput) {
               individualNode = divide(depth - 1, maxDepth, board, false);
          }
          
          if (depth >= maxDepth && showOutput) {
               for (int i = 0; i < 3-depth; i++) { printf("  "); }
               printf("%c%d%c%d: %llu", numberToFile(initial), numberToRank(initial),
                    numberToFile(terminal), numberToRank(terminal), individualNode);
               printf("\n");
          }

          undoMove(board, moveList.getMove(i), capturedPiece);
          board.setCastlingRights(castlingRights);
          board.setEnpassantSquare(enpassantSquare);
          board.setHalfMoveClock(halfMoveClock);
          board.setMoveNumber(moveNumber);
          board.setEndgame(isEndgame);
     }
     return node;

}
uint64_t divide2(const int depth, const int maxDepth, Board& board, const bool showOutput) {

     if (depth == 0) { return 1; }

     //  output text file for large output
     std::ofstream output2;
     output2.open("divide.txt");

     uint64_t node = 0, individualNode = 0;
     int capturedPiece;

     MoveList moveList = moveGeneration(board);
     std::array<bool, 4> castlingRights = board.getCastlingRights();
     int enpassantSquare = board.getEnpassantSquare();
     int halfMoveClock = board.getHalfMoveClock();
     int moveNumber = board.getMoveNumber();
     bool isEndgame = board.getEndgame();

     //if (depth == 1) { return depthLegalMoveCount[depth]; }

     for (int i = 0; i < moveList.getCounter(); i++) {
          int initial = moveList.getMove(i).getInitial();
          int terminal = moveList.getMove(i).getTerminal();

          capturedPiece = makeMove(board, moveList.getMove(i));


          node += divide(depth - 1, maxDepth, board, showOutput);
          if (showOutput) {
               individualNode = divide(depth - 1, maxDepth, board, false);
          }

          if (depth >= maxDepth && showOutput) {
               output2 << numberToFile(initial) << numberToRank(initial) <<
                    numberToFile(terminal) << numberToRank(terminal) << ": " << individualNode << std::endl;
          }


          undoMove(board, moveList.getMove(i), capturedPiece);
          board.setCastlingRights(castlingRights);
          board.setEnpassantSquare(enpassantSquare);
          board.setHalfMoveClock(halfMoveClock);
          board.setMoveNumber(moveNumber);
          board.setEndgame(isEndgame);
     }
     return node;
     output2.close();
}

int makeMove(Board &board, const Move& move) {
     int capturedPiece = EMPTYSQUARE;
     int initial = move.getInitial(), terminal = move.getTerminal(), moveType = move.getType();

     board.setEnpassantSquare(0);
     board.changeTurn();

     if (moveType == NORMAL) {
          capturedPiece = board.getSquare(terminal);
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
     }
     else if (moveType == DOUBLEMOVE) {
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          board.setEnpassantSquare((terminal + initial) / 2);
     }
     else if (moveType == QUEENSIDE_CASTLING) {
          //  move king
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          //  move rook
          board.setSquare(terminal + COLUMN, board.getSquare(initial - 4 * COLUMN));
          board.setSquare(initial - 4 * COLUMN, EMPTYSQUARE);
          //  castling does not involve capture
     }
     else if (moveType == KINGSIDE_CASTLING) {
          //  move king
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          //  move rook
          board.setSquare(terminal - COLUMN, board.getSquare(terminal + COLUMN));
          board.setSquare(terminal + COLUMN, EMPTYSQUARE);
          //  castling does not involve capture
     }
     else if (moveType == KNIGHT_PROMOTION) {
          capturedPiece = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEKNIGHT);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKKNIGHT);
          }
          board.setSquare(initial, EMPTYSQUARE);
     }
     else if (moveType == BISHOP_PROMOTION) {
          capturedPiece = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEBISHOP);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKBISHOP);
          }
          board.setSquare(initial, EMPTYSQUARE);
     }
     else if (moveType == ROOK_PROMOTION) {
          capturedPiece = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEROOK);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKROOK);
          }
          board.setSquare(initial, EMPTYSQUARE);
     }
     else if (moveType == QUEEN_PROMOTION) {
          capturedPiece = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEQUEEN);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKQUEEN);
          }
          board.setSquare(initial, EMPTYSQUARE);
     }
     else if (moveType == ENPASSANT) {
          //  White turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, board.getSquare(initial));
               board.setSquare(initial, EMPTYSQUARE);
               board.setSquare(terminal + ROW, EMPTYSQUARE);
               capturedPiece = BLACKPAWN;
          }
          //  Black turn
          else {
               board.setSquare(terminal, board.getSquare(initial));
               board.setSquare(initial, EMPTYSQUARE);
               board.setSquare(terminal - ROW, EMPTYSQUARE);
               capturedPiece = WHITEPAWN;
          }
     }
     else {
          printf("Invalid moveType\n");
          return 0;
     }

     updateBoard(board, move, capturedPiece);
     return capturedPiece;
}
void undoMove(Board &board, const Move& move, const int capturedPiece) {
     int initial = move.getInitial(), terminal = move.getTerminal(), moveType = move.getType();

     board.changeTurn();

     if (moveType == NORMAL) {
          board.setSquare(initial, board.getSquare(terminal));
          board.setSquare(terminal, capturedPiece);
     }
     else if (moveType == DOUBLEMOVE) {
          board.setSquare(initial, board.getSquare(terminal));
          board.setSquare(terminal, EMPTYSQUARE);
     }
     else if (moveType == QUEENSIDE_CASTLING) {
          //  undo king move
          board.setSquare(initial, board.getSquare(terminal));
          board.setSquare(terminal, EMPTYSQUARE);

          //  undo rook move
          board.setSquare(initial - 4 * COLUMN, board.getSquare(terminal + COLUMN));
          board.setSquare(terminal + COLUMN, EMPTYSQUARE);

     }
     else if (moveType == KINGSIDE_CASTLING) {
          //  undo king move
          board.setSquare(initial, board.getSquare(terminal));
          board.setSquare(terminal, EMPTYSQUARE);

          //  undo rook move
          board.setSquare(terminal + COLUMN, board.getSquare(terminal - COLUMN));
          board.setSquare(terminal - COLUMN, EMPTYSQUARE);
     }
     else if (moveType == KNIGHT_PROMOTION || moveType == BISHOP_PROMOTION ||
          moveType == ROOK_PROMOTION || moveType == QUEEN_PROMOTION) {
          //  white turn
          if (checkColor(board.getSquare(terminal)) == WHITE) {
               board.setSquare(terminal, capturedPiece);
               board.setSquare(initial, WHITEPAWN);
          }
          //  black turn
          else {
               board.setSquare(terminal, capturedPiece);
               board.setSquare(initial, BLACKPAWN);
          }
     }
     else if (moveType == ENPASSANT) {
          //  white turn
          if (board.getSquare(terminal) == WHITEPAWN) {
               board.setSquare(terminal, EMPTYSQUARE);
               board.setSquare(initial, WHITEPAWN);
               board.setSquare(terminal + ROW, BLACKPAWN);
          }
          //  black turn
          else {
               board.setSquare(terminal, EMPTYSQUARE);
               board.setSquare(initial, BLACKPAWN);
               board.setSquare(terminal - ROW, WHITEPAWN);
          }
     }
}

/*                                  MISC                                      */
void updateCastling(Board& board, const Move& move) {
     if (board.getSquare(move.getInitial()) == WHITEKING) {
          board.setCastlingRight(WKCASTLING, false);
          board.setCastlingRight(WQCASTLING, false);
     }
     if (board.getSquare(move.getInitial()) == BLACKKING) {
          board.setCastlingRight(BKCASTLING, false);
          board.setCastlingRight(BQCASTLING, false);
     }
     if (board.getSquare(move.getInitial()) == WHITEROOK) {
          if (move.getInitial() == A1) {
               board.setCastlingRight(WQCASTLING, false);
          }
          if (move.getInitial() == H1) {
               board.setCastlingRight(WKCASTLING, false);
          }
     }
     if (board.getSquare(move.getInitial()) == BLACKROOK) {
          if (move.getInitial() == A8) {
               board.setCastlingRight(BQCASTLING, false);
          }
          if (move.getInitial() == H8) {
               board.setCastlingRight(BKCASTLING, false);
          }
     }
}
void updateEnPassant(Board& board, const Move& move) {
     if (move.getType() == DOUBLEMOVE) {
          board.setEnpassantSquare((move.getInitial() + move.getTerminal()) / 2);
     }
     else { board.setEnpassantSquare(0); }
}
void updateHalfMoveClock(Board& board, const Move& move) {
     if (board.getSquare(move.getTerminal()) == EMPTYSQUARE
          && board.getSquare(move.getInitial()) != WHITEPAWN
          && board.getSquare(move.getInitial()) != BLACKPAWN) {
          board.incrementHalfMoveClock();
     }
     else { board.setHalfMoveClock(0); }
}
void updateMoveNumber(Board& board) {
     if (board.getTurn() == WHITE) { board.incrementMoveNumber(); }
}

void updateBoard(Board& board, const Move& move, const int capturedPiece) {
     updateCastling(board, move);
     updateEnPassant(board, move);
     updateHalfMoveClock(board, move);
     board.updateEndgame(move);
     board.updatePieceCount(move, capturedPiece);
     updateMoveNumber(board);
}

gameState checkGameState(const Board& board) {
     int kingPos = -1;
     for (int i = 0; i < 120; i++) {
          if (board.getSquare(i) == WHITEKING && board.getTurn() == WHITE) {
               kingPos = i;
               break;
          }
          if (board.getSquare(i) == BLACKKING && board.getTurn() == BLACK) {
               kingPos = i;
               break;
          }
     }
     
     // Stalemate: 75 Move Rule
     // TODO: 50 Move rule will be implemented in moveGen
     if (board.getHalfMoveClock() >= 150) {
          return STALEMATE_75;
     }

     MoveList moveList = moveGeneration(board);

     // Checkmate
     if (moveList.getCounter() == 0 && isSquareAttacked(board, kingPos)) {
          if (board.getSquare(kingPos) == WHITE) {
               return BLACK_CHECKMATE;
          }
          else {
               return WHITE_CHECKMATE;
          }
     }

     // Stalemate: No legal move
     if (moveList.getCounter() == 0) {
          return STALEMATE_MOVE;
     }
     
     return NOTMATE;
}
bool fiftyMoveCheck(const Board& board) {
     return (board.getHalfMoveClock() >= 100);
}


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
     printf("Castling - WK:%d WQ:%d BK:%d BQ:%d\n", currentBoard.getCastlingRight(WKCASTLING), 
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
               moveGenCounter = 0;
               // TODO: Enable this again
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

               // Output Call Count
               printf("Move Gen Call Count: %d\n", moveGenCounter);
               // TODO: Enable this again
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