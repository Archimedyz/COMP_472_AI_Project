#include "Player.h"

#ifndef _AI_PLAYER_
#define _AI_PLAYER_

class AIPlayer : public Player{
public:
	AIPlayer();
	~AIPlayer();
	std::string makeMove();
private:

};

#endif // _HUMAN_PLAYER_