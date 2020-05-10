a:
	g++ -std=c++17 src/*.cpp -O3 -o aj
shared:
	g++ -O3 -shared -fPIC -std=c++17 src/interpreter.cpp src/variable.cpp src/tokenizer.cpp src/token.cpp src/aj.cpp src/ajhelper.cpp src/statemachine.cpp  -o libs/aj.so
test:
	g++ test.cpp src/tokenizer.cpp src/token.cpp
