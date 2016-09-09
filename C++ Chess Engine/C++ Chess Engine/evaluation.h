#pragma once
#include "protos.h"

/// <summary>
/// This function returns evaluation score of the given board using piece values and PCSQ tables. Positive score signifies white's advantage.
/// </summary>
/// <param name="board">The board that will be evaluated.</param>
/// <returns>The score of the board</returns>
int boardEvaluation(const Board& board);
/// <summary>
/// This function returns the row-reversed position of the given position in board index notation. Used in applying PCSQ table for black.
/// </summary>
/// <param name="position">The position that will be row-reversed.</param>
/// <returns>The row-reversed position</returns>
int reversePosition(const int position);
/// <summary>
/// This function finds the best move using negaMax recursion on the given board for given depth. This function should only be called in rootNegaMax().
/// </summary>
/// <param name="depth">The depth of the recursion.</param>
/// <param name="board">The board that will be analyzed.</param>
/// <returns>The evaluation score of best move.</returns>
int negaMax(const int depth, Board& board);
/// <summary>
/// This function is the root function for negaMax().
/// </summary>
/// <param name="depth">The depth of the recursion.</param>
/// <param name="board">The board that will be analyzed.</param>
/// <param name="bestMove">The best move found.</param>
/// <returns>The evaluation score of best move.</returns>
int rootNegaMax(const int depth, Board& board, Move& bestMove);
/// <summary>
/// This function finds the best move using negaMax recursion and alpha-beta pruning on the given board for given depth. This function should only be called in rootAlphabeta().
/// </summary>
/// <param name="maxDepth">The depth of the recursion.</param>
/// <param name="board">The board that will be analyzed.</param>
/// <param name="alpha">The alpha value used for alpha-beta pruning.</param>
/// <param name="beta">The beta value used for alpha-beta pruning.</param>
/// <returns>The evaluation score of best move.</returns>
int alphabeta(const int maxDepth, Board& board, int alpha, int beta);
/// <summary>
/// This function is the root function for negaMax().
/// </summary>
/// <param name="maxDepth">The depth of the recursion.</param>
/// <param name="board">The board that will be analyzed.</param>
/// <param name="alpha">The alpha value used for alpha-beta pruning.</param>
/// <param name="beta">The beta value used for alpha-beta pruning.</param>
/// <param name="bestMove">The best move found.</param>
/// <returns>The evaluation score of best move.</returns>
int rootAlphabeta(const int maxDepth, Board board, int alpha, int beta, Move& bestMove);