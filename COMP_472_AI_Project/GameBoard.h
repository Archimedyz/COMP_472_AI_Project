#ifndef _GAMEBOARD_
#define _GAMEBOARD_

class GameBoard {
public:
	typedef enum {P_ONE_TURN, P_TWO_TURN} TurnType;
	GameBoard();
	~GameBoard();
	void initBoard();
	void printBoard();
	void getBoard();
	bool movePiece(char fromPos[2], char toPos[2]);
	bool isPlayerOneTurn();
	int isEndState();
	void getPositionArray(int positions[5][2]);
private: 
	int larva_pos[2];	// used to keep track of the larva position for quicker end state checking.
	int bird_pos[4][2];	// used to keep track of the position of each bird for quicker end state checking.
	char board[8][8];
	TurnType curr_turn;
	bool validatePosition(int i, int j);
};

#endif // _GAMEBOARD_