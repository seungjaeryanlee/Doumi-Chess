#pragma once
/// <summary>
/// This function prints the given board to console.
/// </summary>
/// <param name="board">The board that will be printed to console.</param>
void printBoard(const Board& board);
/// <summary>
/// This function prints the given board to console excluding the error squares.
/// </summary>
/// <param name="board">The board that will be printed to console.</param>
void printSimpleBoard(const Board& board);
/// <summary>
/// This function prints the given move to console in filerank-filerank notation.
/// </summary>
/// <param name="moveNumber">The move number of the move that will be printed to console</param>
/// <param name="move">The move that will be printed to console</param>
std::string printMove(const int moveNumber, const Move& move);
/// <summary>
/// This function prints the basic menu of possible user commands.
/// </summary>
void printMenu();
/// <summary>
/// This function prints the extensive menu of possible user commands, including some options created for debugging the code.
/// </summary>
void printDebugMenu();
// TODO: Add documentation
void savePGN(const result gameResult, const Move *savedMove, const int saveIndex, const bool spectate, const int userColor);