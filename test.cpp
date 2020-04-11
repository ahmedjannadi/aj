#include<fstream>
#include<iostream>
#include<map>
#include<vector>
#include<stack>
#include"variable.h"
//#include"src/headers/tokenizer.h"


void read_file(const char* filename) {
	
}

std::map<std::string, int (*)()> c_functions;
std::stack<std::string> st;

void aj_c_function(std::string name, int (*fun)()) {
	c_functions.insert_or_assign(name, fun);
}

int aj_do_function(std::string name) {
	//int (*fun)();
	return c_functions[name]();
}

signed int test() {
	std::cout << st.top() << std::endl;
	st.pop();
	std::cout << "did test" << std::endl;

	return 0;
}

int main(int argc, char** argv) {

	aj_c_function("test",test);
	st.push("10");
	aj_do_function("test");

	/*std::map<std::string, Variable> variables;
	std::vector<Variable> vars;
	vars.push_back(Variable("hello","hello"));

	variables.insert_or_assign("a",Variable("NULL","variables"));

	std::cout << variables["a"].value << std::endl;
	std::cout << vars[0].value << std::endl;
	*/
}
