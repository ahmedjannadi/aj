#include"headers/interpreter.h"

Interpreter::Interpreter() {
}

void Interpreter::changePC(int i) {
	pc = i;
}

void Interpreter::printTokens() {
	for(int i=0; i<tokens.size(); i++) {
		std::cout << tokens[i].value << std::endl;
	}
}

Variable Interpreter::doFunction(std::string function_name) {
	Variable variable = Variable(Variable::NIL, "nil");
	int function_call_pc = pc;
	if(getToken(pc+1).value == "(" && getToken(pc+2).value == ")") {
		pc+=2;
		int function_call_end = pc;
		auto it = tokens.begin();
		for(int i=function_call_pc; i<function_call_end+1; i++) {
			//tokens.erase(tokens.begin()+function_call_pc,tokens.begin()+(function_call_end-function_call_pc)+1);
			tokens.erase(tokens.begin()+function_call_pc);
		}
		//it = tokens.begin();
		for(int i=variables[function_name].function_body.size()-1;i>=0;i--) {
			tokens.insert(it+function_call_pc,variables[function_name].function_body[i]);
		}

		//printTokens();

		pc=function_call_pc;
		int end_pc = function_call_pc+variables[function_name].function_body.size();
		while(pc != end_pc) {
			if(getToken(pc).value == "return") {
				pc++;
				Variable v = doExpression();
				pc = end_pc;
				return v;
			}
			doStatement();
		}

	}
		return variable;
}

Variable Interpreter::doFunctionToken() {
		Variable function = Variable(Variable::FUNCTION,"NIL");
		std::string function_name;
		if(getToken(pc).type == "function") {
			pc++;
			if(getToken(pc).type == "VARIABLE") {
				function_name = getToken(pc).value;
				pc++;
				if(getToken(pc).type == "(") {
					pc++;
					if(getToken(pc).type == ")") {
						pc++;
						int before_end = pc;
						getEndToken();
						int end_pc = pc;
						pc = before_end;
						while(pc != end_pc) {
							function.function_body.push_back(getToken(pc));
							pc++;
						}
						pc++;
					}
				}
			}
		}
		variables.insert_or_assign(function_name,function);
		return function;
}

bool Interpreter::getEndToken() {
	int end_count = 0;
	Token token = getToken(pc);
	while((token.value != "end" || end_count != 0) && pc < tokens.size() ) {
		if(token.value == "if") {
			end_count++;
		}
		if(token.value == "else") {
			end_count++;
		}
		if(token.value == "while") {
			end_count++;
		}
		if(token.value == "function") {
			end_count++;
		}
		if(token.value == "end") {
			end_count--;
		}
		pc++;
		token = getToken(pc);
	}
	return getToken(pc).value == "end";
}

void Interpreter::setC_Function(std::string name, int (*func)()) {
	Variable c_func = Variable(Variable::C_FUNCTION,"NIL");
	c_func.function = func;
	variables.insert_or_assign(name,c_func);
}


void Interpreter::doStatement() {
	std::string variable_name;
	if(tokens[pc].type == "VARIABLE") {
		variable_name = getVariableName();
		if(tokens[pc].value == "=") {
			pc++;
			variables.insert_or_assign(variable_name,doExpression());
			variable_name = "";
			stateMachine.state = StateMachine::IDLE;
		}
		else if (getToken(pc).value == "++") {
			variable_name = tokens[pc-1].value;
			float tmp = std::stof(variables[variable_name].value);
			std::string tmp_string = std::to_string(++tmp);
			// TODO trim ends
			//int last_place = tmp_string.length();
			//while(tmp_string[last_place] == '.' || tmp_string[last_place] == '0') {
			//	last_place--;
			//}
			//tmp_string = tmp_string.substr(0,last_place);
			variables[variable_name].value = tmp_string;
			stateMachine.state = StateMachine::IDLE;
			pc++;
		}
		else if (getToken(pc).value == "--") {
			variable_name = tokens[pc-1].value;
			float tmp = std::stof(variables[variable_name].value);
			std::string tmp_string = std::to_string(--tmp);
			variables[variable_name].value = tmp_string;
			stateMachine.state = StateMachine::IDLE;
			pc++;
		}
		else if (getToken(pc).value == "(") {
			int function_call_pc = pc-1;
			pc++;
			if(getToken(pc).value != ")") {
				stack.push_back(doExpression());
				while(getToken(pc).value == ",") {
					pc++;
					stack.push_back(doExpression());
				}
			}
			if(variables[variable_name].type == Variable::C_FUNCTION) {
				if(getToken(pc).value == ")") {
					//std::cout << "calling " << variable_name << "()" << std::endl;

					variables[variable_name].function();
					stack.clear();
					//stack.erase(stack.begin()+stack_pointer, stack.end());
					pc++;
				}
			}
			else if(variables[variable_name].type == Variable::FUNCTION) {
				if(getToken(pc).value == ")") {
					/*int function_call_end = pc;
					auto it = tokens.begin();
					for(int i=function_call_pc; i<function_call_end+1; i++) {
						//tokens.erase(tokens.begin()+function_call_pc,tokens.begin()+(function_call_end-function_call_pc)+1);
						tokens.erase(tokens.begin()+function_call_pc);
					}
					//it = tokens.begin();
					for(int i=variables[variable_name].function_body.size()-1;i>=0;i--) {
						tokens.insert(it+function_call_pc,variables[variable_name].function_body[i]);
					}
					pc=function_call_pc;*/
					pc = function_call_pc;
					doFunction(variable_name);
				}
			}
			else {
				std::cout << "error: " << variable_name << " is not a function" << std::endl;
				stateMachine.state = StateMachine::IDLE;
			}
		}
		else {
			std::cerr << "wrong syntax -> " + tokens[pc-1].value + " " + tokens[pc].value << std::endl;
			running = false;
		}
	}
	else if(getToken(pc).value == "while") {
		doWhile();
	}
	else if(getToken(pc).value == "if") {
		doIf();
	}
	else if(getToken(pc).value == "function") {
		doFunctionToken();
	}
}

void Interpreter::doWhile() {
	pc++;
	if(getToken(pc).value == "(") {
		pc++;
		int condition_pc = pc;
		Variable condition = doExpression();
		if(getToken(pc).value == ")") {
				pc++;
			if(condition.type == Variable::BOOL) {
				int end_pc = 0;
				if(condition.value == "true") {
				while(condition.value == "true") {
					while(getToken(pc).value != "end") {
						//std::cout << "Token " << getToken(pc).value << std::endl;
						doStatement();
					}
					end_pc = pc;
					pc = condition_pc;
					condition = doExpression();
					pc++;
				}
				}else {
					getEndToken();
					end_pc = pc;
				}
				pc = end_pc + 1;
			}
			 else {
				std::cout << "expected a boolean in while" << std::endl;
			}
		} else {
			std::cout << "expected ( in while" << std::endl;
		}
	}
}

void Interpreter::doIf() {
	pc++;
	if(getToken(pc).value == "(") {
		pc++;
		int condition_pc = pc;
		Variable condition = doExpression();
		if(getToken(pc).value == ")") {
				pc++;
			if(condition.type == Variable::BOOL) {
				int end_pc = 0;
				if(condition.value == "true") {
					while(getToken(pc).value != "end") {
						doStatement();
					}
					end_pc = pc;
				} else {
					getEndToken();
					end_pc = pc;
					if(getToken(pc+1).value == "else") {
						pc += 2;
						while(getToken(pc).value != "end") {
							doStatement();
						}
						end_pc = pc;
					}
				}
				pc = end_pc + 1;
			}
			 else {
				std::cout << "expected a boolean in if" << std::endl;
			}
		} else {
			std::cout << "expected ( in if" << std::endl;
		}
	}
}

Token Interpreter::getToken(int pc) {
	if(pc>=0 && pc < tokens.size()) {
		return tokens[pc];
	}

	return Token("NULL","NULL");
}

bool Interpreter::isOperator(Token t) {
	if (t.value == "+") {
		return true;
	}
	else if (t.value == "==") {
		return true;
	}
	else if (t.value == ">") {
		return true;
	}
	else if (t.value == ">=") {
		return true;
	}
	else if (t.value == "<") {
		return true;
	}
	else if (t.value == "<=") {
		return true;
	}
	else if (t.value == "and") {
		return true;
	}
	else if (t.value == "or") {
		return true;
	}

	return false;
}

Variable Interpreter::getVariable() {
	if(getToken(pc).type != "VARIABLE") {
		std::cerr << "Error parsing variable" << std::endl;
	} else {
		return variables[getVariableName()];
	}
	return Variable(Variable::NIL,"nil");
}

std::string Interpreter::getVariableName() {
	std::string variable_name = "";
	if(getToken(pc).type != "VARIABLE") {
		std::cerr << "Error parsing variable name" << std::endl;
	} else {
		variable_name = tokens[pc].value;
		pc++;
	}
	return variable_name;
}

Variable Interpreter::doExpression() {
	Variable variable(Variable::NIL,"nil");
	
	variable = getExpression();
	while(isOperator(getToken(pc))) {
		pc++;
		if(getToken(pc-1).value == "+") {
			Variable variable2 = getExpression();
			if(variable2.type == Variable::STRING) {
				variable.type = Variable::STRING;
			}
			variable.value += variable2.value;
		}
		else if(getToken(pc-1).value == "==") {
			Variable variable2 = getExpression();
			if(variable2.type == Variable::NUMBER && variable.type == Variable::NUMBER) {
				variable.type = Variable::BOOL;
				if(std::stof(variable.value) == std::stof(variable2.value)) {
					variable.value = "true";
				} else {
					variable.value = "false";
				}
			}
		}
		else if(getToken(pc-1).value == ">") {
			Variable variable2 = getExpression();
			if(variable2.type == Variable::NUMBER && variable.type == Variable::NUMBER) {
				variable.type = Variable::BOOL;
				if(std::stof(variable.value) > std::stof(variable2.value)) {
					variable.value = "true";
				} else {
					variable.value = "false";
				}
			}
		}
		else if(getToken(pc-1).value == ">=") {
			Variable variable2 = getExpression();
			if(variable2.type == Variable::NUMBER && variable.type == Variable::NUMBER) {
				variable.type = Variable::BOOL;
				if(std::stof(variable.value) >= std::stof(variable2.value)) {
					variable.value = "true";
				} else {
					variable.value = "false";
				}
			}
		}
		else if(getToken(pc-1).value == "<") {
			Variable variable2 = getExpression();
			if(variable2.type == Variable::NUMBER && variable.type == Variable::NUMBER) {
				variable.type = Variable::BOOL;
				if(std::stof(variable.value) < std::stof(variable2.value)) {
					variable.value = "true";
				} else {
					variable.value = "false";
				}
			}
		}
		else if(getToken(pc-1).value == "<=") {
			Variable variable2 = getExpression();
			if(variable2.type == Variable::NUMBER && variable.type == Variable::NUMBER) {
				variable.type = Variable::BOOL;
				if(std::stof(variable.value) <= std::stof(variable2.value)) {
					variable.value = "true";
				} else {
					variable.value = "false";
				}
			}
		}
		else if(getToken(pc-1).value == "or") {
			Variable variable2 = getExpression();
			if(variable2.type == Variable::BOOL && variable.type == Variable::BOOL) {
				variable.type = Variable::BOOL;
				if(variable.value == "true" || variable2.value == "true") {
					variable.value = "true";
				} else {
					variable.value = "false";
				}
			}
		}

		else if(getToken(pc-1).value == "and") {
			Variable variable2 = getExpression();
			if(variable2.type == Variable::BOOL && variable.type == Variable::BOOL) {
				variable.type = Variable::BOOL;
				if(variable.value == "true" && variable2.value == "true") {
					variable.value = "true";
				} else {
					variable.value = "false";
				}
			}
		}
		else {
			std::cout << getToken(pc).value << std::endl;
		}
	}

	return variable;
}

Variable Interpreter::getExpression() {
	Variable variable = Variable(Variable::NIL,"nil");

	if(getToken(pc).type == "NUMBER") {
		variable.type = Variable::NUMBER;
		variable.value = getToken(pc).value;
		pc++;
	}
	else if(getToken(pc).type == "VARIABLE"){
		if(getToken(pc+1).type != "("){
			variable = getVariable();
		} else {
			std::string function_name = getToken(pc).value;
			variable = doFunction(function_name);
		}
	}
	else if(getToken(pc).type == "STRING"){
		variable.type = Variable::STRING;
		variable.value = getToken(pc).value;
		pc++;
	}
	else if(getToken(pc).type == "true"){
		variable.type = Variable::BOOL;
		variable.value = "true";
		pc++;
	}
	else if(getToken(pc).type == "false"){
		variable.type = Variable::BOOL;
		variable.value = "false";
		pc++;
	}
	
	Variable::Type type = variable.type;
	//while(getToken(pc).value == "+") { //
	if(getToken(pc).value == "+") {
		int old_pc = pc;
		pc++;
		Variable variable2 = getExpression();
		if(variable2.type == Variable::STRING){ 
			if(variable.type == Variable::STRING) {
				variable.value +=  variable2.value;
				variable.type = Variable::STRING;
			} else {
				pc = old_pc;
			}

		} else if(variable2.type == Variable::NUMBER) {
			if(variable.type == Variable::NUMBER) {
				variable.value = std::to_string(std::stof(variable.value)+std::stof(variable2.value));
			} else {
				pc = old_pc;
			}
		}
	}


	return variable;
}

void Interpreter::interpret_function(Variable function){
	int old_pc = pc;
	std::vector<Token> old_tokens = tokens;

	pc = 0;
	tokens = function.function_body;
	
	// do stuff
	
	pc = old_pc;
	tokens = old_tokens;
}

void Interpreter::interpret(std::vector<Token> t) {
	tokens = t;
	std::string variable_name = "";

	pc = 0;
	running = true;

	while(pc < tokens.size() && running) {
		if(stateMachine.state == StateMachine::IDLE) {
			if(getToken(pc).type == "VARIABLE") {
				stateMachine.state = StateMachine::STATEMENT;
			}
			else if(getToken(pc).type == "while") {
				//stateMachine.state = StateMachine::WHILE;
				stateMachine.state = StateMachine::STATEMENT;
			}
			else if(getToken(pc).type == "if") {
				//stateMachine.state = StateMachine::IF;
				stateMachine.state = StateMachine::STATEMENT;
			}
			else if(getToken(pc).type == "function") {
				//stateMachine.state = StateMachine::IF;
				stateMachine.state = StateMachine::STATEMENT;
			}
			else {
				std::cerr << "Error on : " << tokens[pc].value << std::endl;
				running = false;
			}
		} else if(stateMachine.state == StateMachine::STATEMENT) {
			doStatement();
		} else if(stateMachine.state == StateMachine::PRINT) {
			changePC(pc + 1);
			if(tokens[pc].type == "(") {
					pc++;
					Variable variable = doExpression();
					if(tokens[pc].type == ")") {
						std::cout << variable.value << std::endl;
						stateMachine.state = StateMachine::IDLE;
						pc++;
					}
					else {
						std::cerr << "Expected ) after print" << std::endl;
						running = false;
					}
			}else {
				std::cout << "Expected after print (" << std::endl;
				running = false;
			}
		}
//		else if(stateMachine.state == StateMachine::WHILE) {
//			doWhile();
//		}
//		else if(stateMachine.state == StateMachine::IF) {
//			doIf();
//		}
		else {
			running = false;
		}
	}
}

void Interpreter::interpreter_console() {
	std::cout << "AJ 0.3.0 Copyright(C) Ahmed Jannadi 2020" << std::endl;
	std::string s;
	while(true) {
		std::cout << ">" ;
		std::getline(std::cin, s);
		interpret(tokenizer.getTokens(s));
		//printTokens();
	}
}
