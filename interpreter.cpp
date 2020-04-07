#include"interpreter.h"

Interpreter::Interpreter() {
}

void Interpreter::changePC(int i) {
	pc = i;
}

Token Interpreter::getToken(int pc) {
	if(pc>=0 && pc < tokens.size()) {
		return tokens[pc];
	}

	return Token("NULL","NULL");
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
	while(getToken(pc).value == "+") {
		pc++;
		Variable variable2 = getExpression();
		variable.value += variable2.value;
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
			else {
				std::cerr << "Error on : " << tokens[pc].value << std::endl;
				running = false;
			}
		} else if(stateMachine.state == StateMachine::STATEMENT) {
			if(tokens[pc].type == "VARIABLE") {
				//variable = tokens[pc].value;
				//pc++;
				variable_name = getVariableName();
				if(tokens[pc].type == "=") {
					pc++;
					/*if(tokens[pc].type == "NUMBER") {
						variables.insert_or_assign(variable_name,Variable("number",tokens[pc].value));
						variable_name = "";
						stateMachine.state = StateMachine::IDLE;
						changePC(pc + 1);
					}*/
					variables.insert_or_assign(variable_name,doExpression());
					variable_name = "";
					stateMachine.state = StateMachine::IDLE;
					pc++;
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
			} 
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
		//	std::cout << tokens[i].value << std::endl;
		//}
	}
}
