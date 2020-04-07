#include"interpreter.h"

Interpreter::Interpreter() {
}

void Interpreter::changePC(int i) {
	pc = i;
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
		} else if (getToken(pc).value == "++") {
			variable_name = tokens[pc-1].value;
			int tmp = std::stoi(variables[variable_name].value);
			variables[variable_name].value = std::to_string(++tmp);
			stateMachine.state = StateMachine::IDLE;
			pc++;
		} else {
			std::cerr << "wrong syntax -> " + tokens[pc-1].value + " " + tokens[pc].type << std::endl;
			running = false;
		}
	} else if(getToken(pc).value == "print") {
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
	} else if (t.value == ">") {
		return true;
	} else if (t.value == "<") {
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
			variable.value += variable2.value;
		} else if(getToken(pc-1).value == ">") {
			Variable variable2 = getExpression();
			if(variable2.type == Variable::NUMBER && variable.type == Variable::NUMBER) {
				variable.type = Variable::BOOL;
				if(std::stoi(variable.value) > std::stoi(variable2.value)) {
					variable.value = "true";
				} else {
					variable.value = "false";
				}
			}
		} else if(getToken(pc-1).value == "<") {
			Variable variable2 = getExpression();
			if(variable2.type == Variable::NUMBER && variable.type == Variable::NUMBER) {
				variable.type = Variable::BOOL;
				if(std::stoi(variable.value) < std::stoi(variable2.value)) {
					variable.value = "true";
				} else {
					variable.value = "false";
				}
			}
		} else {
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
	}else if(getToken(pc).type == "VARIABLE"){
		variable = getVariable();
	}else if(getToken(pc).type == "STRING"){
		variable.type = Variable::STRING;
		variable.value = getToken(pc).value;
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
				variable.value = std::to_string(std::stoi(variable.value)+std::stoi(variable2.value));
			} else {
				pc = old_pc;
			}
		}
	}


	return variable;
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
			else if(getToken(pc).type == "print") {
				stateMachine.state = StateMachine::PRINT;
			}
			else if(getToken(pc).type == "while") {
				stateMachine.state = StateMachine::WHILE;
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
		} else if(stateMachine.state == StateMachine::WHILE) {
			pc++;
			if(getToken(pc).value == "(") {
				pc++;
				int condition_pc = pc;
				Variable condition = doExpression();
				if(getToken(pc).value == ")") {
						pc++;
					if(condition.type == Variable::BOOL) {
						int end_pc = 0;
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
						pc = end_pc + 1;
					}
					 else {
						std::cout << "expected a boolean in while" << std::endl;
					}
				} else {
					std::cout << "expected ( in while" << std::endl;
				}
			}
		} else {
			running = false;
		}
	}
}

void Interpreter::interpreter_console() {
	std::cout << "AJ 0.2.0 Copyright(C) Ahmed Jannadi 2020" << std::endl;
	std::string s;
	while(true) {
		std::cout << ">" ;
		std::getline(std::cin, s);
		interpret(tokenizer.getTokens(s));
		//for(int i=0; i<tokens.size(); i++) {
		//	std::cout << tokens[i].type << " : " << tokens[i].value << std::endl;
		//}
	}
}
