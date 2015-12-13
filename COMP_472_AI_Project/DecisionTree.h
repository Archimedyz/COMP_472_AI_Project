#ifndef _DecisionTree_TREE_H_
#define _DecisionTree_TREE_H_

#include <vector>
#include <string>

enum MOVE {
	DOWN_RIGHT,
	DOWN_LEFT,
	UP_RIGHT,
	UP_LEFT
};

struct Node {
	Node * parent;
	std::vector<Node *> children = std::vector<Node*>();
	int height;
	MOVE move;
	int piece;
	int positions[5][2];
	int h_value;
	int nodeNum;
};

class DecisionTree {
public:
	DecisionTree();
	~DecisionTree();
	void resetTree();
	void insertRoot(int positions[5][2]);
	void insertNode(Node * parent, int piece, MOVE move);
	Node * getRoot();
	void printTree();
private:
	int height;
	int nextNodeNum;
	std::vector<std::vector<Node *>> mTree;
};

#endif // _DecisionTree_TREE_H_