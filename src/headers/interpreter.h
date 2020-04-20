#pragma once
#include<iostream>
#include<map>
#include<stack>
#include<algorithm>

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

	std::stack<std::vector<Token>> tokens_stack;
	std::stack<int> pc_stack;
	int stack_pointer = 0;

	std::vector<std::string> function_stack;
	int function_stack_pointer = 0;

	void printTokens(std::vector<Token> tokens); // for debugging

	void interpret(std::vector<Token> tokens);
	void interpret_function(Variable function);
	void interpreter_console();
	void changePC(int i);
	void doStatement();
	void doWhile();
	void doIf();
	Variable doIfFunction();
	bool getEndToken();
	void setC_Function(std::string name, int (*func)());

	void setInteger(std::string name, int v);
	void setFloat(std::string name, float v);
	void setBool(std::string name, bool v);
	void setString(std::string name, std::string v);

	int getInteger(std::string name);
	float getFloat(std::string name);
	bool getBool(std::string name);
	std::string getString(std::string name);

	std::string getStackString(int i);
	int getStackInteger(int i);
	float getStackFloat(int i);
	bool getStackBool(int i);

	void pushStackString(std::string v);
	void pushStackInteger(int v);
	void pushStackFloat(float v);
	void pushStackBool(bool v);

	Variable getVariable();
	Variable getExpression();
	Variable doExpression();
	Variable doFunctionToken();
	Variable doFunction(std::string funciton_name);
	Variable doC_Function(std::string funciton_name);

	Token getToken(int pc);

	std::string getVariableName();
	std::string getString();

	bool isOperator(Token t);


	bool running = false;
	int pc = 0;
	int tokens_size;
};
