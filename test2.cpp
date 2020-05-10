#include "test2.h"

//Aj* aj;

void TEST2_init(Aj *aj) {
	aj->interpreter.setC_Function("test",TEST2_test);
}

int TEST2_test() {
	std::cout << "test" << std::endl;
	return 0;
}
