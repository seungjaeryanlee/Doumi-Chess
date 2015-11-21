# C++ Chess Engine
## Contributor
sjlee235
## Code Explained (In construction)
1. Board Representation
2. Board Evaluation
3. Move Generation
4. Recursion  
I plan to use Negamax algorithm for this engine.

## Current Progress
1. Board Representation - **Complete**
2. Board Evaluation - **Complete**
3. Move Generation - *In progress*
  - Normal Move Generation - **Complete**
  - Special Move Generation (Promotion, Enpassant, Castling) - **Complete**
  - Legal Move Check - **Complete**  
  - Performance Test (a.k.a. perft test) - *In progress*  
    - Change Existing Functions for pointer arguments - **Complete**  
4. Recursion
  - Implement Negamax
  - Test Recursion
5. Other Minor Details  
  - Stalemate rules  
  - Resignation  

## TODOs (In construction)

## Non-necessary Addition to Chess Engine
1. Opening Book  
2. Clock  
3. Endgame Book  
4. WinBoard or UCI  
5. Create own GUI  
6. *These will be worked on after the basic chess engine is completed. (Not in order)*  

## References  
The Piece Square Tables of this version is from the following link:   https://chessprogramming.wikispaces.com/Simplified+evaluation+function  

We used / plan to use perft tests from these websites:  
https://chessprogramming.wikispaces.com/Perft+Results  
http://www.chesskit.com/training/fenkit/index.php?page=p9&d=Page%209
