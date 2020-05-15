#pragma once
#include<string>
#include<vector>
#include<map>
#include"token.h"

class Variable {
public:
	enum Type {STRING,NUMBER,BOOL,C_FUNCTION,FUNCTION,ARRAY,TABLE,NIL};

	Variable();
	Variable(Type type, std::string value);

	Type type;
	std::string value;

	// for C_FUNCTION
	int (*function)();

	// for FUNCTION
	std::vector<Token> function_body;
	std::vector<std::string> function_variable_names;
	std::vector<Variable> function_variables;
	std::map<std::string, Variable> function_params;

	// for ARRAY
	std::vector<Variable> array_values;
};
