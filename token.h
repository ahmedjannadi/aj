#pragma once

class Token {
public:
	Token(std::string value, std::string type);
	~Token();
	std::string value;
	std::string type;
};
