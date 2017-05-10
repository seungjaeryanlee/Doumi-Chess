/******************************************************************************/
/* debugmain.cpp                                                              */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <chrono>
#include <iomanip>

#include "timer.h"
#include "board.h"
#include "movegen.h"
#include "evaluation.h"
#include "pgn.h"
#include "debug.h"

// FIXME: Add Documentation
const int MOVEGEN_TEST_COUNT = 10000;
const int EVALUATE_TEST_COUNT = 10000;
const int COPYBOARD_TEST_COUNT = 10000;
const int MAKEMOVE_TEST_COUNT = 10000;
const int MAKEUNDO_TEST_COUNT = 10000;
const int CHECKGAMESTATE_TEST_COUNT = 10000;
const int ALPHABETA_TEST_COUNT[6] = { 0, 1000, 200, 50, 10, 1 };

const int FUNCTION_WS_LENGTH = 30;
const int TIME_WS_LENGTH = 10;

typedef std::chrono::high_resolution_clock Clock;

// FIXME: Global variables for clarity?
int passCount = 0;
int failCount = 0;

// FIXME: Add Documentation
void REQUIRE(std::string description, bool condition) {
     if (condition) { std::cout << "   SUCCESS   "; passCount++; }
     else           { std::cout << "   FAILURE   "; failCount++; }
     std::cout << description << std::endl;
}

// FIXME: Add Documentation
void test_BoardClass() {

     std::cout << "BOARD CLASS" << std::endl << std::endl;

     Board board;

     // Test 1
     board.setup();
     REQUIRE("Board::setup() with Board::fen() should return initial FEN",
             board.fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

     // Test 2
     board.import("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
     REQUIRE("Board::fen() should return FEN given with Board::import() 1",
          board.fen() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

     // Test 3
     board.import("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
     REQUIRE("Board::fen() should return FEN given with Board::import() 2",
          board.fen() == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
     
     // Test 4
     board.setup();
     REQUIRE("Board::evaluate() should return 0 in initial board",
          board.evaluate() == 0);

     std::cout << std::endl;
}

void test_Perft() {
     // Perft results from https://chessprogramming.wikispaces.com/Perft+Results

     ////////////////////////////////////////////////////////////////////////////////
     
     std::cout << "PERFT rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" << std::endl << std::endl;
     Board board1;
     
     // Test 1
     board1.setup();
     REQUIRE("Perft of depth 1 should give 20 moves",
             divide(1, 0, board1, false) == 20);

     // Test 2
     board1.setup();
     REQUIRE("Perft of depth 2 should give 400 moves",
             divide(2, 0, board1, false) == 400);

     // Test 3
     board1.setup();
     REQUIRE("Perft of depth 3 should give 8902 moves",
             divide(3, 0, board1, false) == 8902);

     // Test 4
     board1.setup();
     REQUIRE("Perft of depth 4 should give 197281 moves",
             divide(4, 0, board1, false) == 197281);

     std::cout << std::endl;

     ////////////////////////////////////////////////////////////////////////////////
     
     std::cout << "PERFT r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1" << std::endl << std::endl;
     Board board2;

     // Test 1
     board2.import("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
     REQUIRE("Perft of depth 1 should give 48 moves",
             divide(1, 0, board2, false) == 48);

     // Test 2
     board2.import("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
     REQUIRE("Perft of depth 2 should give 2039 moves",
             divide(2, 0, board2, false) == 2039);

     // Test 3
     board2.import("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
     REQUIRE("Perft of depth 3 should give 97862 moves",
             divide(3, 0, board2, false) == 97862);

     // Test 4
     board2.import("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
     REQUIRE("Perft of depth 4 should give 4085603 moves",
             divide(4, 0, board2, false) == 4085603);

     std::cout << std::endl;

     ////////////////////////////////////////////////////////////////////////////////
     
     std::cout << "PERFT 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1" << std::endl << std::endl;
     Board board3;

     // Test 1
     board3.import("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
     REQUIRE("Perft of depth 1 should give 14 moves",
             divide(1, 0, board3, false) == 14);

     // Test 2
     board3.import("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
     REQUIRE("Perft of depth 2 should give 191 moves",
             divide(2, 0, board3, false) == 191);

     // Test 3
     board3.import("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
     REQUIRE("Perft of depth 3 should give 2812 moves",
             divide(3, 0, board3, false) == 2812);

     // Test 4
     board3.import("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
     REQUIRE("Perft of depth 4 should give 43238 moves",
             divide(4, 0, board3, false) == 43238);

     std::cout << std::endl;

     ////////////////////////////////////////////////////////////////////////////////
     
     std::cout << "PERFT r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" << std::endl << std::endl;
     Board board4;

     // Test 1
     board4.import("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
     REQUIRE("Perft of depth 1 should give 6 moves",
             divide(1, 0, board4, false) == 6);

     // Test 2
     board4.import("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
     REQUIRE("Perft of depth 2 should give 264 moves",
             divide(2, 0, board4, false) == 264);

     // Test 3
     board4.import("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
     REQUIRE("Perft of depth 3 should give 9467 moves",
             divide(3, 0, board4, false) == 9467);

     // Test 4
     board4.import("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
     REQUIRE("Perft of depth 4 should give 422333 moves",
             divide(4, 0, board4, false) == 422333);

     std::cout << std::endl;

     ////////////////////////////////////////////////////////////////////////////////
     
     std::cout << "PERFT rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" << std::endl << std::endl;
     Board board5;

     // Test 1
     board5.import("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
     REQUIRE("Perft of depth 1 should give 44 moves",
             divide(1, 0, board5, false) == 44);

     // Test 2
     board5.import("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
     REQUIRE("Perft of depth 2 should give 1486 moves",
             divide(2, 0, board5, false) == 1486);

     // Test 3
     board5.import("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
     REQUIRE("Perft of depth 3 should give 62379 moves",
             divide(3, 0, board5, false) == 62379);

     // Test 4
     board5.import("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
     REQUIRE("Perft of depth 4 should give 2103487 moves",
             divide(4, 0, board5, false) == 2103487);

     std::cout << std::endl;

     ////////////////////////////////////////////////////////////////////////////////
     
     std::cout << "PERFT r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10" << std::endl << std::endl;
     Board board6;

     // Test 1
     board6.import("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
     REQUIRE("Perft of depth 1 should give 46 moves",
             divide(1, 0, board6, false) == 46);

     // Test 2
     board6.import("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
     REQUIRE("Perft of depth 2 should give 2079 moves",
             divide(2, 0, board6, false) == 2079);

     // Test 3
     board6.import("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
     REQUIRE("Perft of depth 3 should give 89890 moves",
             divide(3, 0, board6, false) == 89890);

     // Test 4
     board6.import("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
     REQUIRE("Perft of depth 4 should give 3894594 moves",
             divide(4, 0, board6, false) == 3894594);

     std::cout << std::endl;




}

void test_GameState() {
     std::cout << "GAME STATE" << std::endl << std::endl;
     

     // Test 1
     Board board1;
     Board savedBoard1[MAX_MOVENUMBER];
     int saveIndex1 = 0;
     board1.setup();
     MoveList moveList1 = moveGeneration(board1);

     REQUIRE("Initial board state should be NOTMATE",
             checkGameState(board1, moveList1, savedBoard1, saveIndex1) == NOTMATE);

     // Test 2
     Board board2;
     Board savedBoard2[MAX_MOVENUMBER];
     int saveIndex2 = 0;
     board2.import("6bk/5p1p/5P1P/8/8/8/8/4K3 b - - 0 1");
     MoveList moveList2 = moveGeneration(board2);

     REQUIRE("STALEMATE_MOVE should be detected",
             checkGameState(board2, moveList2, savedBoard2, saveIndex2) == STALEMATE_MOVE);

     // Test 3
     Board board3;
     Board savedBoard3[MAX_MOVENUMBER];
     int saveIndex3 = 0;
     board3.import("7k/pppppppp/8/8/8/8/PPPPPPPP/K7 w - - 100 1");
     MoveList moveList3 = moveGeneration(board3);


     REQUIRE("STALEMATE_50 should be detected",
             checkGameState(board3, moveList3, savedBoard3, saveIndex3) == STALEMATE_50);

     // Test 4
     Board board4;
     Board savedBoard4[MAX_MOVENUMBER];
     int saveIndex4 = 0;
     board4.import("7k/pppppppp/8/8/8/8/PPPPPPPP/K7 w - - 150 1");
     MoveList moveList4 = moveGeneration(board4);

     REQUIRE("STALEMATE_75 should be detected",
             checkGameState(board4, moveList4, savedBoard4, saveIndex4) == STALEMATE_75);

     // Test 5
     Board board5;
     Board savedBoard5[MAX_MOVENUMBER];
     int saveIndex5 = 3;
     board5.import("7k/pppppppp/8/8/8/8/PPPPPPPP/K7 w - - 0 1");
     MoveList moveList5 = moveGeneration(board5);

     savedBoard5[0] = board5;
     savedBoard5[1] = board5;
     savedBoard5[2] = board5; // FIXME: Is this needed?
     
     REQUIRE("STALEMATE_3F should be detected",
             checkGameState(board5, moveList5, savedBoard5, saveIndex5) == STALEMATE_3F);

     // Test 6
     Board board6;
     Board savedBoard6[MAX_MOVENUMBER];
     int saveIndex6 = 0;
     board6.import("R6k/R7/8/8/8/8/8/7K b - - 0 1");
     MoveList moveList6 = moveGeneration(board6);

     REQUIRE("WHITE_CHECKMATE should be detected",
             checkGameState(board6, moveList6, savedBoard6, saveIndex6) == WHITE_CHECKMATE);

     std::cout << std::endl;
}

void time_MoveGen() {
     Board board;  board.setup();
     Timer timer;  timer.start();

     for (int i = 0; i < MOVEGEN_TEST_COUNT; i++) {
          moveGeneration(board);
     }

     timer.stop();

     std::cout << std::left << std::setw(FUNCTION_WS_LENGTH) << "  MOVEGEN" << std::right << std::setw(TIME_WS_LENGTH) << timer.duration_nano() / MOVEGEN_TEST_COUNT << " ns. (Average of " << MOVEGEN_TEST_COUNT << " trials)" << std::endl;
}

void time_Evaluate() {
     Board board;  board.setup();
     Timer timer;  timer.start();

     for (int i = 0; i < EVALUATE_TEST_COUNT; i++) {
          board.evaluate();
     }

     timer.stop();
     std::cout << std::left << std::setw(FUNCTION_WS_LENGTH) << "  EVALUATE" << std::right << std::setw(TIME_WS_LENGTH) << timer.duration_nano() / EVALUATE_TEST_COUNT << " ns. (Average of " << EVALUATE_TEST_COUNT << " trials)" << std::endl;
}

void time_CopyBoard() {
     Board board;  board.setup();
     Timer timer;  timer.start();

     for (int i = 0; i < COPYBOARD_TEST_COUNT; i++) {
          Board newBoard = board;
     }

     timer.stop();
     std::cout << std::left << std::setw(FUNCTION_WS_LENGTH) << "  COPYBOARD" << std::right << std::setw(TIME_WS_LENGTH) << timer.duration_nano() / COPYBOARD_TEST_COUNT << " ns. (Average of " << COPYBOARD_TEST_COUNT << " trials)" << std::endl;
}

void time_MakeMove() {
     Board board;  board.setup();
     Timer timer;  timer.start();

     for (int i = 0; i < MAKEMOVE_TEST_COUNT; i++) {
          board.makeMove(Move(B1, C3, NORMAL));
          board.makeMove(Move(C3, B1, NORMAL));
     }

     timer.stop();
     std::cout << std::left << std::setw(FUNCTION_WS_LENGTH) << "  MAKEMOVE" << std::right << std::setw(TIME_WS_LENGTH) << timer.duration_nano() / (MAKEMOVE_TEST_COUNT*2) << " ns. (Average of " << MAKEMOVE_TEST_COUNT*2 << " trials)" << std::endl;
}

void time_MakeMoveUndoMove() {
     Board board;  board.setup();
     Timer timer;  timer.start();

     for (int i = 0; i < MAKEUNDO_TEST_COUNT; i++) {
          board.makeMove(Move(B1, C3, NORMAL));
          board.undoMove(Move(B1, C3, NORMAL), EMPTYSQUARE);
     }

     timer.stop();
     std::cout << std::left << std::setw(FUNCTION_WS_LENGTH) << "  MAKEMOVE + UNDOMOVE" << std::right << std::setw(TIME_WS_LENGTH) << timer.duration_nano() / (MAKEUNDO_TEST_COUNT) << " ns. (Average of " << MAKEUNDO_TEST_COUNT << " trials)" << std::endl;
}

void time_CheckGameState() {
     Board board;  board.setup();
     Board savedBoard[MAX_MOVENUMBER];
     int saveIndex = 0;
     MoveList moveList = moveGeneration(board);
     Timer timer;  timer.start();

     for (int i = 0; i < CHECKGAMESTATE_TEST_COUNT; i++) {
          checkGameState(board, moveList, savedBoard, saveIndex);
     }

     timer.stop();
     std::cout << std::left << std::setw(FUNCTION_WS_LENGTH) << "  CHECKGAMESTATE INIT" << std::right << std::setw(TIME_WS_LENGTH) << timer.duration_nano() / (CHECKGAMESTATE_TEST_COUNT) << " ns. (Average of " << CHECKGAMESTATE_TEST_COUNT << " trials)" << std::endl;
}

void time_Alphabeta(const int depth) {
     Board board;  board.setup();
     Variation pv;
     Board savedBoard[MAX_MOVENUMBER];
     int saveIndex = 0;
     Timer timer;  timer.start();

     for (int i = 0; i < ALPHABETA_TEST_COUNT[depth]; i++) {
          rootAlphabeta(depth, board, &pv, savedBoard, saveIndex);
     }

     timer.stop();
     std::cout << std::left << std::setw(FUNCTION_WS_LENGTH) << "  ALPHABETA " + std::to_string(depth) << std::right << std::setw(TIME_WS_LENGTH) << timer.duration_nano() / (ALPHABETA_TEST_COUNT[depth] * 2) << " ns. (Average of " << ALPHABETA_TEST_COUNT[depth] << " trials)" << std::endl;
}

int main() {
     Timer programTimer;
     programTimer.start();
     
     std::cout << "********************************************************************************" << std::endl;
     std::cout << "*                                                                              *" << std::endl;
     std::cout << "*                            DOUMI CHESS UNIT TESTS                            *" << std::endl;
     std::cout << "*                                                                              *" << std::endl;
     std::cout << "********************************************************************************" << std::endl << std::endl;

     test_BoardClass();
     test_Perft();
     test_GameState();

     // FIXME: Add MoveGen Test: check equality of elements in array
     // FIXME: Add MakeMove Test
     // FIXME: Add UndoMove Test
     // FIXME: Add Alphabeta Test (check unmodified)
     // FIXME: Add Mate in X "Test"

     /*
     printf("\n");
     printf("--------------------------------------------------------------------------------\n");
     printf(" MakeMove / Evaluation Test                                                     \n");
     printf("--------------------------------------------------------------------------------\n");
     printf("\n");


     Board copiedInitBoard;

     for (int i = 0; i < moveList.getCounter(); i++) {
          copiedInitBoard = initialBoard;
          printf("%2d  ", i + 1);
          std::cout << numberToFilerank(moveList.getMove(i).getInitial())
               << numberToFilerank(moveList.getMove(i).getTerminal());
          int capturedPiece = copiedInitBoard.makeMove(moveList.getMove(i));
          printf("  %4d\n", copiedInitBoard.evaluate());
     }

     
     printf("\n");
     printf("--------------------------------------------------------------------------------\n");
     printf(" Depth 1 Alphabeta Test                                                         \n");
     printf("--------------------------------------------------------------------------------\n");
     printf("\n");

     Board savedBoard[MAX_MOVENUMBER];
     int saveIndex = 0;

     Variation bestMoves1;
     int bestMoveScore1 = rootAlphabeta(1, initialBoard, &bestMoves1, savedBoard, saveIndex);
     printf("Best Move Score: %3d\n", bestMoveScore1);
     std::cout << "Best Moves: ";
     printVariation(std::cout, bestMoves1);


     printf("\n");
     printf("--------------------------------------------------------------------------------\n");
     printf(" Depth 2 Alphabeta Test                                                         \n");
     printf("--------------------------------------------------------------------------------\n");
     printf("\n");

     Variation bestMoves2;
     int bestMoveScore2 = rootAlphabeta(2, initialBoard, &bestMoves2, savedBoard, saveIndex);
     printf("Best Move Score: %3d\n", bestMoveScore2);
     std::cout << "Best Moves: ";
     printVariation(std::cout, bestMoves2);


     printf("\n");
     printf("--------------------------------------------------------------------------------\n");
     printf(" Depth 3 Alphabeta Test                                                         \n");
     printf("--------------------------------------------------------------------------------\n");
     printf("\n");

     Variation bestMoves3;
     int bestMoveScore3 = rootAlphabeta(3, initialBoard, &bestMoves3, savedBoard, saveIndex);
     printf("Best Move Score: %3d\n", bestMoveScore3);
     std::cout << "Best Moves: ";
     printVariation(std::cout, bestMoves3);

     
     printf("\n");
     printf("--------------------------------------------------------------------------------\n");
     printf(" Depth 4 Alphabeta Test                                                         \n");
     printf("--------------------------------------------------------------------------------\n");
     printf("\n");

     Variation bestMoves4;
     int bestMoveScore4 = rootAlphabeta(4, initialBoard, &bestMoves4, savedBoard, saveIndex);
     printf("Best Move Score: %3d\n", bestMoveScore4);
     std::cout << "Best Moves: ";
     printVariation(std::cout, bestMoves4);

     
     
     printf("\n");
     printf("--------------------------------------------------------------------------------\n");
     printf(" Depth 5 Alphabeta Test                                                         \n");
     printf("--------------------------------------------------------------------------------\n");
     printf("\n");

     LINE bestMoves5;
     int bestMoveScore5 = rootAlphabeta(5, initialBoard, &bestMoves5, savedBoard, saveIndex);
     printf("Best Move Score: %3d\n", bestMoveScore5);
     std::cout << "Best Moves: ";
     for (int i = 0; i < 4; i++) {
          std::cout << numberToFilerank(bestMoves5.argmove[i].getInitial())
               << numberToFilerank(bestMoves5.argmove[i].getTerminal())
               << " ";
     }
     printf("\n");

     
     printf("\n");
     printf("--------------------------------------------------------------------------------\n");
     printf(" First 4 Moves with Depth 4 Test                                                \n");
     printf("--------------------------------------------------------------------------------\n");
     printf("\n");

     for (int i = 0; i < 4; i++) {
          LINE bestMoves;
          int bestMoveScore = rootAlphabeta(4, initialBoard, &bestMoves, savedBoard, saveIndex);
          printf("Best Move Score: %3d\n", bestMoveScore);
          std::cout << "Best Moves: ";
          for (int j = 0; j < 4; j++) {
               std::cout << numberToFilerank(bestMoves.argmove[j].getInitial())
                    << numberToFilerank(bestMoves.argmove[j].getTerminal())
                    << " ";
          }
          printf("\n");
          initialBoard.makeMove(bestMoves[4]);
     }

     printf("\n");
     printf("--------------------------------------------------------------------------------\n");
     printf(" Mate in 1 Test                                                                 \n");
     printf("--------------------------------------------------------------------------------\n");
     printf("\n");

     Board matein1Board;
     Board savedBoard5[MAX_MOVENUMBER];
     int saveIndex5 = 0;
     matein1Board.import("7k/RR6/8/8/8/8/8/7K w - - 0 1");

     Variation matein1Move;
     int matein1Score = rootAlphabeta(4, matein1Board, &matein1Move, savedBoard5, saveIndex5);
     printf("Best Move Score: %3d\n", matein1Score);
     std::cout << "Best Moves: ";
     printVariation(std::cout, matein1Move);

     

     printf("\n");
     printf("--------------------------------------------------------------------------------\n");
     printf(" Mate in 2 Test                                                                 \n");
     printf("--------------------------------------------------------------------------------\n");
     printf("\n");

     Board matein2Board;
     matein2Board.import("7k/8/RR6/8/8/8/8/7K w - - 0 1");

     Variation matein2Move;
     int matein2Score = rootAlphabeta(4, matein2Board, &matein2Move, savedBoard5, saveIndex5);
     printf("Best Move Score: %3d\n", matein2Score);
     std::cout << "Best Moves: ";
     printVariation(std::cout, matein2Move);

     

     printf("\n");
     printf("--------------------------------------------------------------------------------\n");
     printf(" Forced Stalemate in Unfavorable Position Test                                  \n");
     printf("--------------------------------------------------------------------------------\n");
     printf("\n");

     Board forcedStalemateBoard;
     forcedStalemateBoard.import("5k2/r7/4PKP1/4PPP1/8/8/8/N3NN2 b - - 0 1");
     //forcedStalemateBoard.import("5k2/r7/4PKP1/4PPP1/8/8/8/4QQQ1 b - - 0 1");
     printf("Board Evaluation: %d\n", forcedStalemateBoard.evaluate());

     forcedStalemateBoard.print();

     Variation forcedStalemateMoves;
     int forcedStalemateScore = rootAlphabeta(4, forcedStalemateBoard, &forcedStalemateMoves, savedBoard5, saveIndex5);
     printf("Best Move Score: %3d\n", forcedStalemateScore);
     std::cout << "Best Moves: ";
     printVariation(std::cout, forcedStalemateMoves);

     */

     std::cout << std::endl;
     std::cout << "********************************************************************************" << std::endl;
     std::cout << "*                                                                              *" << std::endl;
     std::cout << "*                              UNIT TESTS SUMMARY                              *" << std::endl;
     std::cout << "*                                                                              *" << std::endl;
     std::cout << "********************************************************************************" << std::endl << std::endl;
     
     programTimer.stop();
     std::cout << "  Test Duration: " << programTimer.duration_milli() << " ms." << std::endl;
     std::cout << "  Passed tests : " << passCount << std::endl;
     std::cout << "  Failed tests : " << failCount << std::endl;

     std::cout << std::endl;
     std::cout << "********************************************************************************" << std::endl;
     std::cout << "*                                                                              *" << std::endl;
     std::cout << "*                            DOUMI CHESS BOTTLENECK                            *" << std::endl;
     std::cout << "*                                                                              *" << std::endl;
     std::cout << "********************************************************************************" << std::endl << std::endl;

     time_MoveGen();
     time_Evaluate();
     time_CopyBoard();
     time_MakeMove();
     time_MakeMoveUndoMove();
     time_CheckGameState();
     
     std::cout << std::endl;

     time_Alphabeta(1);
     time_Alphabeta(2);
     time_Alphabeta(3);
     time_Alphabeta(4);
     time_Alphabeta(5);

     std::cout << std::endl;

     return 0;
}