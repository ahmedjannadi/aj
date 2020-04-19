#include<iostream>
#include<string>
#include <unistd.h>

//#include"headers/interpreter.h"
#include"headers/aj.h"

//Interpreter interpreter;
Aj aj;

int aj_sleep() {
	if(aj.interpreter.stack.size() > 0 ) {
		sleep(std::stoi(aj.interpreter.stack[0].value));
	}
	return 0;
}

int print() {
	if(aj.interpreter.stack.size() > 0 ) {
		std::cout << aj.interpreter.stack[0].value << std::endl;
	}
	return 0;
}

int input() {
	if(aj.interpreter.stack.size() > 0 ) {
		std::cout << aj.interpreter.stack[0].value << std::endl;
	}
	std::string s;
	std::getline(std::cin,s);
	Variable v = Variable(Variable::STRING, s);
	aj.interpreter.stack.push_back(v);
	return 1;
}

int add() {
	if(aj.interpreter.stack.size() >= 2 ) {
		float a = std::stof(aj.interpreter.stack[0].value);
		float b = std::stof(aj.interpreter.stack[1].value);
		aj.interpreter.stack.push_back(Variable(Variable::NUMBER,std::to_string(a+b)));
	}

	return 1;
}

int type() {
	if(aj.interpreter.stack.size() > 0) {
		std::string types[] = {"STRING","NUMBER","BOOL","C_FUNCTION","FUNCTION","ARRAY","TABLE","NIL"};
		Variable v = Variable(Variable::STRING,types[aj.interpreter.stack[0].type]);
		aj.interpreter.stack.push_back(v);
		//std::cout << types[aj.interpreter.stack[0].type] << std::endl;
	}

	return 1;
}

int main(int argc, char** argv) {
	/*std::string test = "a = 5 b=5 c = \"hello world\"";
	auto tokens = interpreter.tokenizer.getTokens(test);

	for(int i=0; i<tokens.size(); i++) {
		std::cout << tokens[i].type << " : " << tokens[i].value << std::endl;
	} */
	aj.interpreter.setC_Function("print",print);
	aj.interpreter.setC_Function("input",input);
	aj.interpreter.setC_Function("add",add);
	aj.interpreter.setC_Function("type",type);
	aj.interpreter.setC_Function("sleep",aj_sleep);
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
