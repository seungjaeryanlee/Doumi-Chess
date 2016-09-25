#pragma once
#include "protos.h"


/// <summary>
/// This function finds the best move using negaMax recursion and alpha-beta pruning on the given board for given depth. This function should only be called in rootAlphabeta().
/// </summary>
/// <param name="maxDepth">The depth of the recursion.</param>
/// <param name="board">The board that will be analyzed.</param>
/// <param name="alpha">The alpha value used for alpha-beta pruning.</param>
/// <param name="beta">The beta value used for alpha-beta pruning.</param>
/// <returns>The evaluation score of best move.</returns>
// int alphabeta(const int maxDepth, Board& board, int alpha, int beta);
// TODO: Update documentation
int alphabeta(const int depth, Board& board, int alpha, int beta, Board savedBoard[MAX_MOVENUMBER], int saveIndex);
/// <summary>
/// This function is the root function for negaMax().
/// </summary>
/// <param name="maxDepth">The depth of the recursion.</param>
/// <param name="board">The board that will be analyzed.</param>
/// <param name="alpha">The alpha value used for alpha-beta pruning.</param>
/// <param name="beta">The beta value used for alpha-beta pruning.</param>
/// <param name="bestMove">The best move found.</param>
/// <returns>The evaluation score of best move.</returns>
//int rootAlphabeta(const int maxDepth, Board board, int alpha, int beta, Move& bestMove);
// TODO: Update documentation
int rootAlphabeta(const int maxDepth, Board board, int alpha, int beta, Move& bestMove, Board savedBoard[MAX_MOVENUMBER], int saveIndex);