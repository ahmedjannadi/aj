#include<iostream>
#include<string>

#include"interpreter.h"

Interpreter interpreter;

int main(int argc, char** argv) {
	/*std::string test = "a = 5 b=5";
	auto tokens = interpreter.tokenizer.getTokens(test);
	interpreter.interpret(tokens);*/
	
	if(argc < 2) {
		interpreter.interpreter_console();
	} else {
		std::string arg1 = argv[1];
		std::string arg2 = argv[2];
		if(arg1.compare("-i") == 0) {

			interpreter.interpret(interpreter.tokenizer.getTokens(arg2));
		}
	}

	return 0;

}
