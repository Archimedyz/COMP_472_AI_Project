#ifndef _PLAYER_
#define _PLAYER_

#include <string>
#include <chrono>

class Player {
public :
	Player(); // Constructor
	~Player(); // Destructor
	virtual std::string makeMove(); // function used to allow the player to make a move.
	double getMoveTime(); // returns the time take for the previous move in seconds
protected:
	std::chrono::steady_clock::time_point move_start_time; // time just after makeMove() is called
	std::chrono::steady_clock::time_point move_end_time; // time right before entering the move
	std::chrono::duration<double> move_time; // Previous move's time
private:

};

#endif // _Player_