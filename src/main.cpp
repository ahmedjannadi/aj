#include<iostream>
#include<string>

#include"headers/interpreter.h"

Interpreter interpreter;

int print() {
	if(interpreter.stack.size() >0 ) {
		std::cout << interpreter.stack[0].value << std::endl;
	}
	return 0;
}

int add() {
	if(interpreter.stack.size() >= 2 ) {
		float a = std::stof(interpreter.stack[0].value);
		float b = std::stof(interpreter.stack[1].value);
		std::cout << a+b << std::endl;
	}

	return 0;
}

int type() {
	if(interpreter.stack.size() >= 1) {
		std::string types[] = {"STRING","NUMBER","BOOL","C_FUNCTION","FUNCTION","ARRAY","TABLE","NIL"};
		std::cout << types[interpreter.stack[0].type] << std::endl;
	}

	return 0;
}

int main(int argc, char** argv) {
	/*std::string test = "a = 5 b=5 c = \"hello world\"";
	auto tokens = interpreter.tokenizer.getTokens(test);

	for(int i=0; i<tokens.size(); i++) {
		std::cout << tokens[i].type << " : " << tokens[i].value << std::endl;
	} */
	interpreter.setC_Function("print",print);
	interpreter.setC_Function("add",add);
	interpreter.setC_Function("type",type);
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
