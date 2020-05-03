#include <stdafx.h>
#include "HILLCL.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

Node::Node() {
	coin1 = coin2 = coin3 = true;
	parent = nullptr;
}

Node::Node(bool coin1_, bool coin2_, bool coin3_) {
	coin1 = coin1_;
	coin2 = coin2_;
	coin3 = coin3_;
	parent = nullptr;
}

Node::Node(Node& node) {
	coin1 = node.coin1;
	coin2 = node.coin2;
	coin3 = node.coin3;
	parent = node.parent;
}

HILLCL::HILLCL()
{
	srand(time(NULL));
}

HILLCL::~HILLCL()
{
	for (Node* node : nodes)
	{
		delete node;
	}
}

void HILLCL::solve(Node& start_state_)
{
	Node* start = new Node(start_state_);
	Node* prev;
	calc_value(start);
	nodes.push_back(start);
	Node* state = start;
	int attempt = 0;

	while (!check_goal(state)) {
		prev = start;
		attempt++;
		for (size_t i = 0; i < 3; i++) {
			state = expand_node(prev);
			nodes.push_back(state);
			calc_value(state);

			if (i != 2 && check_goal(state)) {
				state = start;
				break;
			}

			if (state->value >= prev->value) {
				state->parent = prev;
				prev = state;
			}
		}
	}

	print_solution(state);
	cout << attempt << endl;
}

bool HILLCL::check_goal(Node* node)
{
	if ((node->coin1 && node->coin2 && node->coin3) || (!node->coin1 && !node->coin2 && !node->coin3)) {
		return true;
	}

	return false;
}

Node* HILLCL::expand_node(Node* node)
{
	Node* child = new Node(*node);
	int r = rand() % 3;

	switch (r)
	{
	case 0:
		child->coin1 = !node->coin1;
		child->coin2 = !node->coin2;
		break;
	case 1:
		child->coin3 = !node->coin3;
		child->coin2 = !node->coin2;
		break;
	case 2:
		child->coin1 = !node->coin1;
		child->coin3 = !node->coin3;
		break;
	}

	return child;
}

void HILLCL::calc_value(Node* node)
{
	int value1 = 0;
	int value2 = 0;

	node->coin1 ? value1++ : value2++;
	node->coin2 ? value1++ : value2++;
	node->coin3 ? value1++ : value2++;

	node->value = value1 > value2 ? value1 : value2;
}

void HILLCL::print_solution(Node* node)
{
	Node* parent = node;
	do
	{
		cout << parent->coin1 << " " << parent->coin2 << " " << parent->coin3 << endl;
		parent = parent->parent;
	} while (parent);
}
