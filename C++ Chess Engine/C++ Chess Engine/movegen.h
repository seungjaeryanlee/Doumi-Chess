#pragma once

/// <summary>
/// This function returns the list of possible legal moves
/// </summary>
/// <param name="board">The board that will be checked for all possible moves.</param>
/// <returns>The list of possible legal moves.</returns>
MoveList moveGeneration(const Board& board);
/// <summary>
/// This function returns the list of possible pawn moves
/// </summary>
/// <param name="board">The board that will be checked for all possible pawn moves.</param>
/// <param name="position">The board index notation of the position the pawn is in.</param>
/// <param name="moveList">The list of moves where the new moves will be added in.</param>
void pawnMoveGeneration(const Board& board, const int position, MoveList& moveList);
/// <summary>
/// This function returns the list of possible knight moves
/// </summary>
/// <param name="board">The board that will be checked for all possible knight moves.</param>
/// <param name="position">The board index notation of the position the knight is in.</param>
/// <param name="moveList">The list of moves where the new moves will be added in.</param>
void knightMoveGeneration(const Board& board, const int position, MoveList& moveList);
/// <summary>
/// This function returns the list of possible bishop moves
/// </summary>
/// <param name="board">The board that will be checked for all possible bishop moves.</param>
/// <param name="position">The board index notation of the position the bishop is in.</param>
/// <param name="moveList">The list of moves where the new moves will be added in.</param>
void bishopMoveGeneration(const Board& board, const int position, MoveList& moveList);
/// <summary>
/// This function returns the list of possible rook moves
/// </summary>
/// <param name="board">The board that will be checked for all possible rook moves.</param>
/// <param name="position">The board index notation of the position the rook is in.</param>
/// <param name="moveList">The list of moves where the new moves will be added in.</param>
void rookMoveGeneration(const Board& board, const int position, MoveList& moveList);
/// <summary>
/// This function returns the list of possible queen moves
/// </summary>
/// <param name="board">The board that will be checked for all possible queen moves.</param>
/// <param name="position">The board index notation of the position the queen is in.</param>
/// <param name="moveList">The list of moves where the new moves will be added in.</param>
void queenMoveGeneration(const Board& board, const int position, MoveList& moveList);
/// <summary>
/// This function returns the list of possible king moves
/// </summary>
/// <param name="board">The board that will be checked for all possible king moves.</param>
/// <param name="position">The board index notation of the position the king is in.</param>
/// <param name="moveList">The list of moves where the new moves will be added in.</param>
void kingMoveGeneration(const Board& board, const int position, MoveList& moveList);


/// <summary>
/// This function returns the list of possible castling moves
/// </summary>
/// <param name="board">The board that will be checked for all possible castling moves.</param>
/// <param name="moveList">The list of moves where the new moves will be added in.</param>
void castlingMoveGeneration(const Board& board, MoveList& moveList);
/// <summary>
/// This function returns the list of possible promotion moves
/// </summary>
/// <param name="board">The board that will be checked for all possible promotion moves.</param>
/// <param name="position">The board index notation of the position the pawn is in.</param>
/// <param name="moveList">The list of moves where the new moves will be added in.</param>
void promotionMoveGeneration(const Board& board, const int position, MoveList& moveList);
/// <summary>
/// This function returns the list of possible en passant moves
/// </summary>
/// <param name="board">The board that will be checked for all possible en passant moves.</param>
/// <param name="position">The board index notation of the position the pawn is in.</param>
/// <param name="moveList">The list of moves where the new moves will be added in.</param>
void enpassantMoveGeneration(const Board& board, MoveList& moveList);

/// <summary>
/// This function adds a given move to the given movelist.
/// </summary>
/// <param name="initial">The initial square of the move in board index notation.</param>
/// <param name="terminal">The terminal square of the move in board index notation.</param>
/// <param name="moveType">The type of the move.</param>
/// <param name="moveList">The list of moves where the new move will be added in.</param>
void addMove(int initial, int terminal, int moveType, MoveList& moveList);
/// <summary>
/// This function adds four promotion moves from the initial square to the terminal square to the given movelist.
/// </summary>
/// <param name="initial">The initial square of the moves in board index notation.</param>
/// <param name="terminal">The terminal square of the moves in board index notation.</param>
/// <param name="moveList">The list of moves where the new moves will be added in.</param>
void addPromotionMove(int initial, int terminal, MoveList& moveList);

