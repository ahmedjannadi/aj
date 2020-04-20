a:
	g++ -std=c++17 src/*.cpp -o aj
shared:
	g++ -shared -fPIC -std=c++17 src/interpreter.cpp src/variable.cpp src/tokenizer.cpp src/token.cpp src/aj.cpp src/statemachine.cpp  -o libs/aj.so
test:
	g++ test.cpp src/tokenizer.cpp src/token.cpp
