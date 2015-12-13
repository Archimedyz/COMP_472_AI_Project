#include "AIPlayer.h"
#include <iostream>

AIPlayer::AIPlayer(GameBoard * gb) {
	dt = DecisionTree();
	this->gb = gb;
}

AIPlayer::~AIPlayer() {

}

std::string AIPlayer::makeMove() {

	// start the move timer
	move_start_time = std::chrono::steady_clock::now();

	int positions[5][2];
	gb->getPositionArray(positions);
	// reset state space tree
	dt.resetTree();
	// initialize the root level of the tree
	dt.insertRoot(positions);
	Node * rootNode = dt.getRoot();
	
	// now perform the state space search, by generating in a depth first fashion and then determining the h(n) values as we go along
	buildDecisionTree(rootNode);

	// uncomment the following line to view the decision tree in the format <parentNum, nodeNum, h_value>. Reduces decision time exponentially
	//dt.printTree();

	// determine the final 'input'
	char ai_input[6];
	ai_input[0] = (positions[pieceToMove][1] + 'A'); // D
	ai_input[1] = ('8' - positions[pieceToMove][0]); // 2
	ai_input[2] = ' ';
	if(nextMove == UP_RIGHT) { // E3
		ai_input[3] = ai_input[0]  + 1;
		ai_input[4] = ai_input[1]  + 1;
	} else if(nextMove == DOWN_RIGHT) {	// E1
		ai_input[3] = ai_input[0]  + 1;
		ai_input[4] = ai_input[1]  - 1;
	} else if(nextMove == DOWN_LEFT) {	// C1	
		ai_input[3] = ai_input[0] - 1;
		ai_input[4] = ai_input[1] - 1;
	} else if(nextMove == UP_LEFT) { // C3
		ai_input[3] = ai_input[0] - 1;
		ai_input[4] = ai_input[1] + 1;
	}
	ai_input[5] = 0;

	// record the end time.
	move_end_time = std::chrono::steady_clock::now();
	move_time = std::chrono::duration_cast<std::chrono::duration<double>>(move_end_time - move_start_time);

	std::cout << ai_input << " : h(n) = " << rootNode->h_value << "\n";

	return ai_input;
}

int AIPlayer::heuristicFunction(const int positions[5][2]) {
	
	// determine how far the larva is from the goal. don't allow for zero, might mess with calculations later
	int larvaDepth = 1 + positions[0][0];
	//if at the goal, return the highest possible value.
	if(larvaDepth == 8) {
		return 10000;
	}

	// otherwise check how many move the larva has to go.
	int availableMoves = 0;
	for(int i = 0; i < 4; ++i) {
			if(validateMove(positions, 0, MOVE(i))) {
				++availableMoves;
			}
	}

	// there are no moves for the larva, then return the lowest value, 0
	if(availableMoves == 0) {
		return 0;
	}

	// determine how hamming distance of all birds to larva combined. 
	int spaceFromBirds = 0;
	for(int i = 0; i < 4; ++i) {
		spaceFromBirds += abs(positions[i][0] - positions[0][0]);
		spaceFromBirds += abs(positions[i][1] - positions[0][1]);
	}
	// due the diagonal movement, the values are alsways even and thus divides by two
	spaceFromBirds /= 2;

	int h_value = 0;

	h_value = depthMultiplier * larvaDepth + movesMultiplier * availableMoves + spaceMultiplier * spaceFromBirds;

	return h_value;

}

// does not consider if the piece is a larva, that duty is for the calling function.
bool AIPlayer::validateMove(const int positions[5][2], int piece, MOVE move) {

	int newPos[2]; 
	if(move == UP_RIGHT) {
		newPos[0] = positions[piece][0] - 1;
		newPos[1] = positions[piece][1] + 1;
	} else if(move == DOWN_RIGHT) {		
		newPos[0] = positions[piece][0] + 1;
		newPos[1] = positions[piece][1] + 1;
	} else if(move == DOWN_LEFT) {		
		newPos[0] = positions[piece][0] + 1;
		newPos[1] = positions[piece][1] - 1;
	} else if(move == UP_LEFT) {		
		newPos[0] = positions[piece][0] - 1;
		newPos[1] = positions[piece][1] - 1;
	}

	// ensure the new position is within the bounds
	if(newPos[0] < 0 || newPos[0] > 7 || newPos[1] < 0 || newPos[1] > 7) {
		return false;
	}

	// make sure the new position is not on top of an existing piece
	for(int i = 0; i < 5; ++i) {
		if(i != piece) { 
			if(newPos[0] == positions[i][0] && newPos[1] == positions[i][1]) {
				return false;
			}
		} 
	}

	// if everything above checked out, its a valid move
	return true;
}

void AIPlayer::buildDecisionTree(Node * node) {
	/* 
	 * if the larva has reached the lowest possible position there is no next move
	 * the larva wins, so run the heuristic fundtion to get the highest value.
	 * otherwise, if the node is a leaf, just get the heuristic value.
	 */
	if(node->height == maxDepth || node->positions[0][0] == 7) {
		node->h_value = heuristicFunction(node->positions);
		return;
	}
	
	
	// all other situations, keep generating nodes
	bool h_value_assigned = false; // check to see if any children passed up their h(n). if not, must be a leaf, so evaluate h(n) here.
	bool is_larva_move = (gb->isPlayerOneTurn() && node->height % 2 == 0) || (!gb->isPlayerOneTurn() && node->height % 2 != 0);
	bool got_h_value_from_child = false;

	if(is_larva_move) { // generate 4 moves for larva, and take the maximum h_value from children
		for(int i = 0; i < 4; ++i) {
			if(validateMove(node->positions, 0, MOVE(i))) { // if the move is valid, generate a node for it and go to it
				dt.insertNode(node, 0, MOVE(i));
				buildDecisionTree(node->children[node->children.size()-1]);
				// the node we just looked at should now hav a heuristic value, so simply compare it to the current value
				// if this is the first value, then simply take it.
				if(got_h_value_from_child) { //compare
					if(node->children[node->children.size()-1]->h_value > node->h_value) {
						node->h_value = node->children[node->children.size()-1]->h_value;
						if(node->height == 0) { // if the node is the root, we want to keep track of what the move is.
							nextMove = MOVE(i);
							pieceToMove = 0;
						}
					}
				} else { // take
					node->h_value = node->children[node->children.size()-1]->h_value;
					if(node->height == 0) { // if the node is the root, we want to keep track of what the move is.
						nextMove = MOVE(i);
						pieceToMove = 0;
					}
					got_h_value_from_child = true;
				}
				h_value_assigned = true;
			}
		}
	} else { // generate 2 moves for each bird, and take the minimum h_value from children 
		for(int i = 0; i < 4; ++i) {
			if(validateMove(node->positions, 1+i, MOVE(UP_LEFT))) { // if a bird can move up-left
				dt.insertNode(node, 1+i, MOVE(UP_LEFT));
				buildDecisionTree(node->children[node->children.size()-1]);
				// the node we just looked at should now hav a heuristic value, so simply compare it to the current value
				// if this is the first value, then simply take it.
				if(got_h_value_from_child) { //compare
					if(node->children[node->children.size()-1]->h_value < node->h_value) {
						node->h_value = node->children[node->children.size()-1]->h_value;
						if(node->height == 0) { // if the node is the root, we want to keep track of what the move is.
							nextMove = UP_LEFT;
							pieceToMove = 1+i;
						}
					}
				} else { // take
					node->h_value = node->children[node->children.size()-1]->h_value;
					if(node->height == 0) { // if the node is the root, we want to keep track of what the move is.
						nextMove = UP_LEFT;
						pieceToMove = 1+i;
					}
					got_h_value_from_child = true;
				}
				h_value_assigned = true;
			}
			if(validateMove(node->positions, 1+i, MOVE(UP_RIGHT))) { // if a bird can move up-right
				dt.insertNode(node, 1+i, MOVE(UP_RIGHT));
				buildDecisionTree(node->children[node->children.size()-1]);
				// the node we just looked at should now hav a heuristic value, so simply compare it to the current value
				// if this is the first value, then simply take it.
				if(got_h_value_from_child) { //compare
					if(node->children[node->children.size()-1]->h_value < node->h_value) {
						node->h_value = node->children[node->children.size()-1]->h_value;
						if(node->height == 0) { // if the node is the root, we want to keep track of what the move is.
							nextMove = UP_RIGHT;
							pieceToMove = 1+i;
						}
					}
				} else { // take
					node->h_value = node->children[node->children.size()-1]->h_value;
					if(node->height == 0) { // if the node is the root, we want to keep track of what the move is.
						nextMove = UP_RIGHT;
						pieceToMove = 1+i;
					}
					got_h_value_from_child = true;
				}
				h_value_assigned = true;
			}
		}
	}

	// at this point, if any children were generated, this node should have a heuristic value, if not, get it for the current state
	if(!h_value_assigned) {
		node->h_value = heuristicFunction(node->positions);
	}
}