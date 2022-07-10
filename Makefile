FLAGS=-Wall -Wextra -pedantic -g
SRC=./src

mop: $(SRC)/mop.cpp $(SRC)/lexer.cpp $(SRC)/parser.cpp $(SRC)/astnode.cpp $(SRC)/visitor.cpp 
	mkdir -p build
	$(CXX) $(FLAGS) $^ -o build/$@