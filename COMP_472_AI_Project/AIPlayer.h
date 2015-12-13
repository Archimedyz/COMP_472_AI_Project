#include "Player.h"
#include "DecisionTree.h"

#ifndef _AI_PLAYER_
#define _AI_PLAYER_

class AIPlayer : public Player{
public:
	AIPlayer(GameBoard * gb);
	~AIPlayer();
	std::string makeMove();
	int heuristicFunction(const int positions[5][2]);
private:
	DecisionTree dt;
	GameBoard * gb;
	const int maxDepth = 7;
	bool validateMove(const int positions[5][2], int piece,  MOVE move);
	void buildDecisionTree(Node * node);
	// these variables are set when building the decision tree, to eliminate searching later
	MOVE nextMove; 
	int pieceToMove;
	// weights of each feature for the heuristic.
	const int depthMultiplier = 10;
	const int movesMultiplier = 1;
	const int spaceMultiplier = 1;
};

#endif // _AI_PLAYER_