#include<iostream>
#include<string>

#include"headers/aj.h"
#include"headers/ajhelper.h"

Aj aj;

int main(int argc, char** argv) {
	AJ_Helper_Init(&aj);

	if(argc < 2) {
		aj.interpreter.interpreter_console();
	}
	else if (argc == 3){
		std::string arg1 = argv[1];
		std::string arg2 = argv[2];
		if(arg1.compare("-i") == 0) {
			auto tokens = aj.interpreter.tokenizer.getTokens(arg2);
			aj.interpreter.interpret(tokens);
		}
	}
	else if (argc == 2) {
		std::string arg = argv[1];
		aj.doFile(arg);
	}

	return 0;

}
