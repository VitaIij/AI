#pragma once
#include <vector>

struct State
{
	std::vector<bool> left;
	std::vector<bool> right;
	bool boat_left;
	State* parent;
	std::vector<State*> children;
	int f;
	int F;

	State();
	State(std::vector<bool> left, std::vector<bool> right, bool boat_left);
	State(State&);
};

class RBFS
{
public:
	RBFS();
	~RBFS();
	void solve();
private:
	State* start;
	State* end;
	std::vector<State*> states;
	int rbfs(State* state, int flimit);
	bool states_equal(State* first, State* second);
	bool check_goal(State* state);
	bool check_repeats(State* state);
	void calc_next_steps(State* state);
	bool check_state(State* state);
	void move_right(State* state, int first, int second);
	void move_left(State* state, int first, int second);
	bool can_move_right(State* state, int first, int second);
	bool can_move_left(State* state, int first, int second);
	void calc_f(State* node);
	void calc_best_alt(State* node, State** best, State** alt);
	void print_solution(State* node);
};

