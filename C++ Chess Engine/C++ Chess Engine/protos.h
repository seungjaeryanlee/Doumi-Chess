#pragma once
#include "defs.h"
#include "move.h"
#include "board.h"
#include <Windows.h>

/*                                    CALL COUNT                               */
void moveGenCalled();
/// <summary>
/// This function prints the basic menu of possible user commands.
/// </summary>
void printMenu();
/// <summary>
/// This function prints the extensive menu of possible user commands, including some options created for debugging the code.
/// </summary>
void printDebugMenu();


/*                                   RECURSION                                */
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
/// <summary>
/// This function applies the given move to the given board.
/// </summary>
/// <param name="board">The board that the move will apply to.</param>
/// <param name="move">The move to be applied to the board.</param>
/// <returns>The piece that was captured with the move</returns>
int makeMove(Board &board, const Move& move);
/// <summary>
/// This function reverts the given move on the given board.
/// </summary>
/// <param name="board">The board that the move will be reverted in</param>
/// <param name="move">The move that will be reverted</param>
/// <param name="capturedPiece">The piece that was captured by the move</param>
void undoMove(Board &board, const Move& move, const int capturedPiece);


/*                                  MISC                                      */
// TODO: Check if this can be integrated inside makeMove
/// <summary>
/// This function updates the castling rights of the given board if the given move changed it. This is called after the move was applied to the board.
/// </summary>
/// <param name="board">The board that will be checked.</param>
/// <param name="move">The move that was made that could have changed the castling rights.</param>
void updateCastling(Board& board, const Move& move);
/// <summary>
/// This function updates the en passant square of the given board if the given move changed it. This is called after the move was applied to the board.
/// </summary>
/// <param name="board">The board that will be checked.</param>
/// <param name="move">The move that was made that could have changed the en passant square.</param>
void updateEnPassant(Board& board, const Move& move);
/// <summary>
/// This function updates the half move clock of the given board if the given move changed it. This is called after the move was applied to the board.
/// </summary>
/// <param name="board">The board that will be checked.</param>
/// <param name="move">The move that was made that has changed the half move clock.</param>
void updateHalfMoveClock(Board& board, const Move& move);
/// <summary>
/// This function updates the move number of the given board if the given move changed it. This is called after the move was applied to the board.
/// </summary>
/// <param name="board">The board that will be checked.</param>
/// <param name="move">The move that was made that could have changed the move number.</param>
void updateMoveNumber(Board& board);
/// <summary>
/// This function updates the non-board components (casting, enpassant, halfmoveclock, movenumber) of the given board if the given move changed it. This is called after the move was applied to the board.
/// </summary>
/// <param name="board">The board that will be checked.</param>
/// <param name="move">The move that was made that could have changed the board properties.</param>
// TODO: UPDATE
void updateBoard(Board& board, const Move& move, const int capturedPiece);

/// <summary>
/// This function checks the game state of the given board.
/// </summary>
/// <param name="board">The board that will be checked for its game state.</param>
/// <returns>The game state of the board.</returns>
gameState checkGameState(const Board& board);
/// <summary>
/// This function checks if in the given board a fifty move rule can be declared.
/// </summary>
/// <param name="board">The board that will be checked for fifty move rule.</param>
/// <returns>True if fifty move rule is possible. False if not.</returns>
bool fiftyMoveCheck(const Board& board);
