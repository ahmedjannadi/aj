#pragma once
#include<string>

class Variable {
public:
	enum Type {STRING,NUMBER,BOOL,NIL};

	Variable();
	Variable(Type type, std::string value);

	Type type;
	std::string value;
};
