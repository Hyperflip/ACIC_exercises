all: exercise1

exercise1: main.o mystring.o
		g++ -std=c++17 -fsanitize=leak *.o -o exercise1

main.o: main.cpp
		g++ -std=c++17 -fsanitize=leak -c main.cpp

mystring.o: MyString.h MyString.cpp
		g++ -std=c++17 -fsanitize=leak -c MyString.cpp

run: all
		./exercise1

clean:
		rm *.o exercise1 -f