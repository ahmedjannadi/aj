#include<iostream>
#include<string>

#include"interpreter.h"

Interpreter interpreter;

int main() {
	std::string test = "a = 5 b=5";
	auto tokens = interpreter.tokenizer.getTokens(test);

	//interpreter.interpret(tokens);

	std::cout << "AJ Interpreter version 0.1" << std::endl;
	std::string s;
	while(true) {
		std::cout << ">" ;
		std::getline(std::cin, s);
		interpreter.interpret(interpreter.tokenizer.getTokens(s));
	}
	return 0;
}
