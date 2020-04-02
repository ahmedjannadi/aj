#include"variable.h"
#include<string>

Variable::Variable() {

}

Variable::Variable(std::string type, std::string value) {
	this->type = type;
	this->value = value;
}
