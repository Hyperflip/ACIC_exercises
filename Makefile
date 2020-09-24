all: exercise1

exercise1: main.o mystring.o
		g++ -std=c++17 -fsanitize=address *.o -o exercise1

main.o: main.cpp
		g++ -std=c++17 -fsanitize=address -c main.cpp

mystring.o: MyString.h MyString.cpp
		g++ -std=c++17 -fsanitize=address -c MyString.cpp

run: all
		./exercise1

clean:
		rm *.o exercise1 -f