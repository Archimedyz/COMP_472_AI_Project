----
###	COMP_472_AI_Project
----
####	Awais Ali
----

DESCRIPTION:
	
The application is a game called "Hungry Birds."
It is a two-player, turn-based game, in which player 1 controls a single piece, the larva, and player 2 controls 4 bird pieces.
The rules are as follows:
1. Larva plays first. Birds play second.
2. The Larva may move in the following for directions:
	- Up-Right
	- Up-Left
	- Down-Right
	- Down-Left
3. Each bird may move in the following directions:
	- Up-Right
	- Up-Left
4. No piece may move off the board.
5. No piece may onto another piece.
6. If the Larva reaches Row 1 (Marked with "Fence"), Player 1 wins.
7. If the Larva has not reached Row 1, and the Larva cannot move on it's turn. Player 2 wins.
8. If the Larva has not reached Row 1, and the Birds cannot move on their turn, Player 1 Wins.

Input for making moves, for any piece is of the format: "<src> <dest>" (this include the space), where "<src>", and "<dest>" are the source and destination corrdiantes of the piece respectively.
- Any of the following is a valid move input:
	* A1 B2
	* e2 D3
	* f4 g3
- The following are invalid move inputs:
	* a1b2
	* a1  b2
	* B1 c2 d3
	* E3 D
	* stuff
- After entering a move (be it human or AI), you must press the Return Key (or Enter Key) to continue if the input was valid. This is simply employed to check the input provided by the AI, as wel as the time taken.

NOTES:
	This application was a project for the Artificial Intelligence course offered at Concordia University (Montreal) in the Fall Semeters of 2015.
	The Application uses a MiniMax Algotirthm (no alpha-beta pruning) with a simple heuristic function.
	The AI players look  ahead 7 levels fromt he current state to make their decision, mostly due to the time restrictions on the project. (max: 3 seconds per move as either player.)

			
