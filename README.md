# Doumi Chess
## Description
**Doumi Chess** is a  program written in C++ that can play a full game of chess against human players or other computer chess engines. 
## Contributor
sjlee235
## Code Explained (In construction)
1. Board Representation  
I have a Board class to represent a board. It consists of a size 120 array to represent the location of each piece. The 12x10 array uses the middle 8x8 as the chessboard, and all the other squares are declared error squares. With these error squares, it is easier to generate moves for bishop, rook, queen and knight. Eventually I plan to change to bitboard mechanism. The Board class also holds variables for castling rights, en passant square, turn, half move clock, and move number.
2. Board Evaluation  
A board is evaluated based on its position using a **Piece-Square Table** and also its **Piece Value**. I plan to include **Piece Combinations** (N+B, N+N, B+B, etc...) for better evaluation.
3. Move Generation  
Move generation is done first in a pseudo-legal way, disregarding whether the move is impossible because it endangers the king. Then each move is assessed on whether it is actual legal. I plan to optimize the assessment process so that only moves by pinned pieces are checked.
4. Recursion  
I am using **Negamax Algorithm** with **Alphabeta Pruning** for this engine.

## TODOs  
*These are not in order.*  
1. Opening Book  
2. Clock  
3. Endgame Book  
4. Complete Implementation of of UCI Protocol  
6. Complex heuristic for Board Evaluation  
7. Resignation  
8. Move Ordering for better Alphabeta Pruning
9. BitBoards  
10. Piece Combination Values  
11. PieceCount in Board class

## Dependency Diagram
![Dependency](/../master/diagram.png?raw=true)


## References  
The Piece Square Tables of this version is from the following link:   https://chessprogramming.wikispaces.com/Simplified+evaluation+function  

To validate the move generation function, the perft tests from these websites were used:  
https://chessprogramming.wikispaces.com/Perft+Results  
http://www.chesskit.com/training/fenkit/index.php?page=p9&d=Page%209
