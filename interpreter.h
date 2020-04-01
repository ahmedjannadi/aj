#pragma once
#include<iostream>
#include<map>

#include"tokenizer.h"
#include"statemachine.h"

class Interpreter {
public:
	Interpreter();
	Tokenizer tokenizer;
	StateMachine stateMachine;

	std::vector<Token> tokens;
	std::map<std::string, std::string> variables;

	void interpret(std::vector<Token> tokens);
	void interpreter_console();
	void changePC(int i);
	Token getToken(int pc);

	bool running = false;
	int pc = 0;
	int tokens_size;
};
