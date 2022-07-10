FLAGS=-Wall -Wextra -pedantic -g
SRC=./src

mop: $(SRC)/mop.cpp $(SRC)/lexer.cpp
	mkdir -p build
	$(CXX) $(FLAGS) $^ -o build/$@