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
	}
	
	Variable::Type type = variable.type;
	while(getToken(pc).type == "+") { //
	//if(getToken(pc).value == "+") {
		int old_pc = pc;
		pc++;
		Variable variable2 = getExpression();
		if(variable.type == Variable::STRING){ 
			if(variable2.type != Variable::STRING) {
				variable.value +=  variable2.value;
				variable.type = Variable::STRING;
			}else {
				
			}
		} else {
			variable.value = std::to_string(std::stoi(variable.value)+std::stoi(variable2.value));
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
				std::cerr << "Error" << std::endl;
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
					variables.insert_or_assign(variable_name,getExpression());
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
					std::string string_to_print = "";
					if(tokens[pc].type == "NUMBER") {
						//std::cout << tokens[pc].value;
						string_to_print = tokens[pc].value;
					}
					else if(tokens[pc].type == "VARIABLE") {
						//std::cout << variables[tokens[pc]] << std::end;
						string_to_print = variables[tokens[pc].value].value;
					}
					else {
						std::cerr << "Expected a variable or a number (or a string WIP)" << std::endl;
						running = false;
					}
					pc++;
					if(tokens[pc].type == ")") {
						std::cout << string_to_print << std::endl;
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
	std::cout << "AJ 0.1.0 Copyright(C) Ahmed Jannadi 2020" << std::endl;
	std::string s;
	while(true) {
		std::cout << ">" ;
		std::getline(std::cin, s);
		interpret(tokenizer.getTokens(s));
	}
}
