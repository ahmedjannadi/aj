#include<fstream>
#include<iostream>
#include<map>
#include<vector>
#include<stack>
//#include"variable.h"
#include<iostream>
#include"src/headers/tokenizer.h"


int main(int argc, char** argv) {
	Tokenizer t;
	auto tokens = t.getTokens("print(type(test())) end");
	for(int i=0; i< tokens.size(); i++) {
		std::cout << tokens[i].value << std::endl;
	}
}
