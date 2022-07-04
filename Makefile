FLAGS=-Wall -Wextra -pedantic
SRC=./src

mop: $(SRC)/mop.cpp
	mkdir -p build
	$(CXX) $(FLAGS) $^ -o build/$@