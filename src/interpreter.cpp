#include"headers/interpreter.h"

Interpreter::Interpreter() {
}


void Interpreter::changePC(int i) {
	pc = i;
}

void Interpreter::printTokens(std::vector<Token> tokens) {
	for(int i=0; i<tokens.size(); i++) {
		std::cout << getToken(i).value << std::endl;
	}
}

void Interpreter::setInteger(std::string name, int v) {
	variables[name] = Variable(Variable::NUMBER,std::to_string(v));
}

void Interpreter::setFloat(std::string name, float v) {
	variables[name] = Variable(Variable::NUMBER,std::to_string(v));
}

void Interpreter::setString(std::string name, std::string v) {
	variables[name] = Variable(Variable::STRING,v);
}

void Interpreter::setBool(std::string name, bool v) {
	std::string v_bool;
	if(v) {
		v_bool = "true";
	}else {
		v_bool = "false";
	}
	variables[name] = Variable(Variable::BOOL,v_bool);
}

int Interpreter::getInteger(std::string name) {
	return std::stoi(variables[name].value);
}

float Interpreter::getFloat(std::string name) {
	return std::stof(variables[name].value);
}

bool Interpreter::getBool(std::string name) {
	return variables[name].value == "true";
}

std::string Interpreter::getString(std::string name) {
	return variables[name].value;
}


int Interpreter::getStackInteger(int i) {
	return std::stoi(stack[i].value);
}

float Interpreter::getStackFloat(int i) {
	return std::stof(stack[i].value);
}

std::string Interpreter::getStackString(int i) {
	return stack[i].value;
}

bool Interpreter::getStackBool(int i) {
	return stack[i].value == "true";
}

void Interpreter::pushStackInteger(int v) {
	stack.push_back(Variable(Variable::NUMBER,std::to_string(v)));
}

void Interpreter::pushStackFloat(float v) {
	stack.push_back(Variable(Variable::NUMBER,std::to_string(v)));
}

void Interpreter::pushStackString(std::string v) {
	stack.push_back(Variable(Variable::STRING,v));
}

void Interpreter::pushStackBool(bool v) {
	std::string v_bool;
	if(v) {
		v_bool = "true";
	}
	else {
		v_bool = "false";
	}
	stack.push_back(Variable(Variable::BOOL,v_bool));
}

Variable Interpreter::doC_Function(std::string function_name) {
	Variable variable = Variable(Variable::NIL, "nil");
	if(getToken(pc+1).value == "(") {
		pc+=2;
		
		while(getToken(pc).value != ")") {
				stack.push_back(doExpression());
				if(getToken(pc).value == ",") {
					pc++;
				}
		}

		if(getToken(pc).value == ")") {
			if(variables[function_name].function() == 1) {
				variable = stack[stack.size()-1];
			}
			stack.clear();
			pc++;
		}
	}
		return variable;
}

Variable Interpreter::doFunction(std::string function_name) {
	Variable function;
	Variable variable = Variable(Variable::NIL, "nil");
	if(getToken(pc+1).value == "(") {
		pc+=2;
		function = variables[function_name];
		
		int function_variable_name_index = 0;

		while(getToken(pc).value != ")") {
				//function.function_variables.push_back(doExpression());
				function.function_params[function.function_variable_names[function_variable_name_index]]=doExpression();
				function_variable_name_index++;

				if(getToken(pc).value == ",") {
					pc++;
				}
		}

		if(getToken(pc).value == ")") {
			if(function.type == Variable::FUNCTION) {
				tokens_stack.push(tokens);
				pc_stack.push(pc);
				tokens = function.function_body;
				pc = 0;
				function_stack.push_back(function);
				function_stack_pointer++;
		
				while(pc < tokens.size()) {
					if(getToken(pc).value == "return") {
						pc++;
						variable = doExpression();
						break;
					}
	
					if(getToken(pc).value == "if") {
						Variable tmp = doIfFunction();
						if(tmp.type != Variable::NIL) {
							variable = tmp;
							break;
						}
					}
					doStatement();
				}
	
				function.function_variables.clear(); // clear params
		
				pc = pc_stack.top();
				tokens = tokens_stack.top();
				pc_stack.pop();
				tokens_stack.pop();
				function_stack.pop_back();
				function_stack_pointer--;
			}
			
				
			pc++;
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
					// TODO fetch params
					while(getToken(pc).type != ")") {
						if(getToken(pc).type == "VARIABLE") {
							function.function_variable_names.push_back(getToken(pc).value);
							pc++;
						}
						if(getToken(pc).value == ",") {
							pc++;
						}
					}

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

		//function.function_variable_names.push_back("ahmed");
		//function.function_variables.push_back(Variable(Variable::NUMBER, "12"));

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
	int start_pc = pc;
	if(getToken(pc).type == "VARIABLE") {
		variable_name = getVariableName();

		// TODO Array

		//Variable *var;
		/*if(variables[variable_name].type == Variable::Array) {
			if(getToken(pc).value == "[") {
				pc++;
				if(getToken(pc).value == "]") {
					
				}
			}
		}*/

		if(getToken(pc).value == "=") {
			pc++;
			variables.insert_or_assign(variable_name,doExpression());
			variable_name = "";
			stateMachine.state = StateMachine::IDLE;
		}
		else if (getToken(pc).value == "++") {
			variable_name = getToken(pc-1).value;
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
			variable_name = getToken(pc-1).value;
			float tmp = std::stof(variables[variable_name].value);
			std::string tmp_string = std::to_string(--tmp);
			variables[variable_name].value = tmp_string;
			stateMachine.state = StateMachine::IDLE;
			pc++;
		}
		else if (getToken(pc).value == "(") {
			pc--;
			if(variables[variable_name].type == Variable::C_FUNCTION) {
				doC_Function(variable_name);
			}
			else if(variables[variable_name].type == Variable::FUNCTION) {
				doFunction(variable_name);
			}
			else {
				std::cout << "error: " << variable_name << " is not a function" << std::endl;
				stateMachine.state = StateMachine::IDLE;
				while(getToken(pc).value != ")" || pc >= tokens.size()) {
					pc++;
				}
				pc++;
			}
		}
		else {
			//pc = start_pc;
			//std::cout << doExpression().value << std::endl;
			std::cout << "wrong syntax -> " + getToken(pc-1).value + " " + getToken(pc).value << std::endl;
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
				int start_pc = pc;
				getEndToken();
				int end_if_pc = pc;
				pc = start_pc;

			if(condition.type == Variable::BOOL) {
				if(condition.value == "true") {
					while(pc<end_if_pc) {
						// check return statement
						doStatement();
					}
					if(getToken(pc+1).value == "else") {
						pc++;
						getEndToken();
					}
				} else {
					pc = end_if_pc;
					if(getToken(pc+1).value == "else") {
						pc += 2;
						int start_else_pc = pc;
						getEndToken();
						int end_else_pc = pc;
						pc = start_else_pc;
						while(pc<end_else_pc) {
							// check return statement
							doStatement();
						}
					}
				}
				pc++; 
			}
			else if (condition.type == Variable::STRING) {
				if (condition.value == "true") {
					while (pc < end_if_pc) {
						// check return statement
						doStatement();
					}
					if (getToken(pc + 1).value == "else") {
						pc++;
						getEndToken();
					}
				}
				else {
					pc = end_if_pc;
					if (getToken(pc + 1).value == "else") {
						pc += 2;
						int start_else_pc = pc;
						getEndToken();
						int end_else_pc = pc;
						pc = start_else_pc;
						while (pc < end_else_pc) {
							// check return statement
							doStatement();
						}
					}
				}
				pc++;
			}
			else {
				std::cout << "expected a boolean or string in if" << std::endl;
			}
		} else {
			std::cout << "expected ) in if" << std::endl;
		}
	} 
	else {
			std::cout << "expected ( in if" << std::endl;
	}
}

Variable Interpreter::doIfFunction() {
	pc++;
	if(getToken(pc).value == "(") {
		pc++;
		int condition_pc = pc;
		Variable condition = doExpression();
		if(getToken(pc).value == ")") {
				pc++;
				int start_pc = pc;
				getEndToken();
				int end_if_pc = pc;
				pc = start_pc;

			if(condition.type == Variable::BOOL) {
				if(condition.value == "true") {
					while(pc<end_if_pc) {
						// check return statement
						if(getToken(pc).value == "return") {
							pc++;
							return doExpression();
						}
						doStatement();
					}
					if(getToken(pc+1).value == "else") {
						pc++;
						getEndToken();
					}
				} else {
					pc = end_if_pc;
					if(getToken(pc+1).value == "else") {
						pc += 2;
						int start_else_pc = pc;
						getEndToken();
						int end_else_pc = pc;
						pc = start_else_pc;
						while(pc<end_else_pc) {
							// check return statement
							if(getToken(pc).value == "return") {
								pc++;
								return doExpression();
							}
							doStatement();
						}
					}
				}
				pc++; 
			}
			 else {
				std::cout << "expected a boolean in if" << std::endl;
			}
		} else {
			std::cout << "expected ( in if" << std::endl;
		}
	}

	return Variable(Variable::NIL,"NIL");
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
	else if (t.value == "-") {
		return true;
	}
	else if (t.value == "*") {
		return true;
	}
	else if (t.value == "/") {
		return true;
	}
	else if (t.value == "!=") {
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

/*Variable Interpreter::getVariable() {
	if(getToken(pc).type != "VARIABLE") {
		std::cout << "Error parsing variable" << std::endl;
	} else {
		std::string variable_name = getVariableName();
		for(int i=function_stack_pointer-1; i>=0 ; i--) {
			Variable function = variables[function_stack[i]];
			auto tmp = std::find(function.function_variable_names.begin(), function.function_variable_names.end(),variable_name);
			int tmp2 = 0;
			for(int j=0; j<function_stack.size(); j++) {
				if (function_stack[i] == function_stack[j]) {
					tmp2++;
				}
			}
			if(tmp != function.function_variable_names.end()) {
				return function.function_variables.at(((tmp2-1)*function.function_variable_names.size())+std::distance(function.function_variable_names.begin(), tmp));
			}
		}
		return variables[variable_name];
	}
	return Variable(Variable::NIL,"nil");
}*/

Variable Interpreter::getVariable() {
	if(getToken(pc).type != "VARIABLE") {
		std::cerr << "Error parsing variable" << std::endl;
	} else {
			std::string variable_name = getVariableName();
			for(int i=function_stack_pointer-1; i>=0; i--) {
				Variable function = function_stack[i];
				if (function.function_params.find(variable_name) != function.function_params.end())
					return function.function_params[variable_name];
			}
		return variables[variable_name];
	}
	return Variable(Variable::NIL,"nil");
}

/*Variable Interpreter::getVariable() {
	if(getToken(pc).type != "VARIABLE") {
		std::cerr << "Error parsing variable" << std::endl;
	} else {
			std::string variable_name = getVariableName();
			for(int i=function_stack_pointer-1; i>=0; i--) {
				Variable function = function_stack[i];
				auto tmp = std::find(function.function_variable_names.begin(), function.function_variable_names.end(), variable_name);
				return function.function_variables.at(std::distance(function.function_variable_names.begin(),tmp));
			}
		return variables[variable_name];
	}
	return Variable(Variable::NIL,"nil");
}*/

/*Variable Interpreter::getVariable() {
	if(getToken(pc).type != "VARIABLE") {
		std::cerr << "Error parsing variable" << std::endl;
	} else {
		return variables[getVariableName()];
	}
	return Variable(Variable::NIL,"nil");
}*/

std::string Interpreter::getVariableName() {
	std::string variable_name = "";
	if(getToken(pc).type != "VARIABLE") {
		std::cout << "Error parsing variable name" << std::endl;
	} else {
		variable_name = getToken(pc).value;
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
		else if(getToken(pc-1).value == "-") {
			Variable variable2 = getExpression();
			if((variable.type == Variable::NUMBER || variable.type == Variable::NIL) && variable2.type == Variable::NUMBER) {
				if(variable.type == Variable::NIL) {
					variable.value = "0";
				}
				variable.type = Variable::NUMBER;
				variable.value = std::to_string(std::stof(variable.value)-std::stof(variable2.value));
			}
		}
		else if(getToken(pc-1).value == "*") {
			Variable variable2 = getExpression();
			if((variable.type == Variable::NUMBER || variable.type == Variable::NIL) && variable2.type == Variable::NUMBER) {
				if(variable.type == Variable::NIL) {
					variable.value = "0";
				}
				variable.type = Variable::NUMBER;
				variable.value = std::to_string(std::stof(variable.value)*std::stof(variable2.value));
			}
		}
		else if(getToken(pc-1).value == "/") {
			Variable variable2 = getExpression();
			if((variable.type == Variable::NUMBER || variable.type == Variable::NIL) && variable2.type == Variable::NUMBER) {
				if(variable.type == Variable::NIL) {
					variable.value = "0";
				}
				variable.type = Variable::NUMBER;
				variable.value = std::to_string(std::stof(variable.value)/std::stof(variable2.value));
			}
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
			else if (variable2.type == Variable::STRING && variable.type == Variable::STRING) {
				variable.type = Variable::BOOL;
				if (variable.value == variable2.value) {
					variable.value = "true";
				}
				else {
					variable.value = "false";
				}
			}
		}
		else if (getToken(pc - 1).value == "!=") {
			Variable variable2 = getExpression();
			if (variable2.type == Variable::NUMBER && variable.type == Variable::NUMBER) {
				variable.type = Variable::BOOL;
				if (std::stof(variable.value) == std::stof(variable2.value)) {
					variable.value = "false";
				}
				else {
					variable.value = "true";
				}
			}
			else if (variable2.type == Variable::STRING && variable.type == Variable::STRING) {
				variable.type = Variable::BOOL;
				if (variable.value == variable2.value) {
					variable.value = "false";
				}
				else {
					variable.value = "true";
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
			Variable variable2 = doExpression();
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
			Variable variable2 = doExpression();
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
		if(getToken(pc+1).type == "("){
			std::string function_name = getToken(pc).value;
			if(variables[function_name].type == Variable::FUNCTION) {
				variable = doFunction(function_name);
			}
			else {
				variable = doC_Function(function_name);
			}
		}
		else if(getToken(pc+1).type == "[") {
			Variable array = variables[getToken(pc).value];
			pc += 2;
			Variable index = doExpression();
			if(getToken(pc).type == "]") {
				try
				{
					variable = array.array_values[std::stoi(index.value)];
					pc++;
				}
				catch (const std::exception&)
				{
					std::cout << "Error on : " << getToken(pc-2).value<< getToken(pc-1).value << getToken(pc).value << std::endl;
					exit(0);
				}	
			}
		}
		else {
			variable = getVariable();
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
	else if(getToken(pc).type == "not"){
		pc++;
		variable = getExpression();
		if(variable.type == Variable::BOOL) {
			if(variable.value == "true") {
				variable.value = "false";
			}else {
				variable.value = "true";
			}
		}
	}
	else if(getToken(pc).value == "[") {
		pc++;
		if(getToken(pc).value == "]") {
			variable.type = Variable::ARRAY;
			variable.value = "test";
			pc++;
		} else {
			variable.array_values.push_back(doExpression());
			while(getToken(pc).value == ",") {
				pc++;
				variable.array_values.push_back(doExpression());
			}
			if(getToken(pc).value == "]") {
				variable.type = Variable::ARRAY;
				variable.value = std::to_string(variable.array_values.size());
				pc++;
			}
		}
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
				std::cout << "Error on : " << getToken(pc).value << std::endl;
				running = false;
			}
		} else if(stateMachine.state == StateMachine::STATEMENT) {
			doStatement();
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
		std::cout << "> " ;
		std::getline(std::cin, s);
		//printTokens(tokenizer.getTokens(s));
		interpret(tokenizer.getTokens(s));
	}
}
