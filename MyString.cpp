#include "MyString.h"
#include <iostream>
#include <string.h>

int MyString::lengthOf(const char* arr) {
    /*
    count up and iterate over the char array
    until the terminator char ('\0') is hit,
    this is the length of the char array
    */
    int i = 0;
    for(i = 0; arr[i] != '\0'; i++);
    return i;
}

MyString::MyString(const char* arr) {
    this->arr = arr;
    this->length = lengthOf(arr);
}

MyString* MyString::Concatenate(MyString* str, MyString* str2) {
    /*
    the array resulting from concatenation does NOT
    need to be created on the HEAP, as it will be
    passed to the scope of the new MyString object,
    which will be returned. this new object has to be
    created on the heap, as it would otherwise be destroyed
    once the function terminates
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
    resultString->length = length;
    return resultString;
}

int MyString::GetLength() {
    return this->length;
}

const char* MyString::c_str() {
    return this->arr;
}