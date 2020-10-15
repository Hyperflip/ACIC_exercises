/*
Author: Philipp Andert
*/

#include <iostream>
#include <string.h>

class MyString {
private:
    char* arr;
    int length;

    // method for calculating the length of a char array
    static int lengthOf(const char* arr) {
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

    /*
    destructor neccessary to clean up the
    char array that was allocated on the heap
    in the constructor
    */
    ~MyString() {
        delete[] this->arr;
    }

    /*
    I decided for Concatenate() to be a static method that
    takes two MyStrings and returns another new MyString.
    It seemed sensible to me to think of Concatenate() as a
    utility function, as it does not alter any one MyString.
    */
    static MyString* Concatenate(MyString* str, MyString* str2) {
        /*
        the array resulting from concatenation does NOT
        need to be created on the HEAP, as it will be
        passed to the scope of the new MyString object,
        which will be returned. in the constructor of MyString,
        a new char array will be alocated.
        */
        int length = str->GetLength() + str2->GetLength();
        char resultArr[length + 1];

        // copy the first array into resultArr
        strcpy(resultArr, str->arr);

        /*
        the starting position for the appension of str2 equals
        the array's pointer + an offset (the length of the first str)
        */
        char* concatPos = resultArr + str->GetLength();

        // copy str2 into new position
        strcpy(concatPos, str2->c_str());

        /*
        just in case append termination char at the end,
        even though strcpy should copy it as well.
        maybe it's required to implement strcpy ourselves
        in a future exercise ¯\_(ツ)_/¯, which is why
        I'm keeping it in
        */
        resultArr[length] = '\0';

        /*
        expected procedure of this implementation:

        .......     -> empty resultArr
      + Foo#        -> after first strcpy
      +    Bar#     -> after second strcpy
      +       #     -> appension of term char
      = FooBar#     -> final char array
        */

        // return a new MyString
        MyString* resultString = new MyString(resultArr);
        return resultString;
    }

    int GetLength() {
        return this->length;
    }

    const char* c_str() {
        return this->arr;
    }
};

int main() {

    MyString* str = new MyString("Foo");
    MyString* str2 = new MyString("Bar");

    std::cout << "str: " << str->c_str() << std::endl;
    std::cout << "str2: " << str2->c_str() << std::endl;
    
    MyString* str3 = MyString::Concatenate(str, str2);

    std::cout << "str3: " << str3->c_str() << std::endl;
    std::cout << "length of str3: " << str3->GetLength() << std::endl;

    delete str;
    delete str2;
    delete str3;

    return 0;
}