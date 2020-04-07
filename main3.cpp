#include<iostream>
#include<string>

#include"interpreter.h"

Interpreter interpreter;

int main(int argc, char** argv) {
	/*std::string test = "a = 5 b=5 c = \"hello world\"";
	auto tokens = interpreter.tokenizer.getTokens(test);

	for(int i=0; i<tokens.size(); i++) {
		std::cout << tokens[i].type << " : " << tokens[i].value << std::endl;
	} */
	
	if(argc < 2) {
		interpreter.interpreter_console();
	} else {
		std::string arg1 = argv[1];
		std::string arg2 = argv[2];
		if(arg1.compare("-i") == 0) {
			auto tokens = interpreter.tokenizer.getTokens(arg2);
			interpreter.interpret(tokens);
		}
	}

	return 0;

}
