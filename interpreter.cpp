#include"interpreter.h"

Interpreter::Interpreter() {
}

void Interpreter::interpret(std::vector<Token> tokens) {
	int pc = 0;
	bool running = true;
	std::string variable = "";
	while(pc < tokens.size() && running) {
		if(stateMachine.state == StateMachine::IDLE) {
			if(tokens[pc].type == "VARIABLE") {
				stateMachine.state = StateMachine::STATEMENT;
				pc++;
				if(tokens[pc].type == "=") {
					variable = tokens[pc-1].value;
					pc++;
					stateMachine.state = StateMachine::EXPRESSION;
					if(tokens[pc].type == "NUMBER") {
						variables.insert_or_assign(variable,tokens[pc].value);
						variable = "";
						stateMachine.state = StateMachine::IDLE;
						pc++;
					}
				}
			} else if(tokens[pc].type == "print") {
					stateMachine.state = StateMachine::STATEMENT;
					pc++;
					if(tokens[pc].type == "(") {
							pc++;
							std::string string_to_print = "";
							if(tokens[pc].type == "NUMBER") {
								//std::cout << tokens[pc].value;
								string_to_print = tokens[pc].value;
							}
							else if(tokens[pc].type == "VARIABLE") {
								//std::cout << variables[tokens[pc]] << std::end;
								string_to_print = variables[tokens[pc].value];
							}
							else {
								std::cerr << "Expected a variable or a number (or a string WIP)" << std::endl;
								running = false;
							}
							pc++;
							if(tokens[pc].type == ")") {
								std::cout << string_to_print << std::endl;
								stateMachine.state = StateMachine::IDLE;
								pc++;
							}
							else {
								std::cerr << "Expected ) after print" << std::endl;
							}
					}else {
						std::cout << "Expected after print (" << std::endl;
						running = false;
					}
			}
		}else {
			running = false;
		}
	}
}
