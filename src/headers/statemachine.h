#pragma once

#include<string>
#include<vector>

class StateMachine {
public:
	StateMachine();
	enum State {IDLE, STATEMENT, EXPRESSION, FUNCTION_ASSIGNEMENT, CONDITION, IF, ELSE, WHILE, PRINT};
	State state;
};
