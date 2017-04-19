/******************************************************************************/
/* debug.h                                                                    */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#pragma once

#include "board.h"

/// <summary>
/// This function finds the number of possible move sequences for given depth and divides them into categories of first few moves given by maxDepth and prints them to console.
/// </summary>
/// <param name="depth">The current depth of the recursion.</param>
/// <param name="maxDepth">The maximum depth the moves will be divided in output</param>
/// <param name="board">The board being checked.</param>
/// <param name="showOutput">Wheter or not to show intermediate output.</param>
/// <returns>The number of possible move sequences of given depth.</returns>
uint64_t divide(const int depth, const int maxDepth, Board& board, const bool showOutput);
/// <summary>
/// This function finds the number of possible move sequences for given depth and divides them into categories of first few moves given by maxDepth amd saves them into a file.
/// </summary>
/// <param name="depth">The current depth of the recursion.</param>
/// <param name="maxDepth">The maximum depth the moves will be divided in output</param>
/// <param name="board">The board being checked.</param>
/// <param name="showOutput">Wheter or not to show intermediate output.</param>
/// <returns>The number of possible move sequences of given depth.</returns>
uint64_t divide2(const int depth, const int maxDepth, Board& board, const bool showOutput);