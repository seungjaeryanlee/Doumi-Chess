#pragma once
void moveGeneration(const Board& board, MoveList& moveList);
void pawnMoveGeneration(const Board& board, const int position, MoveList& moveList);
void knightMoveGeneration(const Board& board, const int position, MoveList& moveList);
void bishopMoveGeneration(const Board& board, const int position, MoveList& moveList);
void rookMoveGeneration(const Board& board, const int position, MoveList& moveList);
void queenMoveGeneration(const Board& board, const int position, MoveList& moveList);
void kingMoveGeneration(const Board& board, const int position, MoveList& moveList);

void castlingMoveGeneration(const Board& board, MoveList& moveList);
void promotionMoveGeneration(const Board& board, const int position, MoveList& moveList);
void enpassantMoveGeneration(const Board& board, MoveList& moveList);

void addMove(int initial, int terminal, int moveType, MoveList& moveList);
void addPromotionMove(int initial, int terminal, MoveList& moveList);

bool squareAttackCheck(Board board, int position);
