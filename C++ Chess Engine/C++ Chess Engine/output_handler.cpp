#include "defs.h"
#include "protos.h"
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <iomanip> 

void printBoard(const Board& board) {
     for (int i = 0; i < 120; i++) {
          if (i % 10 == 0) {
               printf("\n");
          }
          switch (board.getSquare(i)) {
          case ERRORSQUARE:
               printf("X ");
               break;
          case EMPTYSQUARE:
               printf("- ");
               break;
          case WHITEPAWN:
               printf("P ");
               break;
          case WHITEKNIGHT:
               printf("N ");
               break;
          case WHITEBISHOP:
               printf("B ");
               break;
          case WHITEROOK:
               printf("R ");
               break;
          case WHITEQUEEN:
               printf("Q ");
               break;
          case WHITEKING:
               printf("K ");
               break;
          case BLACKPAWN:
               printf("p ");
               break;
          case BLACKKNIGHT:
               printf("n ");
               break;
          case BLACKBISHOP:
               printf("b ");
               break;
          case BLACKROOK:
               printf("r ");
               break;
          case BLACKQUEEN:
               printf("q ");
               break;
          case BLACKKING:
               printf("k ");
               break;
          }
     }
     printf("\n");
}
void printSimpleBoard(const Board& board) {
     for (int i = 2; i < 10; i++) {
          printf("%d| ", 10 - i);
          for (int j = 1; j < 9; j++) {
               switch (board.getSquare(i*ROW + j*COLUMN)) {
               case ERRORSQUARE:
                    printf("X ");
                    break;
               case EMPTYSQUARE:
                    printf("- ");
                    break;
               case WHITEPAWN:
                    printf("P ");
                    break;
               case WHITEKNIGHT:
                    printf("N ");
                    break;
               case WHITEBISHOP:
                    printf("B ");
                    break;
               case WHITEROOK:
                    printf("R ");
                    break;
               case WHITEQUEEN:
                    printf("Q ");
                    break;
               case WHITEKING:
                    printf("K ");
                    break;
               case BLACKPAWN:
                    printf("p ");
                    break;
               case BLACKKNIGHT:
                    printf("n ");
                    break;
               case BLACKBISHOP:
                    printf("b ");
                    break;
               case BLACKROOK:
                    printf("r ");
                    break;
               case BLACKQUEEN:
                    printf("q ");
                    break;
               case BLACKKING:
                    printf("k ");
                    break;
               }
          }
          printf("\n");
     }
     printf("  ----------------\n");
     printf("   a b c d e f g h\n");
}
std::string printMove(const int moveNumber, const Move& move) {
     std::string output = std::to_string(moveNumber) + ": " + numberToFilerank(move.getInitial()) + " "
          + numberToFilerank(move.getTerminal()) + " ";

     switch (move.getType()) {
     case NORMAL:
          output += "\n";
          break;
     case DOUBLEMOVE:
          output += "\n";
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
          output += "Promotion to Bishopn";
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

void savePGN(const result gameResult, const Move *savedMove, const int saveIndex, const bool spectate, const int userColor) {
     std::ofstream pgn;
     pgn.open("output.pgn");
     pgn << "[Event \"Friendly Match\"]\n";
     pgn << "[Site \"Princeton, NJ USA\"]\n";
     time_t now = time(0);
     struct tm timeInfo;
     localtime_s(&timeInfo, &now);
     pgn << "[Date \"" << timeInfo.tm_year + 1900 << "." << std::setfill('0')
          << std::setw(2) << timeInfo.tm_mon + 1 << "." << std::setfill('0')
          << std::setw(2) << timeInfo.tm_mday << "\"]\n";
     pgn << "[Round \"1\"]\n";
     if (spectate) {
          pgn << "[White \"Computer\"]\n";
          pgn << "[Black \"Computer\"]\n";
     }
     else if (userColor == WHITE) {
          pgn << "[White \"User\"]\n";
          pgn << "[Black \"Computer\"]\n";
     }
     else {
          pgn << "[White \"Computer\"]\n";
          pgn << "[Black \"User\"]\n";
     }
     switch (gameResult) {
     case BLACK_WIN:
          pgn << "[Result \"0-1\"]\n";
          break;
     case TIE:
          pgn << "[Result \"1/2-1/2\"]\n";
          break;
     case WHITE_WIN:
          pgn << "[Result \"1-0\"]\n";
          break;
     case NOT_FINISHED:
          pgn << "[Result \"*\"]\n";
     }
     for (int i = 0; i < saveIndex; i++) {
          if (i % 2 == 0) {
               pgn << i / 2 + 1 << ". ";
          }
          int initial = savedMove[i].getInitial();
          int terminal = savedMove[i].getTerminal();
          int type = savedMove[i].getType();

          switch (type) {
          case KINGSIDE_CASTLING:
               pgn << "O-O ";
               break;
          case QUEENSIDE_CASTLING:
               pgn << "O-O-O ";
               break;
          case KNIGHT_PROMOTION:
               pgn << numberToFilerank(terminal) << "=N";
               break;
          case BISHOP_PROMOTION:
               pgn << numberToFilerank(terminal) << "=B";
               break;
          case ROOK_PROMOTION:
               pgn << numberToFilerank(terminal) << "=R";
               break;
          case QUEEN_PROMOTION:
               pgn << numberToFilerank(terminal) << "=Q";
               break;
          case ENPASSANT:
               // TODO: Find correct way
               pgn << numberToFilerank(initial) << numberToFilerank(terminal) << " ";
               break;
          case DOUBLEMOVE:
               pgn << numberToFilerank(terminal) << " ";
               break;
          case NORMAL:
               // TODO: Find correct way
               pgn << numberToFilerank(initial) << numberToFilerank(terminal) << " ";
               break;
          }
          // TODO: Check captures
          // TODO: Check checks (CHECKCEPTION)
          // TODO: Check checkmates
     }

     // Print Game Result
     switch (gameResult) {
     case BLACK_WIN:
          pgn << "0-1\n";
          break;
     case TIE:
          pgn << "1/2-1/2\n";
          break;
     case WHITE_WIN:
          pgn << "1-0\n";
          break;
     case NOT_FINISHED:
          pgn << "*\n";
     }

     pgn.close();

}