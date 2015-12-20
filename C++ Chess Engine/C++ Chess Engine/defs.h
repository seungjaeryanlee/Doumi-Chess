#pragma once
/*                                    DEFINITION                              */
#define PAWNVALUE 100
#define KNIGHTVALUE 300
#define BISHOPVALUE 300
#define ROOKVALUE 500
#define QUEENVALUE 900
#define KINGVALUE 9999
#define ROW 10
#define COLUMN 1
#define MAX_MOVEGEN_COUNT 250
#define MAX_MOVENUMBER 200
#define MAXIMUM_DEPTH 6
#define EVAL_DEPTH 4

typedef unsigned long long u64;



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
     KNIGHT_PROMOTION, BISHOP_PROMOTION, ROOK_PROMOTION, QUEEN_PROMOTION
};
enum castlingCheck {WKCASTLING, WQCASTLING, BKCASTLING, BQCASTLING};
enum result {BLACK_WIN = -1, TIE, WHITE_WIN};