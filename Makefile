all: exercise1

exercise1: main.o mystring.o
		g++ -std=c++17 *.o -o exercise1

main.o: main.cpp
		g++ -std=c++17 -c main.cpp

mystring.o: MyString.h MyString.cpp
		g++ -std=c++17 -c MyString.cpp

run: all
		./exercise1

clean: rm -rf exercise1