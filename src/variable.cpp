#include"headers/variable.h"
#include<string>

Variable::Variable() {

}

Variable::Variable(Type type, std::string value) {
	this->type = type;
	this->value = value;
}
