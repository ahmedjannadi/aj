#include<iostream>
#include<string>

#include"interpreter.h"

Interpreter interpreter;

int main() {
	std::string test = "a = 5 b=5";
	auto tokens = interpreter.tokenizer.getTokens(test);

	//interpreter.interpret(tokens);
	interpreter.interpreter_console();

}
