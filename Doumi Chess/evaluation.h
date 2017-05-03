/******************************************************************************/
/* evaluation.h                                                               */
/* Seung Jae (Ryan) Lee                                                       */
/******************************************************************************/

#pragma once
#include "move.h"
#include "board.h"

const int DEFAULT_ALPHA = -999999;
const int DEFAULT_BETA = 999999;

struct Variation {
     int length;             // Number of moves in the variation.
     Move moves[MAX_DEPTH];  // The moves.
};

/// <summary>
/// This function finds the best move using negaMax recursion and alpha-beta pruning on the given board for given depth. This function should only be called in rootAlphabeta().
/// </summary>
/// <param name="maxDepth">The depth of the recursion.</param>
/// <param name="board">The board that will be analyzed.</param>
/// <param name="alpha">The alpha value used for alpha-beta pruning.</param>
/// <param name="beta">The beta value used for alpha-beta pruning.</param>
/// <param name="pVariation">Current variation.</param>
/// <param name="savedBoard">The past boards. Used for threefold repetition checking.</param>
/// <param name="saveIndex">Number of boards in savedBoard.</param>
/// <returns>The evaluation score of best move.</returns>
int alphabeta(const int depth, Board& board, int alpha, int beta, Variation* pVariation, Board savedBoard[MAX_MOVENUMBER], int saveIndex);
/// <summary>
/// This function is the root function for negaMax().
/// </summary>
/// <param name="maxDepth">The depth of the recursion.</param>
/// <param name="board">The board that will be analyzed.</param>
/// <param name="principalVariation">The best variation found after alphabeta.</param>
/// <param name="savedBoard">The past boards. Used for threefold repetition checking.</param>
/// <param name="saveIndex">Number of boards in savedBoard.</param>
/// <returns>The evaluation score of best move.</returns>
int rootAlphabeta(const int maxDepth, Board board, Variation* principalVariation, Board savedBoard[MAX_MOVENUMBER], int saveIndex);

/// <summary>
/// This function checks the game state of the given board.
/// </summary>
/// <param name="board">The board that will be checked for its game state.</param>
/// <param name="moveList">The list of legal moves allowed on the given board.</param>
/// <param name="savedBoard">The past boards. Used for threefold repetition checking.</param>
/// <param name="saveIndex">Number of boards in savedBoard.</param>
/// <returns>The game state of the board.</returns>
gameState checkGameState(Board& board, const MoveList& moveList, const Board savedBoard[MAX_MOVENUMBER], int saveIndex);

/// <summary>
/// This function prints the variation to given stream.
/// </summary>
/// <param name="stream">The stream that the variation would be printed on.</param>
/// <param name="var">The variation to be printed.</param>
void printVariation(std::ostream& stream, Variation& var);