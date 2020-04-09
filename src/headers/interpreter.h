#pragma once
#include<iostream>
#include<map>
#include<stack>

#include"tokenizer.h"
#include"statemachine.h"
#include"variable.h"

class Interpreter {
public:
	Interpreter();
	Tokenizer tokenizer;
	StateMachine stateMachine;

	std::vector<Token> tokens;
	std::map<std::string, int (*)> c_functions;
	std::map<std::string, Variable> variables;
	std::vector<Variable> stack;

	void interpret(std::vector<Token> tokens);
	void interpreter_console();
	void changePC(int i);
	void doStatement();
	void doWhile();
	void doIf();
	void getEndToken();
	void setC_Function(std::string name, int (*func)());

	Variable getVariable();
	Variable getExpression();
	Variable doExpression();

	Token getToken(int pc);

	std::string getVariableName();
	std::string getString();

	bool isOperator(Token t);


	bool running = false;
	int pc = 0;
	int tokens_size;
};
