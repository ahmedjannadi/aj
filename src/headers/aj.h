#pragma once
#include"interpreter.h"

class Aj {
public:
	Aj();
	Interpreter interpreter;

	void doCommand(std::string command);
	void doFile(std::string filename);
	void doFunction(std::string command);
	int getInteger(std::string variable);
	int getFloat(std::string variable);
	int getString(std::string variable);
};
