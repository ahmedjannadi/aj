#include "headers/ajHelper.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#elif
#include<unistd.h>
#endif

Aj* aj_helper = NULL;

void AJ_Helper_Init(Aj* aj2) {
	aj_helper = aj2;
	aj_helper->interpreter.setC_Function("test", test);
	aj_helper->interpreter.setC_Function("print", print);
	aj_helper->interpreter.setC_Function("sleep", sleep2);
	aj_helper->interpreter.setC_Function("input", input);
	aj_helper->interpreter.setC_Function("type", type);
	aj_helper->interpreter.setC_Function("add", add);
	aj_helper->interpreter.setC_Function("len", len);
	aj_helper->interpreter.setC_Function("append", append);
}

int test() {
	std::cout << "hello world" << std::endl;
	return 0;
}

int print() {
	if(aj_helper->interpreter.stack.size() > 0 ) {
		std::cout << aj_helper->interpreter.stack[0].value << std::endl;
	}
	return 0;
}
int len() {
	if(aj_helper->interpreter.stack.size() > 0 ) {
		if(aj_helper->interpreter.stack[0].type == Variable::ARRAY) {
			aj_helper->interpreter.stack.push_back(Variable(Variable::NUMBER,std::to_string(aj_helper->interpreter.stack[0].array_values.size())));
		}
		else if(aj_helper->interpreter.stack[0].type == Variable::STRING) {
			aj_helper->interpreter.stack.push_back(Variable(Variable::NUMBER,std::to_string(aj_helper->interpreter.stack[0].value.length())));
		}
	}
	return 1;
}

int sleep2() {
	if(aj_helper->interpreter.stack.size() > 0 ) {
#ifdef _WIN32
		Sleep(std::stoi(aj_helper->interpreter.stack[0].value));
#elif
		sleep(std::stoi(aj_helper->interpreter.stack[0].value));
#endif
		
	}
	return 0;
}

int input() {
	if(aj_helper->interpreter.stack.size() > 0 ) {
		std::cout << aj_helper->interpreter.stack[0].value << std::endl;
	}
	std::string s;
	std::getline(std::cin,s);
	Variable v = Variable(Variable::STRING, s);
	aj_helper->interpreter.stack.push_back(v);
	return 1;
}

int add() {
	if(aj_helper->interpreter.stack.size() >= 2 ) {
		float a = std::stof(aj_helper->interpreter.stack[0].value);
		float b = std::stof(aj_helper->interpreter.stack[1].value);
		aj_helper->interpreter.stack.push_back(Variable(Variable::NUMBER,std::to_string(a+b)));
	}

	return 1;
}

int type() {
	if(aj_helper->interpreter.stack.size() > 0) {
		std::string types[] = {"STRING","NUMBER","BOOL","C_FUNCTION","FUNCTION","ARRAY","TABLE","NIL"};
		Variable v = Variable(Variable::STRING,types[aj_helper->interpreter.stack[0].type]);
		aj_helper->interpreter.stack.push_back(v);
		//std::cout << types[aj_helper->interpreter.stack[0].type] << std::endl;
	}

	return 1;
}

int append() {
	if (aj_helper->interpreter.stack.size() >= 2) {
		if (aj_helper->interpreter.stack[0].type == Variable::ARRAY) {
			Variable elemToAdd = Variable(aj_helper->interpreter.stack[1].type, aj_helper->interpreter.stack[1].value);
			Variable tmpArray = aj_helper->interpreter.stack[0];
			tmpArray.array_values.push_back(elemToAdd);

			aj_helper->interpreter.stack.clear();
			aj_helper->interpreter.stack.push_back(tmpArray);
		}
	}
	return 1;
}
