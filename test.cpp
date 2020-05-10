#include <iostream>
#include "test2.h"
#include "src/headers/aj.h"

int main(int argc, char** argv) {
	Aj aj;
	TEST2_init(&aj);

	aj.doCommand("test");
}
