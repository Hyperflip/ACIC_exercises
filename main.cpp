/*
Author: Philipp Andert

UE1 Feedback & TODO:
    * [DONE] LengthOf als auch GetLength hätten const sein können
    * [DONE] unnötige Kopie in concatenate (char[] wird erstellt und dann auf MyString kopiert)
    * [?] es könnte ein Speicherproblem im Zusammenhang mit Concatenate entstehen - RAII beachten
    * [?] warning: variable length array - ISO C++ forbids variable length array
    * [DONE] kein default Konstruktor

new to UE2:
    * MyString::MyString(Mystring* other);
    * MyString& MyString::=operator(MyString other);
    * new demo main routine

*/

#include <iostream>
#include <string.h>

class MyString {
private:
    char* arr;
    int length;

    // method for calculating the length of a char array
    const int lengthOf(const char* arr) const {
        /*
        count up and iterate over the char array
        until the terminator char ('\0') is hit,
        this is the length of the char array
        */
        int i = 0;
        for(i = 0; arr[i] != '\0'; i++);
        return i;
    }

public:
    MyString() : MyString(0) { }

    MyString(int length) {
        this->length = length;
        this->arr = new char[length + 1];
        this->arr[length] = '\0';
    }

    MyString(const char* arr) {
        /*
        +1 for the termination char, which
        will be copied over with strcpy
        */
        this->length = lengthOf(arr);
        /*
        the internal array needs to be allocated anew, else a
        manipulation of the original array that was passed will
        change the contents of the internal array
        */
        this->arr = new char[length + 1];
        strcpy(this->arr, arr);
        this->arr[length] = '\0';
    }

    // copy constructor: implementation using constructor delegation
    MyString(const MyString& other) : MyString(other.c_str()) { }

    /*
    destructor neccessary to clean up the
    char array that was allocated on the heap
    in the constructor
    */
    ~MyString() {
        delete[] this->arr;
    }

    operator const char*() const {
        return this->c_str();
    }

    // NOTE: "MyString&" as the object returned has to have the same address as the assignee
    MyString& operator=(const MyString& other) {
        // source: http://cplusplus.bordoon.com/copyConstructors.html
        
        if(this != &other) {
            // destroy this
            this->MyString::~MyString();
            // rebuild this from other using copy constructor
            new(this) MyString(other);
        }

        return *this;
    }

    /*
    returns by value, because Concatenate does as well,
    since they only manipulate the values of MyString
    */
    MyString operator+(const MyString& other) {
        MyString tmp = Concatenate(*this, other);
        return tmp;
    }

    MyString operator+(const char* arr) {
        return MyString::operator+(MyString(arr));
    }

    /*
    returns by reference, as it is critical to for left
    opperand to keep its address
    */
    MyString& operator+=(const MyString& other) {
        /*
        result of Concatenate needs to be assigned to this first and can't
        be returned directly, as the result will be returned by reference
        */
        *this = Concatenate(*this, other);
        return *this;
    }

    MyString& operator+=(const char* arr) {
        return MyString::operator+=(MyString(arr));
    }

    /*
    I decided for Concatenate() to be a static method that
    takes two MyStrings and returns another MyString.
    It seemed sensible to me to think of Concatenate() as a
    static utility function, as it does not alter any one MyString.
    */
    static MyString Concatenate(const MyString& str, const MyString& str2) {
        /*
        create a new empty MyString with length of str + str2
        */
        int length = str.GetLength() + str2.GetLength();
        MyString resultStr = MyString(length);

        // copy the first array into the new array
        strcpy(resultStr.arr, str.arr);

        /*
        the starting position for the appension of str2 equals
        the array's pointer + an offset (the length of the first str)
        */
        char* concatPos = resultStr.arr + str.GetLength();

        // copy str2 into new position
        strcpy(concatPos, str2.c_str());

        /*
        just in case append termination char at the end,
        even though strcpy should copy it as well.
        maybe it's required to implement strcpy ourselves
        in a future exercise ¯\_(ツ)_/¯, which is why
        I'm keeping it in
        */
        resultStr.arr[length] = '\0';

        /*
        expected procedure of this implementation:
        .......     -> empty resultStr->arr
      + Foo#        -> after first strcpy
      +    Bar#     -> after second strcpy
      +       #     -> appension of term char
      = FooBar#     -> final char array
        */

        return resultStr;
    }

    const int GetLength() const {
        return this->length;
    }

    const char* c_str() const {
        return this->arr;
    }
};

int main() {

    MyString s1 = MyString("a");
    MyString s2 = MyString("b");
    MyString s3 = MyString("c");

    std::cout << "s1: " << s1.c_str() << std::endl;
    std::cout << "s2: " << s2.c_str() << std::endl;
    std::cout << "s3: " << s3.c_str() << "\n" << std::endl;

    s3 += s2;
    std::cout << "s3 += s2: " << s3.c_str() << std::endl;

    s3 += "Hello";
    std::cout << "s3 += \"Hello\": " << s3.c_str() << "\n" << std::endl;

    MyString s4 = s1 + s2;
    std::cout << "MyString s4 = s1 + s2: " << s4.c_str() << std::endl;
    
    s4 = s4 + "World";
    std::cout << "s4 = s4 + \"World\": " << s4.c_str() << "\n" << std::endl;

    std::cout << "puts(s4): ";
    puts(s4);

    return 0;
}