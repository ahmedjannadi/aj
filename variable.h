#pragma once
#include<string>

class Variable {
public:
	Variable();
	Variable(std::string type, std::string value);
	std::string type;
	std::string value;
};
