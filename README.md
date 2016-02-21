# C++ Chess Engine
## Contributor
sjlee235
## Code Explained (In construction)
1. Board Representation  
I use size 120 array to represent a board. The 12x10 array uses the middle 8x8 as the chessboard, and all the other squares are declared error squares. With these error squares, it is easier to generate moves for bishop, rook, queen and knight.
2. Board Evaluation  
3. Move Generation  
4. Recursion  
I plan to use Negamax algorithm for this engine.

## Current Progress
1. Board Representation - **Complete**  
2. Board Evaluation - **Complete**  
3. Move Generation - **Complete**  
  - Normal Move Generation - **Complete**  
  - Special Move Generation (Promotion, Enpassant, Castling) - **Complete**
  - Legal Move Check - **Complete**  
  - Performance Test (a.k.a. perft test) - **Complete**  
    - Change Existing Functions for pointer arguments - **Complete**  
4. Recursion - *In progress*  
  - Implement Negamax - **Complete**  
  - Implement Alphabeta Pruning - **Complete**  
  - Test Recursion - *In progress*  
  - Consider Edge Cases (Checkmate, Stalemate) - *In progress*
5. Other Minor Details  
  - Stalemate rules - *In progress*  
    - Threefold Repetition (FIDE 9.2)
    - Fifty Move Rule (FIDE 9.3) - *In progress*  
    - 75 Move Rule (FIDE 9.6) - **Complete**  
  - Resignation  
6. Qt Widget
  - Basic Layout Setup
  - Pipe between Engine & Widget
  - Test Widget

## TODOs  
1. Add Undo Move command Player vs. COM
2. Add 50 Move Rule option in moveGen
3. Check Threefold Repetition

## Non-necessary Addition to Chess Engine
1. Opening Book  
2. Clock  
3. Endgame Book  
4. WinBoard or UCI  
5. Create own GUI  
6. Complex heuristic for Board Evaluation  
*These will be worked on after the basic chess engine is completed. (Not in order)*  

## References  
The Piece Square Tables of this version is from the following link:   https://chessprogramming.wikispaces.com/Simplified+evaluation+function  

We used / plan to use perft tests from these websites:  
https://chessprogramming.wikispaces.com/Perft+Results  
http://www.chesskit.com/training/fenkit/index.php?page=p9&d=Page%209
