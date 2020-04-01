#include"interpreter.h"

Interpreter::Interpreter() {
}

void Interpreter::changePC(int i) {
	pc = i;
}

Token Interpreter::getToken(int pc) {
	if(pc>=0 && pc < tokens.size()) {
		return tokens[pc];
	}

	return Token("NULL","NULL");
}

void Interpreter::interpret(std::vector<Token> t) {
	tokens = t;
	std::string variable = "";

	pc = 0;
	running = true;

	while(pc < tokens.size() && running) {
		if(stateMachine.state == StateMachine::IDLE) {
			if(getToken(pc).type == "VARIABLE") {
					stateMachine.state = StateMachine::STATEMENT;
			}
			else if(getToken(pc).type == "PRINT") {
				stateMachine.state = StateMachine::PRINT;
			}
			else {
				std::cerr << "Error" << std::endl;
			}
		}
		if(stateMachine.state == StateMachine::STATEMENT) {
			if(tokens[pc].type == "VARIABLE") {
				pc++;
				if(tokens[pc].type == "=") {
					variable = tokens[pc-1].value;
					pc++;
					if(tokens[pc].type == "NUMBER") {
						variables.insert_or_assign(variable,tokens[pc].value);
						variable = "";
						stateMachine.state = StateMachine::IDLE;
						changePC(pc + 1);
					}
				} else {
					std::cerr << "wrong syntax -> " + tokens[pc-1].value << std::endl;
					running = false;
				}
			} else if(tokens[pc].type == "print") {
					stateMachine.state = StateMachine::STATEMENT;
					changePC(pc + 1);
					if(tokens[pc].type == "(") {
							changePC(pc + 1);
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
							changePC(pc + 1);
							if(tokens[pc].type == ")") {
								std::cout << string_to_print << std::endl;
								stateMachine.state = StateMachine::IDLE;
								changePC(pc + 1);
							}
							else {
								std::cerr << "Expected ) after print" << std::endl;
								running = false;
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

void Interpreter::interpreter_console() {
	std::cout << "AJ 0.1.0 Copyright(C) Ahmed Jannadi 2020" << std::endl;
	std::string s;
	while(true) {
		std::cout << ">" ;
		std::getline(std::cin, s);
		interpret(tokenizer.getTokens(s));
	}
}
