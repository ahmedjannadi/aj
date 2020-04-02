#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<map>

bool debugging = false;

std::vector<std::string> tokens;
std::map<std::string, std::string> variables;

std::string opcode = "";

std::string current_variable = "";
std::string current_operator = "";
std::string current_value = "";
std::string current_function = "";

std::vector<std::string> functions;

bool running = true;

void debug(std::string str) {
	if(debugging) {
		std::cout << str << std::endl;
	}
}

std::string getVariable(std::string var) {
	auto search = variables.find(var);
	if(search != variables.end()){
		return variables[var];
	}
	return "null";
}

std::vector<std::string> split(const std::string& str, char delim = ' ') {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;
	while(std::getline(ss, token, delim)) {
		tokens.push_back(token);
	}
	return tokens;
}



bool isVar(std::string str) {
	std::string letters = "abcdefghijklmnopqrstuwxyz_-";
	bool check = true;

	for(int i=0; i<letters.length(); i++) {
		if(str.find(letters[i]) < 0)
			check = false;
	}

	return check;

}

bool isNumber(std::string str) {
	std::string letters = "1234567890";
	bool check = true;

	for(int i=0; i<letters.length(); i++) {
		if(str.find(letters[i]) < 0)
			check = false;
	}

	return check;
}

bool isOperator(std::string str) {
	bool check = false;

	if(str == "=")
		check = true;
	if(str == "+")
		check = true;
	if(str == "-")
		check = true;

	return check;
}

std::string getToken(int pc) {
	if(pc >= 0 && pc < tokens.size())
		return tokens[pc];
	return "null";
}

void execute(std::vector<std::string> tokens) {
	int pc = 0;
	while(pc < tokens.size()) {
		opcode = getToken(pc);
		debug("Opcode: " +  opcode);
		if(opcode == "exit") {
			debug("exiting");
			running = false;
			break;
		}
		if(isVar(opcode)) {
			std::string oper = getToken(pc+1);
			if(isOperator(oper)){
				if(getToken(pc+1) == "=") {
					variables.insert_or_assign(opcode,getToken(pc+2));
					pc += 3;
				}
			}else {
				std::cout << getVariable(opcode) << std::endl;
				pc++;
			}
		}
	}
}

int main() {
	std::string s = "a = 5 b = 6 b";
	tokens = split(s);
	execute(tokens);

	while(running) {
		std:: cout << ">";
		std::getline(std::cin, s);

		tokens = split(s);
		execute(tokens);
	}
}
