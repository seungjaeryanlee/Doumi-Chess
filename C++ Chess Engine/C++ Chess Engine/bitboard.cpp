#include "bitboard.h"
#include "protos.h"

int bitCount(bitboard bb) {
     int count = 0;
     for (count = 0; bb; count++) {
          bb &= bb - 1;
     }
     return count;
}

int bitboardEvaluation(Board& board) {
     // TODO: Improve with Having variables for pieceCounts
     int score = 0;
     score = bitCount(board.getWKBB) * WHITEKING
           + bitCount(board.getWQBB) * WHITEQUEEN
           + bitCount(board.getWRBB) * WHITEROOK
           + bitCount(board.getWBBB) * WHITEBISHOP
           + bitCount(board.getWNBB) * WHITEKNIGHT
           + bitCount(board.getWPBB) * WHITEPAWN
           - bitCount(board.getBKBB) * BLACKKING
           - bitCount(board.getBQBB) * BLACKQUEEN
           - bitCount(board.getBRBB) * BLACKROOK
           - bitCount(board.getBNBB) * BLACKBISHOP
           - bitCount(board.getBNBB) * BLACKKNIGHT
           - bitCount(board.getBPBB) * BLACKPAWN;

     // TODO: Read and Add PCSQ Table Score
     
}