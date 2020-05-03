#pragma once
#include <vector>
#include <ctime>

class Node {
public:
	Node();
	Node(bool, bool, bool);
	Node(Node& node);
	bool coin1;
	bool coin2;
	bool coin3;
	int value;
	Node* parent;
};

class HILLCL
{
public:
	HILLCL();
	~HILLCL();
	void solve(Node& start_node);
private:
	std::vector<Node*> nodes;
	bool check_goal(Node* node);
	Node* expand_node(Node* node);
	void calc_value(Node* node);
	void print_solution(Node* node);

};

