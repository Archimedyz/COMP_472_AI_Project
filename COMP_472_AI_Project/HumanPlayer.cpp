#include <iostream>
#include <regex>
#include <Exception>
#include "HumanPlayer.h"

HumanPlayer::HumanPlayer() {

}

HumanPlayer::~HumanPlayer() {

}

std::string HumanPlayer::makeMove() {
	char input[6];
	std::regex input_format("^[A-Z][0-9] [A-Z][0-9]$");
	move_start_time = std::chrono::steady_clock::now();
	while(1) {
		std::cin.get(input, 6);
		flushInput();
		for(int i = 0; i < 5; ++i) {
			input[i] = toupper(input[i]);
		}
		if(!std::regex_match(input, input_format)) {
			std::cout << "=========================================================================\nInvalid input.\nPlease enter the coordinates of the piece, and the coordinates of the destination separated by a space.\n(Example: A1 B2)\nAny extra characters after the fifth are discarded.\n=========================================================================\nMOVE: ";
			continue;
		}
		break;
	}
	move_end_time = std::chrono::steady_clock::now();
	move_time = std::chrono::duration_cast<std::chrono::duration<double>>(move_end_time - move_start_time);
	return input;
}


void HumanPlayer::flushInput() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}