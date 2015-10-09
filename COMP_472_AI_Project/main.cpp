#include <iostream>
#include <regex>
#include "GameBoard.h"
#include "Player.h"
#include "AIPlayer.h"
#include "HumanPlayer.h"

using namespace std;

typedef enum { HUMAN_HUMAN, HUMAN_AI, AI_HUMAN, AI_AI } Gamemode;

void mainMenu();
void flushInput();
void initGame(Gamemode g);
void play();

GameBoard * gb = new GameBoard();

Player * player_one;
Player * player_two;

int main() {
	// go tot the main menu

	mainMenu();

	std::cout << "Thanks for using this app. Press any key to terminate . . ." << endl;
	getchar();
	return 0;
}

void mainMenu() {
	int selection;

	bool terminate = false;
	bool selection_error = false;

	while(!terminate) {
		std::cout << "---------------------------------------------------" << endl
			 << "\tHUNGRY BIRDS" << endl
			 << "---------------------------------------------------" << endl
			 << "Choose a game mode." << endl
			 << "\t1. Human vs Human" << endl
			 << "\t2. Human vs AI (under construction)" << endl
			 << "\t3. AI vs Human (under construction)" << endl
			 << "\t4. AI vs AI (under construction)" << endl
			 << "\t5. Exit" << endl
			 << "---------------------------------------------------" << endl
			 << (selection_error ? "Invalid Selection. Please enter either 1 2 or 3." : "") << endl
			 << "Selection: ";
		try {
	
		} catch(int e) {
			selection_error = true;
			continue;
		}

		cin >> selection;
		flushInput();

		if(selection == 1) {
			initGame(HUMAN_HUMAN);
		} else if(selection == 2) {
			initGame(HUMAN_AI);
		} else if(selection == 3) {
			initGame(AI_HUMAN);
		} else if(selection == 4) {
			initGame(AI_AI);
		} else if(selection == 5) {
			terminate = true;
		} else {
			selection_error = true;
		}
	}	
}

void flushInput() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void initGame(Gamemode g) {
	// if there was a previous game, destroy the old player pointers.
	if(player_one != nullptr) {
		delete player_one;
		player_one = NULL;
	}
	if(player_two != nullptr) {
		delete player_two;
		player_two = NULL;
	}

	// Initialize players, and board.
	if(g == HUMAN_HUMAN) {
		player_one = new HumanPlayer();
		player_two = new HumanPlayer();
	} else if(g == HUMAN_AI) {
		player_one = new HumanPlayer();
		player_two = new AIPlayer();
		std::cout << std::endl << "Currently Under Construction. Please try again at a later time." << std::endl;
		return;
	} else if(g == AI_HUMAN) {
		player_one = new AIPlayer();
		player_two = new HumanPlayer();
		std::cout << std::endl << "Currently Under Construction. Please try again at a later time." << std::endl;
		return;
	} else { // only other option is g == AI_AI.
		player_one = new AIPlayer();
		player_two = new AIPlayer();
		std::cout << std::endl << "Currently Under Construction. Please try again at a later time." << std::endl;
		return;
	}

	play();
}

void play() {

	gb->initBoard();
	
	bool valid_move = true;

	int winner = gb->isEndState();

	while(winner == 0) {
		std::cout << endl;
		gb->printBoard();
		std::cout << (gb->isPlayerOneTurn() ? "Player 1" : "Player 2") << "'s Turn." << endl << endl;

		if(!valid_move) {
			if(gb->isPlayerOneTurn()) {
				std::cout << "=========================================================================\nInvalid Move. You must choose the Larva's Coordinates, followed by an adjacent diagonal cooridinate.\n=========================================================================" << endl << endl;
			} else {
				std::cout << "=========================================================================\nInvalid Move. You must choose a Bird's Coordinates, followed by an adjacent diagonal cooridinate.\nRemember, Birds can only move forward!\n=========================================================================" << endl << endl;
			}
		}

		std::cout << "MOVE: ";

		string move_input = "";

		if(gb->isPlayerOneTurn()) {
			move_input = player_one->makeMove();
		} else {
			move_input = player_two->makeMove();
		}

		// parse the input, trim extra whitespace
		// TRIM HERE
		
		char fromPos[2];
		char toPos[2];
		
		fromPos[0] = move_input[0];
		fromPos[1] = move_input[1];
		toPos[0] = move_input[3];
		toPos[1] = move_input[4];

		printf("Time for Move: %.6fs\n", (gb->isPlayerOneTurn() ? player_one->getMoveTime() : player_two->getMoveTime()));
		
		// make the move, if made, then continue, else display an error message.
		valid_move = gb->movePiece(fromPos, toPos);

		// check to see if an end state has been reached
		winner = gb->isEndState();
	}

	gb->printBoard();

	std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\tGAME OVER\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	std::cout << endl << ((winner == 1) ? "Player 1" : "Player 2") << " Wins\nPress any key to continue.\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	getchar();
}