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
	std::map<std::string, std::string> variables;

	void interpret(std::vector<Token> tokens);
};
