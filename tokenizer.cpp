#include"tokenizer.h"

Tokenizer::Tokenizer() {
	reserved_tokens.push_back("function");
	reserved_tokens.push_back("end");
	reserved_tokens.push_back("print");
	reserved_tokens.push_back("type");
	reserved_tokens.push_back("true");
	reserved_tokens.push_back("false");
	reserved_tokens.push_back("while");
}

bool Tokenizer::checkChar(char c, std::string str) {
	bool check = true;
		if(str.find(std::tolower(c)) < 0 || str.find(std::tolower(c)) > str.length()){
			check = false;
		}
	return check;
}

bool Tokenizer::isLetter(char c) {
	return checkChar(c,"abcdefghijklmnopqrstuvwxyz");
}

bool Tokenizer::isDigit(char c) {
	return checkChar(c,"0123456789");
}

bool Tokenizer::isOperator(char c) {
	return checkChar(c,"-+*/%><=");
}

bool Tokenizer::checkReserved(std::string token) {
	
}

std::vector<Token> Tokenizer::getTokens(std::string str) {
	std::vector<Token> tokens;

	std::string token = "";
	std::string type = "";
	std::string last_type = "";
	bool started = false;
	bool to_string = false;
	int i = 0;
	while(i<=str.length()) {
		if(isLetter(str[i])) {
			type = "VARIABLE";
		} else if(isDigit(str[i])) {
			type = "NUMBER";
		} else if(isOperator(str[i])) {
			type = "OPERATOR";
		} else if(checkChar(str[i],"(){}[],;:")) {
			type = str[i];
		}else if(checkChar(str[i],"\"\'")){
				if(!to_string) {
					type = "STRING";
					i++;
				} else {
					type = "";
				}
				to_string = !to_string;
		} else {
			type = "";
		}
		if(to_string) {
			type = "STRING";
		}
		if(started) {
			if(last_type != type || i==str.length()) {
				if(last_type != "") {
					if(last_type == "VARIABLE"){
						bool reserved = false;
						for(int j=0;j<reserved_tokens.size();j++) {
							if(reserved_tokens[j] == token) {
								reserved = true;
								break;
							}
						}
						if(reserved) {
							tokens.push_back(Token(token,token));
						} else {
							tokens.push_back(Token(token,last_type));
						}
					} else {
						tokens.push_back(Token(token,last_type));
					}
				}
				token = "";
			}
		}

		started = true;
		token += str[i];
		last_type = type;
		i++;
	}

	return tokens;
}
