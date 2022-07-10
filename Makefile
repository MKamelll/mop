FLAGS=-Wall -Wextra -pedantic -g
SRC=./src

mop: $(SRC)/*.cpp
	mkdir -p build
	$(CXX) $(FLAGS) $^ -o build/$@