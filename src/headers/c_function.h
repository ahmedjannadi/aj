#pragma once
#include"variable.h"

class C_Function : Variable {
	C_Function();
	C_Function(int (*func)());
	int (*function)();
};
