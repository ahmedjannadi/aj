#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<map>

std::vector<std::string> tokens;
std::map<std::string,std::string> vars;

bool isLetter(char c) {
	bool check = true;
	std::string letters = "abcdefghijklmnopqrstuvwxyz";
		if(letters.find(std::tolower(c)) < 0 || letters.find(std::tolower(c)) > letters.length()){
			check = false;
		}
	return check;
}

bool isDigit(char c) {
	bool check = true;
	std::string letters = "0123456789";
		if(letters.find(std::tolower(c)) < 0 || letters.find(std::tolower(c)) > letters.length()){
			check = false;
		}
	return check;
}

bool isSymbol(char c) {
	bool check = true;
	std::string letters = "=-+;:,(){}[]";
		if(letters.find(std::tolower(c)) < 0 || letters.find(std::tolower(c)) > letters.length()){
			check = false;
		}
	return check;
}

void split(std::string str, char delim=' ') {
		std::string token = "";
		std::string type = "";
		std::string last_type = "";
		bool started = false;
		int i = 0;
		while(i<=str.length()) {
			if(isLetter(str[i])) {
				type = "letter";
			}else if(isDigit(str[i])) {
				type = "digit";
			} else if(isSymbol(str[i])) {
				type = "symbol";
			} else {
				type = "";
			}
			if(started) {
				if(last_type != type || i==str.length()) {
					if(last_type != "") {
						tokens.push_back(token);
					}
					token = "";
				}
			}

			started = true;
			token += str[i];
			last_type = type;
			i++;
		}
}

// a type=letter last_type=letter => " " type = "" 

void split_bak(std::string str, char delim=' ') {
	std::stringstream ss(str);
	std::string token;
	while(std::getline(ss, token, delim)) {
		tokens.push_back(token);
	}
}


int main(int argc, char** argv) {
	std::string test = "a = 5 b=5 c=5+a+3 function test() print(a) end test()";
	split(test);

	for(int i=0; i<tokens.size(); i++) {
		std::cout << "\""+tokens[i]+"\"" << std::endl;
	}
	return 0;
}
