#include "Player.h"

#ifndef _HUMAN_PLAYER_
#define _HUMAN_PLAYER_

class HumanPlayer : public Player{
public:
	HumanPlayer();
	~HumanPlayer();
	std::string makeMove();
private:
	void flushInput();
};

#endif // _HUMAN_PLAYER_