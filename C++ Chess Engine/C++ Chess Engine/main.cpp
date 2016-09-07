#include <stdio.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include <climits>
#include <array>
#include "protos.h"
#include "defs.h"
#include "movegen.h"
#include "timer.h"

/******************************************************************************/
/*                                 GLOBAL VARIABLE                            */
/******************************************************************************/
bool endGame = false;
//  Current Half Move Number, starts at 0
int halfMoveCount = 0;
Move currentBoardMoveList[MAX_MOVEGEN_COUNT];
int currentBoardMoveCount;
Move currentBoardLegalMoveList[MAX_MOVEGEN_COUNT];
int currentBoardLegalMoveCount;
Move depthAllMoveList[MAXIMUM_DEPTH + 1][MAX_MOVEGEN_COUNT];
int depthAllMoveCount[MAXIMUM_DEPTH + 1];
Move depthLegalMoveList[MAXIMUM_DEPTH + 1][MAX_MOVEGEN_COUNT];
int depthLegalMoveCount[MAXIMUM_DEPTH + 1];
//  Stores Board and Board States for threefold repetition
Board savedBoard[MAX_MOVENUMBER + 1];
//  Saved values for UNDO_MOVE command
int savedTerminalValue[MAX_MOVENUMBER]; // TODO: Check if it should be initialized as ERROR_INTEGER
Move savedMove[MAX_MOVENUMBER + 1];

/******************************************************************************/
/*                                  FUNCTIONS                                 */
/******************************************************************************/

/*                             BOARD SETUP FUNCTIONS                          */
void board120Setup(Board& board) {
     board.setTurn(WHITE);
     board.setEnpassantSquare(0);
     board.setMoveNumber(1);
     board.setFiftyMoveCount(0);

     board.setCastlingArray({ true, true, true, true });
     
     //  Add Empty Squares
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               board.setSquare((i + 2) * 10 + j + 1, EMPTYSQUARE);
          }
     }
     
     //  Add Error Squares
     for (int j = 0; j < 10; j++) {
          board.setSquare(j, ERRORSQUARE);
          board.setSquare(10 + j, ERRORSQUARE);
          board.setSquare(100 + j, ERRORSQUARE);
          board.setSquare(110 + j, ERRORSQUARE);
     }
     for (int j = 0; j < 12; j++) {
          board.setSquare(10*j, ERRORSQUARE);
          board.setSquare(10*j + 9, ERRORSQUARE);
     }
     
     //  Add Non-Pawn Pieces
     board.setSquare(A8, BLACKROOK);
     board.setSquare(B8, BLACKKNIGHT);
     board.setSquare(C8, BLACKBISHOP);
     board.setSquare(D8, BLACKQUEEN);
     board.setSquare(E8, BLACKKING);
     board.setSquare(F8, BLACKBISHOP);
     board.setSquare(G8, BLACKKNIGHT);
     board.setSquare(H8, BLACKROOK);

     board.setSquare(A1, WHITEROOK);
     board.setSquare(B1, WHITEKNIGHT);
     board.setSquare(C1, WHITEBISHOP);
     board.setSquare(D1, WHITEQUEEN);
     board.setSquare(E1, WHITEKING);
     board.setSquare(F1, WHITEBISHOP);
     board.setSquare(G1, WHITEKNIGHT);
     board.setSquare(H1, WHITEROOK);

     //  Add Pawn Pieces
     for (int i = 0; i < 8; i++) {
          board.setSquare(A2 + i, WHITEPAWN);
          board.setSquare(A7 + i, BLACKPAWN);
     }
}
void FENboardSetup(Board& board, const std::string FEN) {
     board.setCastlingArray({ false, false, false, false });
     board.setEnpassantSquare(0);

     //  Add Error Squares
     for (int j = 0; j < 10; j++) {
          board.setSquare(j, ERRORSQUARE);
          board.setSquare(10 + j, ERRORSQUARE);
          board.setSquare(100 + j, ERRORSQUARE);
          board.setSquare(110 + j, ERRORSQUARE);
     }
     for (int j = 0; j < 12; j++) {
          board.setSquare(10 * j, ERRORSQUARE);
          board.setSquare(10 * j + 9, ERRORSQUARE);
     }

     int currentSquare = 21;
     unsigned int i;
     for (i = 0; i < FEN.length(); i++) {
          if (FEN.at(i) == ' ') { break; }
          if (FEN.at(i) == '/') {
               //  next row, first square
               currentSquare = currentSquare / ROW*ROW + ROW + 1;
               continue;
          }
          else if ('1' <= FEN.at(i) && FEN.at(i) <= '8') {
               for (int k = 0; k < FEN.at(i) - '0'; k++) {
                    board.setSquare(currentSquare, EMPTYSQUARE);
                    currentSquare++;
               }
          }
          else {
               switch (FEN.at(i)) {
               case 'p':
                    board.setSquare(currentSquare, BLACKPAWN);
                    break;
               case 'r':
                    board.setSquare(currentSquare, BLACKROOK);
                    break;
               case 'n':
                    board.setSquare(currentSquare, BLACKKNIGHT);
                    break;
               case 'b':
                    board.setSquare(currentSquare, BLACKBISHOP);
                    break;
               case 'q':
                    board.setSquare(currentSquare, BLACKQUEEN);
                    break;
               case 'k':
                    board.setSquare(currentSquare, BLACKKING);
                    break;
               case 'P':
                    board.setSquare(currentSquare, WHITEPAWN);
                    break;
               case 'R':
                    board.setSquare(currentSquare, WHITEROOK);
                    break;
               case 'N':
                    board.setSquare(currentSquare, WHITEKNIGHT);
                    break;
               case 'B':
                    board.setSquare(currentSquare, WHITEBISHOP);
                    break;
               case 'Q':
                    board.setSquare(currentSquare, WHITEQUEEN);
                    break;
               case 'K':
                    board.setSquare(currentSquare, WHITEKING);
                    break;
               }
               currentSquare++;
          }
     }
     i++;
     if (FEN.at(i) == 'w') { board.setTurn(WHITE); }
     else { board.setTurn(WHITE); }

     i += 2;
     if (FEN.at(i) != '-') {
          while (FEN.at(i) != ' ') {
               if (FEN.at(i) == 'K') {
                    board.setCastling(WKCASTLING, true);
               }
               if (FEN.at(i) == 'Q') {
                    board.setCastling(WQCASTLING, true);
               }
               if (FEN.at(i) == 'k') {
                    board.setCastling(BKCASTLING, true);
               }
               if (FEN.at(i) == 'q') {
                    board.setCastling(BQCASTLING, true);
               }
               i++;
          }
          i++;
     }
     else { i += 2; }

     if (FEN.at(i) != '-') {
          //  get enpassant square
          int enpassantSquare;
          enpassantSquare = COLUMN*(FEN.at(i) - 'a' + 1);
          i++;
          enpassantSquare += ROW*('9' - FEN.at(i) + 1);

          board.setEnpassantSquare(enpassantSquare);
     }

     i += 2;
     // One-digit Fifty Move Count
     if (FEN.at(i + 1) == ' ') {
          board.setFiftyMoveCount(FEN.at(i) - '0');
          i += 2;
     }
     // Two-digit Fifty Move Count
     else if ('0' <= FEN.at(i+1) && FEN.at(i+1) <= '9') {
          board.setFiftyMoveCount(10 * (FEN.at(i) - '0') + (FEN.at(i + 1) - '0'));
          i += 3;
     }
     
     // One-digit Move Number
     if (FEN.length() == i+1 || FEN.at(i + 1) == ' ') {
          board.setMoveNumber(FEN.at(i) - '0');
     }
     // Two-digit Move Number
     else if ('0' <= FEN.at(i + 1) && FEN.at(i + 1) <= '9') {
          board.setMoveNumber(10 * (FEN.at(i) - '0') + (FEN.at(i + 1) - '0'));
     }


}
std::string boardToFEN(const Board& board) {
     std::string FEN;
     int emptySquareCount = 0;

     for (int i = 2; i < 10; i++) {
          for (int j = 1; j < 9; j++) {
               if (board.getSquare(i*ROW + j*COLUMN) == EMPTYSQUARE) {
                    emptySquareCount++;
                    continue;
               }
               if (board.getSquare(i*ROW + j*COLUMN) != EMPTYSQUARE &&
                    emptySquareCount != 0) {
                    FEN += ('0' + emptySquareCount);
                    emptySquareCount = 0;
               }
               switch (board.getSquare(i*ROW + j*COLUMN)) {
               case WHITEPAWN:
                    FEN += 'P';
                    break;
               case WHITEKNIGHT:
                    FEN += 'N';
                    break;
               case WHITEBISHOP:
                    FEN += 'B';
                    break;
               case WHITEROOK:
                    FEN += 'R';
                    break;
               case WHITEQUEEN:
                    FEN += 'Q';
                    break;
               case WHITEKING:
                    FEN += 'K';
                    break;
               case BLACKPAWN:
                    FEN += 'p';
                    break;
               case BLACKKNIGHT:
                    FEN += 'n';
                    break;
               case BLACKBISHOP:
                    FEN += 'b';
                    break;
               case BLACKROOK:
                    FEN += 'r';
                    break;
               case BLACKQUEEN:
                    FEN += 'q';
                    break;
               case BLACKKING:
                    FEN += 'k';
                    break;

               }
          }
          if (emptySquareCount != 0) {
               FEN += ('0' + emptySquareCount);
               emptySquareCount = 0;
          }

          if (i != 9) {
               FEN += '/';
          }

     }

     FEN += ' ';
     if (board.getTurn() == WHITE) { FEN += 'w'; }
     else { FEN += 'b'; }

     FEN += ' ';
     //  no castling available
     bool WKCastling = board.getCastling(WKCASTLING);
     bool WQCastling = board.getCastling(WQCASTLING);
     bool BKCastling = board.getCastling(BKCASTLING);
     bool BQCastling = board.getCastling(BQCASTLING);
     if (!(WKCastling || WQCastling || BKCastling || BQCastling)) {
          FEN += '-';
     }
     else {
          if (WKCastling) {
               FEN += 'K';
          }
          if (WQCastling) {
               FEN += 'Q';
          }
          if (BKCastling) {
               FEN += 'k';
          }
          if (BQCastling) {
               FEN += 'q';
          }
     }

     FEN += ' ';
     if (board.getEnpassantSquare() != 0) {
          FEN += numberToFile(board.getEnpassantSquare());
          FEN += ('0' + numberToRank(board.getEnpassantSquare()));
     }
     else { FEN += '-'; }

     FEN += ' ';
     FEN += std::to_string(board.getFiftyMoveCount());
     FEN += ' ';
     FEN += std::to_string(board.getMoveNumber());
     
     std::cout << FEN << std::endl;
     return FEN;

}
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
               switch (board.getSquare(i*ROW+j*COLUMN)) {
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
int checkColor(const int pieceType) {
     if (WHITEPAWN <= pieceType && pieceType <= WHITEKING) {
          return WHITE;
     }
     else if (BLACKPAWN <= pieceType && pieceType <= BLACKKING) {
          return BLACK;
     }
     else if (pieceType == EMPTYSQUARE || pieceType == ERRORSQUARE) {
          return NEITHER;
     }
     else {
          printf("checkColor unreachable error\n");
          return 0;
     }
}
char numberToFile(const int position) {
     char file = 'a' + position % 10 - 1;
     return file;
}
int numberToRank(const int position) {
     int rank = 10 - position / 10;
     return rank;
}
int filerankToNumber(const char file, const int rank) {
     //  if it is not a correct filerank format, return error
     if ('a' > file || file > 'h' || '0' > rank || rank > '8' ) {
          return ERROR_INTEGER;
     }

     int position120;
     position120 = COLUMN*(file - 'a' + 1) + ROW*(9 - (rank - '1'));
     return position120;
}
std::string numberToFilerank(const int position) {
     std::string fileRank = "";
     char file = 'a' + position % 10 - 1; 
     int rank = 10 - position / 10;
     fileRank += file;
     fileRank += std::to_string(rank);
     return fileRank;
}
void printMove(const Move& move) {
     std::cout << numberToFilerank(move.getInitial()) << " " << numberToFilerank(move.getTerminal()) << " (" << move.getType() << ")" << std::endl;
}

/*                             EVALUATION FUNCTIONS                           */
int boardEvaluation(const Board& board) {
     int score = 0;
     for (int i = 0; i < 120; i++) {
          switch (board.getSquare(i)) {
          case WHITEPAWN:
               score += PAWNVALUE;
               score += PAWN_PCSQTable.at(i);
               break;
          case WHITEKNIGHT:
               score += KNIGHTVALUE;
               score += KNIGHT_PCSQTable.at(i);
               break;
          case WHITEBISHOP:
               score += BISHOPVALUE;
               score += BISHOP_PCSQTable.at(i);
               break;
          case WHITEROOK:
               score += ROOKVALUE;
               score += ROOK_PCSQTable.at(i);
               break;
          case WHITEQUEEN:
               score += QUEENVALUE;
               score += QUEEN_PCSQTable.at(i);
               break;
          case WHITEKING:
               score += KINGVALUE;
               if (endGame) {
                    score += KING_PCSQTable_ENDGAME.at(i);
               }
               else {
                    score += KING_PCSQTable.at(i);
               }
               break;
          case BLACKPAWN:
               score -= PAWNVALUE;
               score -= PAWN_PCSQTable.at(reversePosition(i));
               break;
          case BLACKKNIGHT:
               score -= KNIGHTVALUE;
               score -= KNIGHT_PCSQTable.at(reversePosition(i));
               break;
          case BLACKBISHOP:
               score -= BISHOPVALUE;
               score -= BISHOP_PCSQTable.at(reversePosition(i));
               break;
          case BLACKROOK:
               score -= ROOKVALUE;
               score -= ROOK_PCSQTable.at(reversePosition(i));
               break;
          case BLACKQUEEN:
               score -= QUEENVALUE;
               score -= QUEEN_PCSQTable.at(reversePosition(i));
               break;
          case BLACKKING:
               score -= KINGVALUE;
               if (endGame) {
                    score -= KING_PCSQTable_ENDGAME.at(reversePosition(i));
               }
               else {
                    score -= KING_PCSQTable.at(reversePosition(i));
               }
               break;
          }
     }
     return score;
}
int reversePosition(const int position) {
     int row = position / 10, column = position % 10;
     return (12 - row - 1) * 10 + column;
}

int negaMax(const int depth, Board& board) {
     if (depth == 0) {
          return board.getTurn() * boardEvaluation(board);
     }
     int max_Score = INT_MIN;
     int score;
     int terminalValue;

     moveGeneration(board, depthAllMoveList[depth], &depthAllMoveCount[depth]);
     legalMoves(board, depthAllMoveList[depth], depthAllMoveCount[depth], depthLegalMoveList[depth], &depthLegalMoveCount[depth]);

     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {

          castlingUpdate(board, depthLegalMoveList[depth][i]);
          int enpassantSquare = board.getEnpassantSquare();

          terminalValue = makeMove(board, depthLegalMoveList[depth][i]);

          score = -negaMax(depth-1, board);

          if (score > max_Score) {
               max_Score = score;
          }

          undoMove(board, depthLegalMoveList[depth][i], terminalValue);
          board.setEnpassantSquare(enpassantSquare);
     }

     return max_Score;
}
int rootNegaMax(const int maxDepth, Board& board, Move& bestMove) {

     int max_Score = INT_MIN;
     int score;
     int terminalValue;

     moveGeneration(board, depthAllMoveList[maxDepth], &depthAllMoveCount[maxDepth]);
     legalMoves(board, depthAllMoveList[maxDepth], depthAllMoveCount[maxDepth], depthLegalMoveList[maxDepth], &depthLegalMoveCount[maxDepth]);

     for (int i = 0; i < depthLegalMoveCount[maxDepth]; i++) {
          castlingUpdate(board, depthLegalMoveList[maxDepth][i]);

          int enpassantSquare = board.getEnpassantSquare();
          terminalValue = makeMove(board, depthLegalMoveList[maxDepth][i]);

          score = -negaMax(maxDepth - 1, board);

          if (score > max_Score) {
               max_Score = score;
               bestMove = Move(depthLegalMoveList[maxDepth][i]);
          }

          undoMove(board, depthLegalMoveList[maxDepth][i], terminalValue);
          board.setEnpassantSquare(enpassantSquare);
     }

     return max_Score;
}

int alphabeta(const int depth, Board& board, int alpha, int beta) {
     if (depth == 0) {
          return board.getTurn() * boardEvaluation(board);
     }
     int score;
     int terminalValue;

     moveGeneration(board, depthAllMoveList[depth], &depthAllMoveCount[depth]);
     legalMoves(board, depthAllMoveList[depth], depthAllMoveCount[depth], depthLegalMoveList[depth], &depthLegalMoveCount[depth]);

     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {

          castlingUpdate(board, depthLegalMoveList[depth][i]);

          // Save enpassantSquare so it doesn't get lost while making move
          int enpassantSquare = board.getEnpassantSquare();
         
          terminalValue = makeMove(board, depthLegalMoveList[depth][i]);

          score = -alphabeta(depth - 1, board, -beta, -alpha);

          if (score >= beta) {
               undoMove(board, depthLegalMoveList[depth][i], terminalValue);
               board.setEnpassantSquare(enpassantSquare);
               return beta;
          }
          
          if (score > alpha) {
               alpha = score;
          }
          undoMove(board, depthLegalMoveList[depth][i], terminalValue);
          board.setEnpassantSquare(enpassantSquare);
     }

     return alpha;
}
int rootAlphabeta(const int maxDepth, Board board, int alpha, int beta, Move& bestMove) {
     int score;
     int terminalValue;

     moveGeneration(board, depthAllMoveList[maxDepth], &depthAllMoveCount[maxDepth]);
     legalMoves(board, depthAllMoveList[maxDepth], depthAllMoveCount[maxDepth], depthLegalMoveList[maxDepth], &depthLegalMoveCount[maxDepth]);

     for (int i = 0; i < depthLegalMoveCount[maxDepth]; i++) {

          castlingUpdate(board, depthLegalMoveList[maxDepth][i]);
          int enpassantSquare = board.getEnpassantSquare();
          terminalValue = makeMove(board, depthLegalMoveList[maxDepth][i]);

          score = -alphabeta(maxDepth - 1, board, -beta, -alpha);

          // TODO: Check if this is needed and change it
          if (score >= beta) {

               undoMove(board, depthLegalMoveList[maxDepth][i], terminalValue);
               // TODO: Make sure Castling & EP Square & other details are also undo-ed
               board.setEnpassantSquare(enpassantSquare);
               return beta;
          }

          if (score > alpha) {
               alpha = score;
               bestMove = Move(depthLegalMoveList[maxDepth][i]);
          }

          undoMove(board, depthLegalMoveList[maxDepth][i], terminalValue);
          board.setEnpassantSquare(enpassantSquare);
     }

     return alpha;
}

/*                             GAME CYCLE FUNCTIONS                           */
bool checkGameEnd(const Board& board) {
     bool whiteKing = false, blackKing = false;
     for (int i = 0; i < 120; i++) {
          if (board.getSquare(i) == WHITEKING) { whiteKing = true; }
          if (board.getSquare(i) == BLACKKING) { blackKing = true; }
     }
     return !(whiteKing && blackKing);
}
bool checkEndgame(const Board& board) {
     if (endGame) { return true; }
     else {
          int queenCount = 0;
          for (int i = 0; i < 120; i++) {
               if (board.getSquare(i) == WHITEQUEEN || board.getSquare(i) == BLACKQUEEN) {
                    queenCount++;
               }
          }
          if (queenCount == 0) { return true; }
          else { return false; }
     }
}


/*                             RECURSION FUNCTIONS                             */
u64 divide(int depth, int maxDepth, Board& board, bool showOutput) {

     if (depth == 0) { return 1; }

     depthAllMoveCount[depth] = 0;
     depthLegalMoveCount[depth] = 0;

     u64 node = 0, individualNode = 0;
     int terminalValue;

     // MOVEGEN
     moveGeneration(board, depthAllMoveList[depth], &depthAllMoveCount[depth]);
     // CHECK FOR LEGALS
     legalMoves(board, depthAllMoveList[depth], depthAllMoveCount[depth], depthLegalMoveList[depth], &depthLegalMoveCount[depth]);

     if (depth == 1) { return depthLegalMoveCount[depth]; }

     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {

          int initial = depthLegalMoveList[depth][i].getInitial();
          int terminal = depthLegalMoveList[depth][i].getTerminal();

          castlingUpdate(board, depthLegalMoveList[maxDepth][i]);

          int enpassantSquare = board.getEnpassantSquare();
          
          terminalValue = makeMove(board, depthLegalMoveList[depth][i]);
          
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

          undoMove(board, depthLegalMoveList[depth][i], terminalValue);
          board.setEnpassantSquare(enpassantSquare);
     }
     return node;

}
u64 divide2(int depth, int maxDepth, Board& board, bool showOutput) {

     if (depth == 0) { return 1; }

     //  output text file for large output
     std::ofstream output2;
     output2.open("divide.txt");

     depthAllMoveCount[depth] = 0;
     depthLegalMoveCount[depth] = 0;

     u64 node = 0, individualNode = 0;
     int terminalValue;

     // MOVEGEN
     moveGeneration(board, depthAllMoveList[depth], &depthAllMoveCount[depth]);
     // CHECK FOR LEGALS
     legalMoves(board, depthAllMoveList[depth], depthAllMoveCount[depth], depthLegalMoveList[depth], &depthLegalMoveCount[depth]);

     //if (depth == 1) { return depthLegalMoveCount[depth]; }

     for (int i = 0; i < depthLegalMoveCount[depth]; i++) {
          int initial = depthLegalMoveList[depth][i].getInitial();
          int terminal = depthLegalMoveList[depth][i].getTerminal();

          castlingUpdate(board, depthLegalMoveList[maxDepth][i]);
          int enpassantSquare = board.getEnpassantSquare();
          terminalValue = makeMove(board, depthLegalMoveList[depth][i]);


          node += divide(depth - 1, maxDepth, board, showOutput);
          if (showOutput) {
               individualNode = divide(depth - 1, maxDepth, board, false);
          }

          if (depth >= maxDepth && showOutput) {
               output2 << numberToFile(initial) << numberToRank(initial) <<
                    numberToFile(terminal) << numberToRank(terminal) << ": " << individualNode << std::endl;
          }

          undoMove(board, depthLegalMoveList[depth][i], terminalValue);
          board.setEnpassantSquare(enpassantSquare);
     }
     return node;
     output2.close();
}

int makeMove(Board &board, Move& move) {
     int terminalValue;
     int initial = move.getInitial(), terminal = move.getTerminal(), moveType = move.getType();

     board.setEnpassantSquare(0);
     board.changeTurn();

     if (moveType == NORMAL) {
          terminalValue = board.getSquare(terminal);
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          return terminalValue;
     }
     if (moveType == DOUBLEMOVE) {
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          board.setEnpassantSquare((terminal + initial) / 2);
          //  terminalValue is actually enpassantSquare
          return (terminal + initial) / 2;
     }
     else if (moveType == QUEENSIDE_CASTLING) {
          //  move king
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          //  move rook
          board.setSquare(terminal + COLUMN, board.getSquare(initial - 4 * COLUMN));
          board.setSquare(initial - 4 * COLUMN, EMPTYSQUARE);
          //  castling does not involve capture
          return 0;
     }
     else if (moveType == KINGSIDE_CASTLING) {
          //  move king
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          //  move rook
          board.setSquare(terminal - COLUMN, board.getSquare(terminal + COLUMN));
          board.setSquare(terminal + COLUMN, EMPTYSQUARE);
          //  castling does not involve capture
          return 0;
     }
     else if (moveType == KNIGHT_PROMOTION) {
          terminalValue = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEKNIGHT);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKKNIGHT);
          }
          board.setSquare(initial, EMPTYSQUARE);
          return terminalValue;
     }
     else if (moveType == BISHOP_PROMOTION) {
          terminalValue = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEBISHOP);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKBISHOP);
          }
          board.setSquare(initial, EMPTYSQUARE);
          return terminalValue;
     }
     else if (moveType == ROOK_PROMOTION) {
          terminalValue = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEROOK);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKROOK);
          }
          board.setSquare(initial, EMPTYSQUARE);
          return terminalValue;
     }
     else if (moveType == QUEEN_PROMOTION) {
          terminalValue = board.getSquare(terminal);

          //  white turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, WHITEQUEEN);
          }
          //  black turn
          else {
               board.setSquare(terminal, BLACKQUEEN);
          }
          board.setSquare(initial, EMPTYSQUARE);
          return terminalValue;
     }
     else if (moveType == ENPASSANT) {
          //  White turn
          if (board.getSquare(initial) == WHITEPAWN) {
               board.setSquare(terminal, board.getSquare(initial));
               board.setSquare(initial, EMPTYSQUARE);
               board.setSquare(terminal + ROW, EMPTYSQUARE);
               return BLACKPAWN;
          }
          //  Black turn
          else {
               board.setSquare(terminal, board.getSquare(initial));
               board.setSquare(initial, EMPTYSQUARE);
               board.setSquare(terminal - ROW, EMPTYSQUARE);
               return WHITEPAWN;
          }
     }
     else {
          printf("makeMove unreachable error\n");
          return 0;
     }
}
void undoMove(Board &board, Move& move, int terminalValue) {
     int initial = move.getInitial(), terminal = move.getTerminal(), moveType = move.getType();

     board.changeTurn();
     if (moveType == NORMAL) {
          board.setSquare(initial, board.getSquare(terminal));
          board.setSquare(terminal, terminalValue);
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
               board.setSquare(terminal, terminalValue);
               board.setSquare(initial, WHITEPAWN);
          }
          //  black turn
          else {
               board.setSquare(terminal, terminalValue);
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
void castlingUpdate(Board& board, const Move& move) {
     if (board.getSquare(move.getInitial()) == WHITEKING) {
          board.setCastling(WKCASTLING, false);
          board.setCastling(WQCASTLING, false);
     }
     if (board.getSquare(move.getInitial()) == BLACKKING) {
          board.setCastling(BKCASTLING, false);
          board.setCastling(BQCASTLING, false);
     }
     if (board.getSquare(move.getInitial()) == WHITEROOK) {
          if (move.getInitial() == A1) {
               board.setCastling(WQCASTLING, false);
          }
          if (move.getInitial() == H1) {
               board.setCastling(WKCASTLING, false);
          }
     }
     if (board.getSquare(move.getInitial()) == BLACKROOK) {
          if (move.getInitial() == A8) {
               board.setCastling(BQCASTLING, false);
          }
          if (move.getInitial() == H8) {
               board.setCastling(BKCASTLING, false);
          }
     }
}
int isTerminalNode(Board& board) {
     Move tempBoardMoveList[MAX_MOVEGEN_COUNT];
     int tempBoardMoveCount;
     Move tempBoardLegalMoveList[MAX_MOVEGEN_COUNT];
     int tempBoardLegalMoveCount;
     
     moveGeneration(board, tempBoardMoveList, &tempBoardMoveCount);
     legalMoves(board, tempBoardMoveList, tempBoardMoveCount, tempBoardLegalMoveList, &tempBoardLegalMoveCount);
     
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

     // Checkmate                                                                                                                 
     if (tempBoardLegalMoveCount == 0 && squareAttackCheck(board, kingPos)) {
          return CHECKMATE;
     }

     // Stalemate: No legal move
     if (tempBoardLegalMoveCount == 0) {
          return STALEMATE_MOVE;
     }
     
     // Stalemate: 75 Move Rule
     // TODO: 50 Move rule will be implemented in moveGen
     if (board.getFiftyMoveCount() >= 150) {
          return STALEMATE_75;
     }

     // Stalemate: Threefold Repetition
     // TODO: Implement

     return NOTMATE;
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

bool fiftyMoveCheck(Board& board, Move& move) {
     int initial = move.getInitial();
     int terminal = move.getTerminal();

     if (board.getSquare(terminal) == EMPTYSQUARE
          && board.getSquare(initial) != WHITEPAWN
          && board.getSquare(initial) != BLACKPAWN) {
          board.fiftyMoveCountIncrement();
          if (board.getFiftyMoveCount() >= 100) {
               return true;
          }
     }
     else { board.setFiftyMoveCount(0); }
     return false;
}

/******************************************************************************/
/*                               MAIN FUNCTION                                */
/******************************************************************************/
void main() {
     Board currentBoard;

     bool gamePlaying = true;
     result gameResult = NOT_FINISHED; // Records the result of the game
     int userColor = ERROR_INTEGER;    // Which color user plays
     bool spectate = false;            // if true, the game is between two computers
     LARGE_INTEGER frequency, beginTime, endTime; //  added for time performance check

     std::ofstream logtext;
     logtext.open("log.txt");
     
     board120Setup(currentBoard);
     //FENboardSetup("8/8/8/8/6k1/2KNR3/8/8 w - - 99 75");

     printSimpleBoard(currentBoard);
     printf("--------------------------------------------------\n");
     printf("Engine Search Depth: %d\n", EVAL_DEPTH);
     printf("Castling - WK:%d WQ:%d BK:%d BQ:%d\n", currentBoard.getCastling(WKCASTLING), currentBoard.getCastling(WQCASTLING), currentBoard.getCastling(BKCASTLING), currentBoard.getCastling(BQCASTLING));
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

     while (gamePlaying) {
          checkEndgame(currentBoard);

          //  Detect Checkmate/Stalemate
          switch (isTerminalNode(currentBoard)) {
          case CHECKMATE:
               if (currentBoard.getTurn() == WHITE) {
                    gameResult = BLACK_WIN;
               }
               if (currentBoard.getTurn() == BLACK) {
                    gameResult = WHITE_WIN;
               }
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
          while (!correctInput && userColor == ERROR_INTEGER) {
               printf("Which color would you like to play? (W, B or N): ");
               std::getline(std::cin, userCommand);
               if (userCommand.size() == 0) {
                    printf("You must enter W or B!\n");
                    continue;
               }
               if (userCommand.at(0) == 'W') {
                    userColor = WHITE;
                    correctInput = true;
                    logtext << "Player (White) vs. COM (Black)" << std::endl;
                    break;
               }
               else if (userCommand.at(0) == 'B') {
                    userColor = BLACK;
                    correctInput = true;
                    logtext << "COM (White) vs. Player (Black)" << std::endl;
                    break;
               }
               else if (userCommand.at(0) == 'N') {
                    spectate = true;
                    correctInput = true;
                    userColor = NEITHER;
                    logtext << "COM (White) vs. COM (Black)" << std::endl;
                    break;
               }
               else {
                    printf("Wrong input! Write W, B or N.\n");
                    continue;
               }
          }

          logtext << "COM Search Depth: " << EVAL_DEPTH << std::endl;

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
                    if ('A' <= userCommand.at(0) && userCommand.at(0) <= 'E') {
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
                    
                    savedBoard[halfMoveCount] = currentBoard;

                    //  Movelist used for legality/movetype check
                    currentBoardMoveCount = 0;
                    currentBoardLegalMoveCount = 0;
                    moveGeneration(currentBoard, currentBoardMoveList, &currentBoardMoveCount);
                    legalMoves(currentBoard, currentBoardMoveList, currentBoardMoveCount, currentBoardLegalMoveList, &currentBoardLegalMoveCount);

                    // Get user input for move
                    int moveType = NORMAL;
                    correctInput = false;
                    while (!correctInput) {
                         printf("Please enter your move: ");
                         std::getline(std::cin, userCommand); // TODO: do I want to get the entire command?

                         //  Check size
                         if (userCommand.size() < 4) {
                              printf("Wrong format: correct format is [a-h][1-8][a-h][1-8].\n");
                              continue;
                         }

                         initialSquare = filerankToNumber(userCommand.at(0), userCommand.at(1));
                         terminalSquare = filerankToNumber(userCommand.at(2), userCommand.at(3));

                         //  Check if Filerank format is correct
                         if (initialSquare == ERROR_INTEGER || terminalSquare == ERROR_INTEGER) {
                              printf("Wrong format: correct format is [char][int][char][int].\n");
                              continue;
                         }

                         //  Check legality & movetype
                         bool legal = false;
                         for (int i = 0; i < currentBoardLegalMoveCount; i++) {
                              if (initialSquare == currentBoardLegalMoveList[i].getInitial() && terminalSquare == currentBoardLegalMoveList[i].getTerminal()) {
                                   legal = true;
                                   moveType = currentBoardLegalMoveList[i].getType();
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
                         //  TODO: check if there is anything else to check :D
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
                    for (int i = 0; i < halfMoveCount; i++) {
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

                    
                    // Check Fifty Move rule
                    Move userMove = Move(initialSquare, terminalSquare, moveType);
                    if (fiftyMoveCheck(currentBoard, userMove)) {

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

                    
                    // save terminalValue for undoMove;
                    savedTerminalValue[halfMoveCount] = makeMove(currentBoard, userMove);
                    
                    savedMove[halfMoveCount] = Move(userMove);
                    

                    if (currentBoard.getTurn() == WHITE) { currentBoard.moveNumberIncrement(); }
                    
                    halfMoveCount++;

                    // add to log file
                    logtext << currentBoard.getMoveNumber() << ": " << numberToFile(initialSquare) << numberToRank(initialSquare) << " " 
                         << numberToFile(terminalSquare) << numberToRank(terminalSquare) << std::endl;

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
                         printf("What depth? (1~%d): ", MAXIMUM_DEPTH);
                         std::getline(std::cin, userCommand);
                         if (userCommand.size() == 0 || userCommand.at(0) - '0' < 1 || userCommand.at(0) - '0' > MAXIMUM_DEPTH) {
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
                         printf("What depth? (1~%d): ", MAXIMUM_DEPTH);
                         std::getline(std::cin, userCommand);
                         if (userCommand.size() == 0 || userCommand.at(0) - '0' < 1 || userCommand.at(0) - '0' > MAXIMUM_DEPTH) {
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
                    //  TerminalSquare needs to be saved
                    if (savedTerminalValue[halfMoveCount] == ERROR_INTEGER || halfMoveCount == 0) {
                         printf("No move can be undone!\n");
                         continue;
                    }
                    else {
                         halfMoveCount--;
                         currentBoard = Board(savedBoard[halfMoveCount]);
                         userColor = -userColor;
                    }
               }
               else if (commandType == COM_MAKE_MOVE) {
                    userColor = -userColor;
                    continue;
               }
               else if (commandType == PRINT_ALL_MOVES) {
                    moveGeneration(currentBoard, currentBoardMoveList, &currentBoardMoveCount);
                    legalMoves(currentBoard, currentBoardMoveList, currentBoardMoveCount, currentBoardLegalMoveList, &currentBoardLegalMoveCount);

                    printf("Movecount: %d\n", currentBoardLegalMoveCount);
                    for (int i = 0; i < currentBoardLegalMoveCount; i++) {
                         printf("%d: ", i + 1);
                         printMove(currentBoardLegalMoveList[i]);
                    }
                    continue;
               }
               else if (commandType == ALPHABETA_COMPARE) {

                    LARGE_INTEGER frequency2, beginTime2, endTime2;
                    
                    frequency2 = startTimer(&beginTime2, 2);
                    int negamaxValue = negaMax(EVAL_DEPTH, currentBoard);
                    stopTimer(&endTime2, 2);
                    printf("Negamax Value: %d\n", negamaxValue);
                    std::cout << "Negamax timer " << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms elapsed." << std::endl;

                    frequency2 = startTimer(&beginTime2, 2);
                    int alphabetaValue = alphabeta(EVAL_DEPTH, currentBoard, -999999, 999999);
                    stopTimer(&endTime2, 2);
                    printf("Alphabeta Value: %d\n", alphabetaValue);
                    std::cout << "Alphabeta timer : " << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms elapsed." << std::endl;

               }
               else if (commandType == EVALUATE_BOARD) {
                    printf("Current Board Evaluation: %d\n", boardEvaluation(currentBoard));
                    continue;
               }
               else if (commandType == ALPHABETA_SPEED_CHECK) {
                    LARGE_INTEGER frequency2, beginTime2, endTime2;

                    frequency2 = startTimer(&beginTime2, 2);
                    int alphabetaValue = alphabeta(EVAL_DEPTH, currentBoard, -999999, 999999);
                    stopTimer(&endTime2, 2);
                    printf("Alphabeta Value: %d\n", alphabetaValue);
                    std::cout << "Alphabeta timer : " << elapsedTime(beginTime2, endTime2, frequency2, 2) << " ms elapsed." << std::endl;
               }
               else if (commandType == PRINT_SAVED_FEN) {
                    for (int i = 0; i < halfMoveCount; i++) {
                         boardToFEN(savedBoard[i]); // Print statement inside boardToFEN() prints the FEN
                    }
               }
          }
          
          //  Computer turn
          else if (currentBoard.getTurn() == -userColor || spectate == true) {

               savedBoard[halfMoveCount] = currentBoard;

               Move alphabetaMove;
               int alphabetaValue = rootAlphabeta(EVAL_DEPTH, currentBoard, -999999, 999999, alphabetaMove);
               printf("Alphabeta Value: %d\n", alphabetaValue);
               printf("Alphabeta Move: ");
               printMove(alphabetaMove);

               int initial = alphabetaMove.getInitial();
               int terminal = alphabetaMove.getTerminal();
               int moveType = alphabetaMove.getType();

               //  Increment or reset Fifty move count
               //  TODO: Add 50 Move Rule option in move generation / selection
               if (currentBoard.getSquare(terminal) == EMPTYSQUARE
                    && currentBoard.getSquare(initial) != WHITEPAWN
                    && currentBoard.getSquare(initial) != BLACKPAWN) {
                    currentBoard.fiftyMoveCountIncrement();
               }
               else { currentBoard.setFiftyMoveCount(0); }

               // Check Fifty move rule
               if (fiftyMoveCheck(currentBoard, alphabetaMove)) {
                    // If in bad position, declare fifty move rule
                    printf("Computer declares Fifty Move Rule.\n");
                    logtext << "Computer declares Fifty Move Rule." << std::endl;
                    if (alphabetaValue <= STALEMATE_BOUND) {
                         gamePlaying = false;
                         gameResult = TIE;
                         break;
                    }
               }
                    
               //  Save castlingCheck for undoMove
               savedBoard[halfMoveCount].setCastlingArray(currentBoard.getCastlingArray());
               //  Update castlingCheck
               castlingUpdate(currentBoard, alphabetaMove);

               //  Update enpassant square
               if (moveType == DOUBLEMOVE) {
                    currentBoard.setEnpassantSquare((initial + terminal) / 2);
               }
               else { currentBoard.setEnpassantSquare(0); }

               //  Make best move and print board
               savedTerminalValue[halfMoveCount] = makeMove(currentBoard, alphabetaMove);
               //  Save move for undoMove
               savedMove[halfMoveCount] = Move(alphabetaMove);
               logtext << currentBoard.getMoveNumber() << ": " << numberToFilerank(initial) << " " << numberToFilerank(terminal) << std::endl;

               printSimpleBoard(currentBoard);

               //  Print out move and move number
               if (moveType == KINGSIDE_CASTLING) {
                    printf("%d: O-O\n", currentBoard.getMoveNumber());
               }
               else if (moveType == QUEENSIDE_CASTLING) {
                    printf("%d: O-O-O\n", currentBoard.getMoveNumber());
               }
               else {
                    printf("%d: ", currentBoard.getMoveNumber());
                    printMove(alphabetaMove);
               }

               //  Increment move
               if (currentBoard.getTurn() == WHITE) { currentBoard.moveNumberIncrement(); }

               //  Check if game is over
               gamePlaying = !checkGameEnd(currentBoard);
               if (!gamePlaying) { break; }

               // Check Threefold repetition
               int repetitionCount = 0;
               for (int i = 0; i < halfMoveCount; i++) {
                    if (currentBoard.isAlmostEqual(savedBoard[i])) {
                         repetitionCount++;
                    }
                    if (repetitionCount >= 3) { break; }
               }

               if (repetitionCount >= 3) {
                    correctInput = false;
                    bool declareTie = false;
                    while (!correctInput) {
                         printf("Computer: Declare Threefold Repetition? (Y/N):");
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

               // At the very end since multiple things are saved while the computer makes a move
               halfMoveCount++;
          }
     }

     //  Print Game Result
     printf("Game Result: ");
     logtext << "Game Result: ";
     switch (gameResult) {
     case BLACK_WIN:
          printf("0-1\n");
          logtext << "0-1" << std::endl;
          break;
     case TIE:
          printf("1/2-1/2\n");
          logtext << "1/2-1/2" << std::endl;
          break;
     case WHITE_WIN:
          printf("1-0\n");
          logtext << "1-0" << std::endl;
          break;
     case NOT_FINISHED:
          printf("0-0: Game not finished\n");
          logtext << "0-0: Game not finished" << std::endl;
     }

     //  Stop timer and print elapsed time
     stopTimer(&endTime, 1);
     printElapsedTime(beginTime, endTime, frequency, 1);
     logtext << "Total Time: " << elapsedTime(beginTime, endTime, frequency, 1) << "ms" << std::endl;
}