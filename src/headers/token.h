#pragma once

#include<string>

class Token {
public:
	Token(std::string value, std::string type);
	std::string value;
	std::string type;
};
