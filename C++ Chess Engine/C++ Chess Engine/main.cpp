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
#include "output_handler.h"
#include "evaluation.h"

/******************************************************************************/
/*                                  FUNCTIONS                                 */
/******************************************************************************/

/*                             BOARD SETUP FUNCTIONS                          */
void board120Setup(Board& board) {
     board.setTurn(WHITE);
     board.setEnpassantSquare(0);
     board.setMoveNumber(1);
     board.setHalfMoveClock(0);
     board.setCastlingRights({ true, true, true, true });

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
     board.setCastlingRights({ false, false, false, false });
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
     else { board.setTurn(BLACK); }

     i += 2;
     if (FEN.at(i) != '-') {
          while (FEN.at(i) != ' ') {
               if (FEN.at(i) == 'K') {
                    board.setCastlingRight(WKCASTLING, true);
               }
               if (FEN.at(i) == 'Q') {
                    board.setCastlingRight(WQCASTLING, true);
               }
               if (FEN.at(i) == 'k') {
                    board.setCastlingRight(BKCASTLING, true);
               }
               if (FEN.at(i) == 'q') {
                    board.setCastlingRight(BQCASTLING, true);
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
          board.setHalfMoveClock(FEN.at(i) - '0');
          i += 2;
     }
     // Two-digit Fifty Move Count
     else if ('0' <= FEN.at(i+1) && FEN.at(i+1) <= '9') {
          board.setHalfMoveClock(10 * (FEN.at(i) - '0') + (FEN.at(i + 1) - '0'));
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
     bool WKCastling = board.getCastlingRight(WKCASTLING);
     bool WQCastling = board.getCastlingRight(WQCASTLING);
     bool BKCastling = board.getCastlingRight(BKCASTLING);
     bool BQCastling = board.getCastlingRight(BQCASTLING);
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
     FEN += std::to_string(board.getHalfMoveClock());
     FEN += ' ';
     FEN += std::to_string(board.getMoveNumber());
     
     std::cout << FEN << std::endl;
     return FEN;

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
          printf("Invalid pieceType\n");
          return 0;
     }
}
char numberToFile(const int position) {
     return ('a' + position % ROW - 1);
}
int numberToRank(const int position) {
     return (10 - position / ROW);
}
int filerankToNumber(const char file, const int rank) {
     //  if it is not a correct filerank format, return error
     if ('a' > file || file > 'h' || 1 > rank || rank > 8 ) {
          return ERRORCODE;
     }
     return COLUMN*(file - 'a' + 1) + ROW*(9 - (rank - 1));
}
std::string numberToFilerank(const int position) {
     return numberToFile(position) + std::to_string(numberToRank(position));
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
          updateBoard(board, moveList.getMove(i), capturedPiece);
          
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
          updateBoard(board, moveList.getMove(i), capturedPiece);


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
     int capturedPiece;
     int initial = move.getInitial(), terminal = move.getTerminal(), moveType = move.getType();

     board.setEnpassantSquare(0);
     board.changeTurn();

     if (moveType == NORMAL) {
          capturedPiece = board.getSquare(terminal);
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          return capturedPiece;
     }
     if (moveType == DOUBLEMOVE) {
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          board.setEnpassantSquare((terminal + initial) / 2);
          return EMPTYSQUARE;
     }
     else if (moveType == QUEENSIDE_CASTLING) {
          //  move king
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          //  move rook
          board.setSquare(terminal + COLUMN, board.getSquare(initial - 4 * COLUMN));
          board.setSquare(initial - 4 * COLUMN, EMPTYSQUARE);
          //  castling does not involve capture
          return EMPTYSQUARE;
     }
     else if (moveType == KINGSIDE_CASTLING) {
          //  move king
          board.setSquare(terminal, board.getSquare(initial));
          board.setSquare(initial, EMPTYSQUARE);
          //  move rook
          board.setSquare(terminal - COLUMN, board.getSquare(terminal + COLUMN));
          board.setSquare(terminal + COLUMN, EMPTYSQUARE);
          //  castling does not involve capture
          return EMPTYSQUARE;
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
          return capturedPiece;
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
          return capturedPiece;
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
          return capturedPiece;
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
          return capturedPiece;
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
          printf("Invalid moveType\n");
          return 0;
     }
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
     if (moveList.getCounter() == 0 && squareAttackCheck(board, kingPos)) {
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
                    int alphabetaValue = alphabeta(6, currentBoard, DEFAULT_ALPHA, DEFAULT_BETA);
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
          }
          
          //  Computer turn
          else if (currentBoard.getTurn() == -userColor || spectate == true) {
               LARGE_INTEGER frequency, beginTime, endTime;
               frequency = startTimer(&beginTime, 2);

               savedBoard[saveIndex] = currentBoard;

               Move abMove;
               int abValue = rootAlphabeta(EVAL_DEPTH, currentBoard, DEFAULT_ALPHA, DEFAULT_BETA, abMove);
               printf("Alphabeta Value: %d\n", abValue);
               std::cout << "Alphabeta Move: " << printMove(currentBoard.getMoveNumber(), abMove);

               // Make Move, Save and Print
               savedCapturedPiece[saveIndex] = makeMove(currentBoard, abMove);
               updateBoard(currentBoard, abMove, savedCapturedPiece[saveIndex]);
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