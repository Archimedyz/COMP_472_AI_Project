#include "DecisionTree.h"
#include <iostream>
#include <vector>

DecisionTree::DecisionTree() {
	height = -1;
	nextNodeNum = 0;
	mTree = std::vector<std::vector<Node *>>();
}

DecisionTree::~DecisionTree() {

}

void DecisionTree::resetTree() {
	mTree.clear();
	height = -1;
	nextNodeNum = 0;
}

void DecisionTree::insertRoot(int positions[5][2]) {
	if(height != -1) {
		return;
	}

	Node * root = new Node;
	root->parent = nullptr;
	root->height = 0;
	for(int i = 0; i < 5; ++i) {
		memcpy(root->positions[i], positions[i], 2 * sizeof(int));	
	}

	mTree.push_back(std::vector<Node *>());
	height = 0;
	root->nodeNum = nextNodeNum++;
	mTree[0].push_back(root);
}

void DecisionTree::insertNode(Node * parent, int piece, MOVE move) {
	if(height == -1) {
		return;
	}

	Node * newNode = new Node;
	newNode->parent = parent;
	//std::cout << "from parent: " << parent->nodeNum << "from child: " << newNode->parent->nodeNum << "\n";
	parent->children.push_back(newNode);
	newNode->height = parent->height + 1;
	// if this is the first node of this height, insert the row
	if(height < newNode->height) {
		mTree.push_back(std::vector<Node *>());
		++height;
	}
	// save the move information
	newNode->move = move;
	newNode->piece = piece;
	// determine the new positions after the move. The Player AI class will ensure that the move is valid.
	for(int i = 0; i < 5; ++i) {
		if(i == piece) { // this is the piece to move
			if(move == UP_RIGHT) {
				newNode->positions[i][0] = parent->positions[i][0] - 1;
				newNode->positions[i][1] = parent->positions[i][1] + 1;
			} else if(move == DOWN_RIGHT) {		
				newNode->positions[i][0] = parent->positions[i][0] + 1;
				newNode->positions[i][1] = parent->positions[i][1] + 1;
			} else if(move == DOWN_LEFT) {		
				newNode->positions[i][0] = parent->positions[i][0] + 1;
				newNode->positions[i][1] = parent->positions[i][1] - 1;
			} else if(move == UP_LEFT) {		
				newNode->positions[i][0] = parent->positions[i][0] - 1;
				newNode->positions[i][1] = parent->positions[i][1] - 1;
			}
		} else { // other pieces remain the same
			memcpy(newNode->positions[i], parent->positions[i], 2 * sizeof(int));
		}
	}

	newNode->nodeNum = nextNodeNum++;

	mTree[newNode->height].push_back(newNode);
}

Node * DecisionTree::getRoot() {
	return mTree[0][0];
}

void DecisionTree::printTree() {
	std::cout << "\n------------------------------\n"
		<< "\tTREE\n"
		<< "------------------------------\n";
	for(int i = 0; i < mTree.size(); ++i) {
		std::cout << "\nHeight: " << i << "\n";
		for(int j = 0; j < mTree[i].size(); ++j) {
			Node * n = mTree[i][j];
			std::cout << "\t<" << ((i == 0) ? "null" : std::to_string(n->parent->nodeNum));
			std::cout << ", " << n->nodeNum;
			std::cout  << ", " << n->h_value << ">\n";
		}
	}
}