#pragma once
void moveGeneration(const Board& board, Move legalMoveList[MAX_MOVEGEN_COUNT], int *legalMoveCount);
void pawnMoveGeneration(const Board& board, const int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount);
void knightMoveGeneration(const Board& board, const int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount);
void bishopMoveGeneration(const Board& board, const int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount);
void rookMoveGeneration(const Board& board, const int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount);
void queenMoveGeneration(const Board& board, const int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount);
void kingMoveGeneration(const Board& board, const int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount);

void castlingMoveGeneration(const Board& board, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount);
void promotionMoveGeneration(const Board& board, const int position, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount);
void enpassantMoveGeneration(const Board& board, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount);

void addMove(int initial, int terminal, int moveType, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount);
void addPromotionMove(int initial, int terminal, Move moveList[MAX_MOVEGEN_COUNT], int *moveCount);

bool squareAttackCheck(Board board, int position);
