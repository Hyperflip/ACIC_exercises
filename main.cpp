/*
Author: Philipp Andert

General TODO:
* clean up comments regarding decisions from previous exercises (?)

UE1 Feedback & TODO:
    * [DONE] LengthOf als auch GetLength hätten const sein können
    * [DONE] unnötige Kopie in concatenate (char[] wird erstellt und dann auf MyString kopiert)
    * [?] es könnte ein Speicherproblem im Zusammenhang mit Concatenate entstehen - RAII beachten
    * [?] warning: variable length array - ISO C++ forbids variable length array
    * [DONE] kein default Konstruktor
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
    MyString() {
        this->length = 0;
    }

    MyString(int length) {
        this->length = length;
        this->arr = new char[length + 1];
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
    }

    // copy constructor: implementation using constructor delegation
    MyString(MyString* other) : MyString(other->c_str()) { }

    /*
    destructor neccessary to clean up the
    char array that was allocated on the heap
    in the constructor
    */
    ~MyString() {
        delete[] this->arr;
    }

    MyString& operator=(MyString other) {
        MyString temp(&other);
        std::swap(*this, temp);
        return *this;
    }

    /*
    I decided for Concatenate() to be a static method that
    takes two MyStrings and returns another new MyString.
    It seemed sensible to me to think of Concatenate() as a
    utility function, as it does not alter any one MyString.
    */
    static MyString* Concatenate(MyString* str, MyString* str2) {
        /*
        create a new empty MyString with length of str + str2
        */
        int length = str->GetLength() + str2->GetLength();
        MyString* resultStr = new MyString(length);

        // copy the first array into the new array
        strcpy(resultStr->arr, str->arr);

        /*
        the starting position for the appension of str2 equals
        the array's pointer + an offset (the length of the first str)
        */
        char* concatPos = resultStr->arr + str->GetLength();

        // copy str2 into new position
        strcpy(concatPos, str2->c_str());

        /*
        just in case append termination char at the end,
        even though strcpy should copy it as well.
        maybe it's required to implement strcpy ourselves
        in a future exercise ¯\_(ツ)_/¯, which is why
        I'm keeping it in
        */
        resultStr->arr[length] = '\0';

        /*
        expected procedure of this implementation:

        .......     -> empty resultStr->arr
      + Foo#        -> after first strcpy
      +    Bar#     -> after second strcpy
      +       #     -> appension of term char
      = FooBar#     -> final char array
        */

        // return a new MyString
        return resultStr;
    }

    const int GetLength() const {
        return this->length;
    }

    const char* c_str() {
        return this->arr;
    }
};

int main() {

    MyString* str1 = new MyString("Foo");
    MyString* str2 = new MyString("Bar");

    std::cout << "str1: " << str1->c_str() << std::endl;
    std::cout << "str2: " << str2->c_str() << std::endl;

    MyString* str3 = MyString::Concatenate(str1, str2);

    std::cout << "str3: " << str3->c_str() << std::endl;

    delete str1;
    delete str2;
    delete str3;

    return 0;
}