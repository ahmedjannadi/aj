#pragma once

#include<string>
#include<vector>

#include"token.h"

class Tokenizer {
public:
	Tokenizer();
	std::vector<Token> getTokens(std::string);
private:
	std::vector<std::string> reserved_tokens;
	bool checkReserved(std::string token);
	bool checkChar(char c,std::string str);
	bool isLetter(char c);
	bool isDigit(char c);
	bool isSymbol(char c);
};
