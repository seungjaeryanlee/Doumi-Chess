#include "defs.h"
#include "protos.h"
#include <iostream>
#include <string>

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