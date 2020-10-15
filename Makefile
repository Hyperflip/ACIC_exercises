all: exercise2

exercise2: main.o
		g++ -std=c++17 -fsanitize=leak main.o -o exercise2

main.o: main.cpp
		g++ -std=c++17 -fsanitize=leak -c main.cpp

run: all
		./exercise2

clean:
		rm *.o exercise2 -f