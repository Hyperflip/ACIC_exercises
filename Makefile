all: exercise

exercise: main.o
		clang -lstdc++ -std=c++17 -fsanitize=leak main.o -o exercise

main.o: main.cpp
		clang -std=c++17 -fsanitize=leak -c main.cpp

run: all
		./exercise

clean:
		rm *.o exercise -f