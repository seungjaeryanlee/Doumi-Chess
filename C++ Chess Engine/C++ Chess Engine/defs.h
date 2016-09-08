#pragma once
#include <array>

typedef unsigned long long u64;

/*                                    DEFINITION                              */
const int PAWNVALUE = 100;
const int KNIGHTVALUE = 300;
const int BISHOPVALUE = 300;
const int ROOKVALUE = 500;
const int QUEENVALUE = 900;
const int KINGVALUE = 9999;

const int ROW = 10;
const int COLUMN = 1;

const int MAX_MOVEGEN_COUNT = 250;
const int MAX_MOVENUMBER = 200;
const int MAX_DEPTH = 6;
const int EVAL_DEPTH = 4;

const int ERRORCODE = 970201;
const int STALEMATE_BOUND = -100;


/*                                   ENUMERATION                              */
enum squareType {
     EMPTYSQUARE,
     WHITEPAWN, WHITEKNIGHT, WHITEBISHOP, WHITEROOK, WHITEQUEEN, WHITEKING,
     BLACKPAWN, BLACKKNIGHT, BLACKBISHOP, BLACKROOK, BLACKQUEEN, BLACKKING,
     ERRORSQUARE
};
enum fileRank120 {
     A8 = 21, B8, C8, D8, E8, F8, G8, H8,
     A7 = 31, B7, C7, D7, E7, F7, G7, H7,
     A6 = 41, B6, C6, D6, E6, F6, G6, H6,
     A5 = 51, B5, C5, D5, E5, F5, G5, H5,
     A4 = 61, B4, C4, D4, E4, F4, G4, H4,
     A3 = 71, B3, C3, D3, E3, F3, G3, H3,
     A2 = 81, B2, C2, D2, E2, F2, G2, H2,
     A1 = 91, B1, C1, D1, E1, F1, G1, H1
};
enum color { BLACK = -1, 
             NEITHER = 0,
             WHITE = 1
};
enum moveType {
     NORMAL, DOUBLEMOVE, ENPASSANT, QUEENSIDE_CASTLING, KINGSIDE_CASTLING,
     KNIGHT_PROMOTION, BISHOP_PROMOTION, ROOK_PROMOTION, QUEEN_PROMOTION,
     DECLARE_50
};
enum castlingCheck {WKCASTLING, WQCASTLING, BKCASTLING, BQCASTLING};
enum result {BLACK_WIN = -1, TIE, WHITE_WIN, NOT_FINISHED};
enum commandType {ERROR_COMMAND,
     MOVE,
     DISPLAY_BOARD,
     DISPLAY_FEN,
     BOARD_RESET,
     PERFT,
     QUIT,
     DIVIDE,
     UNDO_MOVE,
     COM_MAKE_MOVE,
     PRINT_ALL_MOVES,
     ALPHABETA_COMPARE,
     EVALUATE_BOARD,
     ALPHABETA_SPEED_CHECK,
     PRINT_SAVED_FEN
};
enum endType {CHECKMATE, STALEMATE_MOVE, STALEMATE_50, STALEMATE_75, STALEMATE_3F, NOTMATE};

//  PCSQ Tables from 
//  https://chessprogramming.wikispaces.com/Simplified+evaluation+function
const std::array<int, 120> PAWN_PCSQTable = {
     //  Possibly use -9999 for error squares for safety
     /*
     -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999,
     -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999, -9999,
     -9999,     0,     0,     0,     0,     0,     0,     0,     0, -9999,
     -9999,     0,     0,     0,     0,     0,     0,     0,     0, -9999,
     0,    50,    50,    50,    50,    50,    50,    50,    50,     0,*/
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0, 50, 50, 50, 50, 50, 50, 50, 50,  0,
     0, 10, 10, 20, 30, 30, 20, 10, 10,  0,
     0,  5,  5, 10, 25, 25, 10,  5,  5,  0,
     0,  0,  0,  0, 20, 20,  0,  0,  0,  0,
     0,  5, -5,-10,  0,  0,-10, -5,  5,  0,
     0,  5, 10, 10,-20,-20, 10, 10,  5,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
const std::array<int, 120> KNIGHT_PCSQTable = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,-50,-40,-30,-30,-30,-30,-40,-50,  0,
     0,-40,-20,  0,  0,  0,  0,-20,-40,  0,
     0,-30,  0, 10, 15, 15, 10,  0,-30,  0,
     0,-30,  5, 15, 20, 20, 15,  5,-30,  0,
     0,-30,  0, 15, 20, 20, 15,  0,-30,  0,
     0,-30,  5, 10, 15, 15, 10,  5,-30,  0,
     0,-40,-20,  0,  5,  5,  0,-20,-40,  0,
     0,-50,-40,-30,-30,-30,-30,-40,-50,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
const std::array<int, 120> BISHOP_PCSQTable = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,-20,-10,-10,-10,-10,-10,-10,-20,  0,
     0,-10,  0,  0,  0,  0,  0,  0,-10,  0,
     0,-10,  0,  5, 10, 10,  5,  0,-10,  0,
     0,-10,  5,  5, 10, 10,  5,  5,-10,  0,
     0,-10,  0, 10, 10, 10, 10,  0,-10,  0,
     0,-10, 10, 10, 10, 10, 10, 10,-10,  0,
     0,-10,  5,  0,  0,  0,  0,  5,-10,  0,
     0,-20,-10,-10,-10,-10,-10,-10,-20,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
const std::array<int, 120> ROOK_PCSQTable = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  5, 10, 10, 10, 10, 10, 10,  5,  0,
     0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
     0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
     0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
     0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
     0, -5,  0,  0,  0,  0,  0,  0, -5,  0,
     0,  0,  0,  0,  5,  5,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
const std::array<int, 120> QUEEN_PCSQTable = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,-20,-10,-10, -5, -5,-10,-10,-20,  0,
     0,-10,  0,  0,  0,  0,  0,  0,-10,  0,
     0,-10,  0,  5,  5,  5,  5,  0,-10,  0,
     0, -5,  0,  5,  5,  5,  5,  0, -5,  0,
     0,  0,  0,  5,  5,  5,  5,  0, -5,  0,
     0,-10,  5,  5,  5,  5,  5,  0,-10,  0,
     0,-10,  0,  5,  0,  0,  0,  0,-10,  0,
     0,-20,-10,-10, -5, -5,-10,-10,-20,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
const std::array<int, 120> KING_PCSQTable = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,-30,-40,-40,-50,-50,-40,-40,-30,  0,
     0,-30,-40,-40,-50,-50,-40,-40,-30,  0,
     0,-30,-40,-40,-50,-50,-40,-40,-30,  0,
     0,-30,-40,-40,-50,-50,-40,-40,-30,  0,
     0,-20,-30,-30,-40,-40,-30,-30,-20,  0,
     0,-10,-20,-20,-20,-20,-20,-20,-10,  0,
     0, 20, 20,  0,  0,  0,  0, 20, 20,  0,
     0, 20, 30, 10,  0,  0, 10, 30, 20,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};
const std::array<int, 120> KING_PCSQTable_ENDGAME = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,-50,-40,-30,-20,-20,-30,-40,-50,  0,
     0,-30,-20,-10,  0,  0,-10,-20,-30,  0,
     0,-30,-10, 20, 30, 30, 20,-10,-30,  0,
     0,-30,-10, 30, 40, 40, 30,-10,-30,  0,
     0,-30,-10, 30, 40, 40, 30,-10,-30,  0,
     0,-30,-10, 20, 30, 30, 20,-10,-30,  0,
     0,-30,-30,  0,  0,  0,  0,-30,-30,  0,
     0,-50,-30,-30,-30,-30,-30,-30,-50,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};
