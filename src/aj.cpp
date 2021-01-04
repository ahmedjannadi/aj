#include<fstream>
#ifdef _WIN32

#elif
#include<unistd.h>
#endif
#include "headers/aj.h"

Aj::Aj() {
	//Aj::interpreter = new Interpreter();
}


void Aj::doCommand(std::string command) {
	interpreter.interpret(interpreter.tokenizer.getTokens(command));
}

void Aj::doFile(std::string filename) {
	std::string line;
	std::string file_content = "";
	std::ifstream file(filename);
	if(file.is_open()){
		while(getline(file,line)) {
			if(line[0] != '#') {
				file_content += line+" ";
			}
		}
	}
	file.close();
	interpreter.interpret(interpreter.tokenizer.getTokens(file_content));
}
