#include <iostream>
#include "GameBoard.h";

/*
COORDINATE MAPPING: (Example 'A1')
 
i = 8 - (1) 
j = ('A') - 'A'

*/


GameBoard::GameBoard() {
	this->initBoard();
}

GameBoard::~GameBoard() {

}

void GameBoard::initBoard() {
	int bird_pos_index = 0;
	// Initialize board, the Larva [D2] and Birds [A1, C1, E1, G1]
	for(int i = 0; i < (sizeof(this->board)/sizeof(* this->board)); ++i) {
		for(int j = 0; j < (sizeof(this->board[i])/sizeof(* this->board[i])); ++j) {
			if(i == 7 && j % 2 == 0) {
				this->board[i][j] = 'B';
				this->bird_pos[bird_pos_index][0] = i;
				this->bird_pos[bird_pos_index][1] = j;
				++bird_pos_index;
			} else if(i == 6 && j == 3) {
				this->board[i][j] = 'L';
				this->larva_pos[0] = i;
				this->larva_pos[1] = j;
			} else {
				this->board[i][j] = '\0';
			}
		}
	}

	curr_turn = TurnType::P_ONE_TURN;
}

void GameBoard::printBoard() {
	// Print out the letters at the top
	std::cout << "            A   B   C   D   E   F   G   H" << std::endl;

	// Print the a border and then loop
	std::cout << "          +---+---+---+---+---+---+---+---+" << std::endl;

	// Print the entries + Side Numbers, and then a bottom border
	for(int i = 0; i < (sizeof(this->board)/sizeof(* this->board)); ++i) {
		// The inital | symbol and line number
		if(i == (sizeof(this->board)/sizeof(* this->board))-1) {
			std::cout << "[FENCE] 1 |";
		} else {
			std::cout << "        " << (8-i) << " |";
		}
		
		for(int j = 0; j < (sizeof(this->board[i])/sizeof(* this->board[i])); ++j) {
			std::cout << " " << this->board[i][j] << " |";
		}
		
		std::cout <<  " " << (8-i);

		if(i == (sizeof(this->board)/sizeof(* this->board))-1) {
			std::cout << " [FENCE]";
		}
		
		std::cout << std::endl << "          +---+---+---+---+---+---+---+---+" << std::endl;
	}

	// Print the Letters at the Bottom again.
	std::cout << "            A   B   C   D   E   F   G   H" << std::endl;
	// also end by printing which player is to play next:
	std::cout << std::endl;
}

bool GameBoard::movePiece(char fromPos[2], char toPos[2]) {
	
	int from_i = '8' - fromPos[1];
	int from_j = fromPos[0] - 'A';
	int to_i = '8' - toPos[1];
	int to_j = toPos[0] - 'A';
	
	if(!validatePosition(from_i, from_j) || !validatePosition(to_i, to_j)) {
		return false;
	}

	// check the positions based on who's turn it is.
	if(curr_turn == TurnType::P_ONE_TURN) {
		// make sure that the piece is the larva.
		if(board[from_i][from_j] != 'L') {
			return false;
		}
		if(!(													// not any of the following:
				(from_i == (to_i + 1) && from_j == (to_j + 1))	// move up-left
			||	(from_i == (to_i + 1) && from_j == (to_j - 1))	// move up-right
			||	(from_i == (to_i - 1) && from_j == (to_j + 1))	// move down-left
			||	(from_i == (to_i - 1) && from_j == (to_j - 1))	// move down-right
		)) {
			return false;
		}
	} else { // curr_turn == TurnType::P_TWO_TURN
		// make sure that the piece is a bird
		if(board[from_i][from_j] != 'B') {
			return false;
		}
		
		if(!(														// not any of the following:
				(from_i == (to_i + 1) && from_j == (to_j + 1))	// move up-left
			||	(from_i == (to_i + 1) && from_j == (to_j - 1))	// move up-right
		)) {
			return false;
		}		
	}

	// in both scenarios, we check if the toPos is empty. Can't be stacking pieces.
	if(board[to_i][to_j] != '\0') {
		return false;
	}

	// now that all checking is out of the way, make the move and switch turns.
	board[from_i][from_j] = '\0';
	board[to_i][to_j] = (curr_turn == TurnType::P_ONE_TURN) ? 'L' : 'B';

	// update the larva/bird position.
	if(curr_turn == TurnType::P_ONE_TURN) {
		this->larva_pos[0] = to_i;
		this->larva_pos[1] = to_j;
	} else {
		for(int i = 0; i < 4; ++i) {
			if(bird_pos[i][0] == from_i && bird_pos[i][1] == from_j) {
				bird_pos[i][0] == to_i;
				bird_pos[i][1] == to_j;
				break;
			}
		}
	}

	curr_turn = (curr_turn == TurnType::P_ONE_TURN) ? TurnType::P_TWO_TURN : TurnType::P_ONE_TURN;
	
	return true;
}

bool GameBoard::isPlayerOneTurn() {
	return (curr_turn == TurnType::P_ONE_TURN);
}

int GameBoard::isEndState() {
	// check if larva has made it to line 1 (the fence)
	if(board[7][0] == 'L' || board[7][2] == 'L' || board[7][4] == 'L' || board[7][6] == 'L') {
		return 1;
	}
	
	// check is the larva is surrounded.
	bool is_surrounded = true;
	for(int i = -1; i < 2; i += 2) { // i checks down and up
		for(int j = -1; j < 2; j += 2) { //j check left and right
			if(validatePosition(larva_pos[0] + i, larva_pos[1] + j) && board[larva_pos[0] + i][larva_pos[1] + j] == '\0') {
				is_surrounded = false;
				break;
			}
		}
		if(!is_surrounded) {
			break;
		}
	}
	if(is_surrounded) {
		return -1;
	}

	// check that the birds can move. if not, larva wins.
	bool can_move_bird = false;
	for(int i = 0; i < 4; ++i) { // i iterates the array of birds.
		for(int j = -1; j < 2; j += 2) { // j checks left and right
			if(validatePosition(bird_pos[i][0] - 1, bird_pos[i][1] + j) && board[bird_pos[i][0] - 1][bird_pos[i][1] + j] == '\0') {
				can_move_bird = true;
				break;
			}
		}
		if(can_move_bird) {
			break;
		}
	}
	if(!can_move_bird) {
		return 1;
	}

	return 0;
}

bool GameBoard::validatePosition(int i, int j) {
	
	// check bounds
	if(i >= 8 || i < 0 || j >= 8 || j < 0) {
		return false;
	}
	
	// check if the piece lands on a diagonal/black square
	if(i % 2 == j % 2) {
		return false;
	}

	return true;
}