#pragma once
#include<string>

class Variable {
public:
	enum Type {STRING,NUMBER,BOOL,C_FUNCTION,FUNCTION,ARRAY,TABLE,NIL};

	Variable();
	Variable(Type type, std::string value);

	Type type;
	std::string value;
	int (*function)();
};
