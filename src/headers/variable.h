#pragma once
#include<string>
#include<vector>
#include"token.h"

class Variable {
public:
	enum Type {STRING,NUMBER,BOOL,C_FUNCTION,FUNCTION,ARRAY,TABLE,NIL};

	Variable();
	Variable(Type type, std::string value);

	Type type;
	std::string value;
	int (*function)();
	std::vector<Token> function_body;
};
