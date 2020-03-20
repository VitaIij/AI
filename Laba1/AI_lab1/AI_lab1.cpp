#include <iostream>
#include <queue>
#include <stack>
using namespace std;

const int bucket1Capacity = 5;
const int bucket2Capacity = 7;
const int sourceCapacity = 12;

struct State
{
	int bucket1;
	int bucket2;
	int source;
	State* parent;	//	pointer to previous state. It is needed to build tree of states

	State(int bucket1, int bucket2, int source = 0) {
		this->bucket1 = bucket1;
		this->bucket2 = bucket2;
		this->source = source;
		parent = NULL;
	}
};

vector<State*> states;

bool solve();
State* createState(int, int, int);
bool checkState(int, int);
void printResult();

int main()
{
	bool solved = solve();

	if (solved) {
		printResult();
	}
	else {
		cout << "Can not find solution";
	}
    
}

bool solve() {
	State *initial = new State(0, 0, sourceCapacity);
	queue<State*> q;
	q.push(initial);
	states.push_back(initial);

	while (!q.empty()) {
		State *current = q.front();
		q.pop();

		while (true) {
			State *newState = createState(current->bucket1, current->bucket2, current->source);
			if (newState->bucket1 == -1) {
				break;
			}

			newState->parent = current;
			states.push_back(newState);
			if (newState->bucket1 == 0 && newState->bucket2 == 6) {
				return true;
			}
			q.push(newState);
		}
	}

	return false;
}

// create new state of buckets
State* createState(int bucket1, int bucket2, int source) {
	int newbucket1 = 0;
	int newbucket2 = 0;
	int newSource = source;
	bool isUnique;
	State *newState = new State(0, 0);

	if (bucket1 != bucket1Capacity && source != 0) {	// pour from source to first bucket
		int addToBucket1 = bucket1Capacity - bucket1;
		newbucket1 = addToBucket1 <= source ? (bucket1 + addToBucket1) : source;
		newbucket2 = bucket2;
		newSource = addToBucket1 <= source ? source - addToBucket1 : 0;
		isUnique = checkState(newbucket1, newbucket2);
		if (isUnique) {
			newState->bucket1 = newbucket1;
			newState->bucket2 = newbucket2;
			newState->source = newSource;
			return newState;
		}
		else {
			newbucket1 = 0;
			newbucket2 = 0;
			newSource = source;
		}
	}
	if (bucket2 != bucket2Capacity && source != 0) {	// pour from source to second bucket
		int addToBucket2 = bucket2Capacity - bucket2;
		newbucket1 = bucket1;
		newbucket2 = addToBucket2 <= source ? (bucket2 + addToBucket2) : source;
		newSource = addToBucket2 <= source ? source - addToBucket2 : 0;
		isUnique = checkState(newbucket1, newbucket2);
		if (isUnique) {
			newState->bucket1 = newbucket1;
			newState->bucket2 = newbucket2;
			newState->source = newSource;
			return newState;
		}
		else {
			newbucket1 = 0;
			newbucket2 = 0;
			newSource = source;
		}
	}
	if (bucket1 != bucket1Capacity && bucket2 != 0) {	// pour from second bucket to first
		int addTobucket1 = bucket1Capacity - bucket1;
		newbucket1 = bucket1 + (bucket2 <= addTobucket1 ? bucket2 : addTobucket1);
		newbucket2 = bucket2 <= addTobucket1 ? 0 : bucket2 - addTobucket1;
		isUnique = checkState(newbucket1, newbucket2);
		if (isUnique) {
			newState->bucket1 = newbucket1;
			newState->bucket2 = newbucket2;
			newState->source = source;
			return newState;
		}
		else {
			newbucket1 = 0;
			newbucket2 = 0;
			newSource = source;
		}
	}
	if (bucket2 != bucket2Capacity && bucket1 != 0) {	// pour from first bucket to second
		int addTobucket2 = bucket2Capacity - bucket2;
		newbucket2 = bucket2 + (bucket1 <= addTobucket2 ? bucket1 : addTobucket2);
		newbucket1 = bucket1 <= addTobucket2 ? 0 : bucket1 - addTobucket2;
		isUnique = checkState(newbucket1, newbucket2);
		if (isUnique) {
			newState->bucket1 = newbucket1;
			newState->bucket2 = newbucket2;
			newState->source = source;
			return newState;
		}
		else {
			newbucket1 = 0;
			newbucket2 = 0;
			newSource = source;
		}
	}
	if (bucket1 != 0) {		// make first bucket empty
		newbucket1 = 0;
		newbucket2 = bucket2;
		newSource = source + bucket1;
		isUnique = checkState(newbucket1, newbucket2);
		if (isUnique) {
			newState->bucket1 = newbucket1;
			newState->bucket2 = newbucket2;
			newState->source = newSource;
			return newState;
		}
		else {
			newbucket1 = 0;
			newbucket2 = 0;
			newSource = source;
		}
	}
	if (bucket2 != 0) {		// make second bucket empty
		newbucket1 = bucket1;
		newbucket2 = 0;
		newSource = source + bucket2;
		isUnique = checkState(newbucket1, newbucket2);
		if (isUnique) {
			newState->bucket1 = newbucket1;
			newState->bucket2 = newbucket2;
			newState->source = newSource;
			return newState;
		}
		else {
			newbucket1 = 0;
			newbucket2 = 0;
			newSource = source;
		}
	}

	return new State(-1, -1);
}

// check if current state of buckets is not copy of existing states
bool checkState(int bucket1, int bucket2) {
	bool isUnique = true;

	for (int i = 0; i < states.size(); i++)	{
		State *state = states[i];
		if (state->bucket1 == bucket1 && state->bucket2 == bucket2) {
			isUnique = false;
			break;
		}
	}

	return isUnique;
}

void printResult() {
	State *state = states.back();
	stack<State*> s;
	while (true) {
		s.push(state);
		if (!state->parent) {
			break;
		}
		state = state->parent;
	}
	cout << "bucket " << bucket1Capacity << " bucket "<< bucket2Capacity << " bucket " << sourceCapacity << endl;
	while (!s.empty()) {
		state = s.top();
		s.pop();
		cout << state->bucket1 << " " << state->bucket2 << " " << state->source << endl;
	}
}