#pragma once
void moveGeneration(const Board& board, Move moveList[250], int *moveCount);
void pawnMoveGeneration(const Board& board, int position, Move moveList[250], int *moveCount);
void knightMoveGeneration(const Board& board, int position, Move moveList[250], int *moveCount);
void bishopMoveGeneration(const Board& board, int position, Move moveList[250], int *moveCount);
void rookMoveGeneration(const Board& board, int position, Move moveList[250], int *moveCount);
void queenMoveGeneration(const Board& board, int position, Move moveList[250], int *moveCount);
void kingMoveGeneration(const Board& board, int position, Move moveList[250], int *moveCount);

void castlingMoveGeneration(const Board& board, Move moveList[250], int *moveCount);
void promotionMoveGeneration(const Board& board, int position, Move moveList[250], int *moveCount);
void enpassantMoveGeneration(const Board& board, Move moveList[250], int *moveCount);

void addMove(int initial, int terminal, int moveType, Move moveList[250], int *moveCount);
void addPromotionMove(int initial, int terminal, Move moveList[250], int *moveCount);

void legalMoves(Board board, Move moveList[250], int moveCount, Move legalMoveList[250], int *legalMoveCount);
bool squareAttackCheck(Board board, int position);
