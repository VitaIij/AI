#include "RBFS.h"
#include <iostream>

using namespace std;

State::State()
{
	left = vector<bool>(6, true);
	right = vector<bool>(6, false);
	boat_left = true;
	parent = nullptr;
	f = F = 0;
}

State::State(std::vector<bool> left_, std::vector<bool> right_, bool boat_left_)
{
	left = left_;
	right = right_;
	boat_left = boat_left_;
	parent = nullptr;
	f = F = 0;
}

State::State(State& state)
{
	left = state.left;
	right = state.right;
	boat_left = state.boat_left;
	parent = state.parent;
	children = state.children;
	f = state.f;
	F = state.F;
}

RBFS::RBFS()
{
	start = new State();
	vector<bool> left = vector<bool>(6, false);
	vector<bool> right = vector<bool>(6, true);
	bool boat_left = false;
	end = new State(left, right, boat_left);
}

RBFS::~RBFS()
{
	for (State* state : states)
	{
		delete state;
	}
	delete end;
}

void RBFS::solve()
{
	calc_f(start);
	states.push_back(start);
	int result = rbfs(start, INT_MAX);

	if (result == INT_MAX)
	{
		cout << "Cant find solution";
	}
}

int RBFS::rbfs(State* state, int flimit)
{
	if (check_goal(state))
	{
		print_solution(state);
		return -1;
	}

	calc_next_steps(state);
	if (state->children.size() == 0)
	{
		return INT_MAX;
	}

	for (size_t i = 0; i < state->children.size(); i++) {
		if (state->children[i]->f < state->F) {
			state->children[i]->F = state->F > state->children[i]->f ? state->F : state->children[i]->f;
		}
		else {
			state->children[i]->F = state->children[i]->f;
		}
	}

	State* best = nullptr, * alt = nullptr;
	calc_best_alt(state, &best, &alt);

	int new_flimit;
	while (best->F <= flimit && best->F < INT_MAX)
	{
		new_flimit = alt ? (alt->F < flimit ? alt->F : flimit) : flimit;
		best->F = rbfs(best, new_flimit);
		if (best->F == -1)
		{
			return -1;
		}
		calc_best_alt(state, &best, &alt);
	}
	return best->F;
}

bool RBFS::check_goal(State* state)
{
	return states_equal(state, end);
}

bool RBFS::states_equal(State* first, State* second)
{
	if (first->boat_left != second->boat_left) {
		return false;
	}

	for (size_t i = 0; i < 6; i++)
	{
		if (first->left[i] != second->left[i] || first->right[i] != second->right[i]) {
			return false;
		}
	}
	return true;
}

bool RBFS::check_repeats(State* new_state)
{
	for (size_t i = 0; i < states.size(); i++) {
		if (states_equal(states[i], new_state)) {
			return true;
		}
	}

	return false;
}

void RBFS::calc_next_steps(State* state)
{
	if (state->boat_left) {
		for (size_t i = 0; i < 6; i++) {
			move_right(state, i, -1);
			for (size_t j = i + 1; j < 6; j++) {
				move_right(state, i, j);
			}
		}
	}
	else {
		for (size_t i = 0; i < 6; i++) {
			move_left(state, i, -1);
			for (size_t j = i + 1; j < 6; j++) {
				move_left(state, i, j);
			}
		}
	}
}

bool RBFS::check_state(State* state)
{
	for (size_t i = 0; i < 3; i++) {
		if (state->left[i]) {
			if (state->left[i + 3]) continue;
			else {
				for (size_t j = 0; j < 3; j++)
				{
					if (j != i && state->left[j + 3]) {
						return false;
					}
				}
			}
		}
	}
	for (size_t i = 0; i < 3; i++) {
		if (state->right[i]) {
			if (state->right[i + 3]) continue;
			else {
				for (size_t j = 0; j < 3; j++)
				{
					if (j != i && state->right[j + 3]) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

void RBFS::move_right(State* state, int first, int second)
{
	if (!can_move_right(state, first, second) || !(state->boat_left)) {
		return;
	}

	State* new_state = new State(*state);

	new_state->boat_left = false;
	if (first != -1) {
		new_state->left[first] = false;
		new_state->right[first] = true;
	}
	if (second != -1) {
		new_state->left[second] = false;
		new_state->right[second] = true;
	}

	if (check_repeats(new_state) || !check_state(new_state)) {
		delete new_state;
		return;
	}

	new_state->parent = state;
	new_state->children = vector<State*>(0);
	state->children.push_back(new_state);
	states.push_back(new_state);
	calc_f(new_state);
}

void RBFS::move_left(State* state, int first, int second)
{
	if (!can_move_left(state, first, second) || state->boat_left) {
		return;
	}

	State* new_state = new State(*state);

	new_state->boat_left = true;
	if (first != -1) {
		new_state->left[first] = true;
		new_state->right[first] = false;
	}
	if (second != -1) {
		new_state->left[second] = true;
		new_state->right[second] = false;
	}

	if (check_repeats(new_state) || !check_state(new_state)) {
		delete new_state;
		return;
	}

	new_state->parent = state;
	new_state->children = vector<State*>(0);
	state->children.push_back(new_state);
	states.push_back(new_state);
	calc_f(new_state);
}

bool RBFS::can_move_right(State* state, int first, int second)
{
	if (first == -1 && second != -1 && state->left[second]) {
		return true;
	}
	else if (second == -1 && first != -1 && state->left[first]) {
		return true;
	}
	else if (first > 2 && second > 2 && state->left[first] && state->left[second]) {
		return true;
	}
	else if (first < 3 && second < 3 && first >=0 && second >= 0 && state->left[first] && state->left[second]) {
		return true;
	}
	else if (first + 3 == second && state->left[first] && state->left[second]) {
		return true;
	}
	return false;
}

bool RBFS::can_move_left(State* state, int first, int second)
{
	if (first == -1 && second != -1 && state->right[second]) {
		return true;
	}
	else if (second == -1 && first != -1 && state->right[first]) {
		return true;
	}
	else if (first > 2 && second > 2 && state->right[first] && state->right[second]) {
		return true;
	}
	else if (first < 3 && second < 3 && first >= 0 && second >= 0 && state->right[first] && state->right[second]) {
		return true;
	}
	else if (first + 3 == second && state->right[first] && state->right[second]) {
		return true;
	}
	return false;
}

void RBFS::calc_f(State* state)
{
	int step = 0;
	int value = 0;

	State* parent = state;
	while (true)
	{
		parent = parent->parent;
		if (!parent) {
			break;
		}
		step++;
	}

	for (size_t i = 0; i < 6; i++) {
		if (state->left[i]) {
			value++;
		}
	}

	state->f = state->F = value + step;
}

void RBFS::calc_best_alt(State* node, State** best, State** alt)
{
	*best = *alt = nullptr;
	if (node->children.size() == 1)
	{
		*best = node->children[0];
	}
	else
	{
		*best = node->children[0];
		*alt = node->children[1];
		for (int i = 1; i < node->children.size(); i++)
		{
			if (node->children[i]->F <= (*best)->F)
			{
				*alt = *best;
				*best = node->children[i];
			}
			else if (node->children[i]->F <= (*alt)->F)
			{
				*alt = node->children[i];
			}
		}
	}
}

void RBFS::print_solution(State* state)
{
	do
	{
		for (int i = 0; i < 3; i++) {
			cout << " " << state->left[i];
		}
		cout << (state->boat_left ? " 0" : "  ") << " " << (state->boat_left ? " " : "0");
		for (int i = 0; i < 3; i++) {
			cout << " " << state->right[i];
		}
		cout << "\n";
		for (int i = 0; i < 3; i++) {
			cout << " " << state->left[i + 3];
		}
		cout << "    ";
		for (int i = 0; i < 3; i++) {
			cout << " " << state->right[i + 3];
		}

		cout << "\n\n";
		state = state->parent;
	} while (state);
}
