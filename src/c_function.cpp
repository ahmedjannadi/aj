#include"headers/c_function.h"

C_Function::C_Function() {

}

C_Function::C_Function(int (*func)()) {
	type = Variable::C_FUNCTION;
	value = "C_FUNCTION";
	function = func;
}
