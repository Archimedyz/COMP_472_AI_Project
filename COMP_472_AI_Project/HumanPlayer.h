#ifndef _HUMAN_PLAYER_
#define _HUMAN_PLAYER_

#include "Player.h"

class HumanPlayer : public Player{
public:
	HumanPlayer();
	~HumanPlayer();
	std::string makeMove();
private:
	void flushInput();
};

#endif // _HUMAN_PLAYER_