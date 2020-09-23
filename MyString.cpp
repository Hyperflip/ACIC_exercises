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
    this->length = lengthOf(arr);
    /*
    the array needs to be created on the heap,
    because Concatenate() will replace the array
    with an array created on the heap.
    this way, 'delete array' in Concatenate() will always
    safely delete the previous array before replacing it
    */
    this->arr = new char[this->length];
    strcpy(this->arr, arr);
}
/*
concatenate appends another string to the original string
*/
void MyString::Concatenate(MyString* str2) {
    /*
    create a new array ON THE HEAP with a
    length of the sum of both string's lengths + 1
    (+1 for the termination char).
    the array needs to be created on the heap, else it
    will be destroyed once the function exits
    */
    int length = this->GetLength() + str2->GetLength();
    char* resultArr = new char[length + 1];

    // copy the first array into resultArr
    strcpy(resultArr, this->arr);

    /*
    the starting position for the appension of str2 equals
    the array's pointer + an offset (the length of the first str)
    */
    char* concatPos = resultArr + this->GetLength();

    // copy str2 into new position
    strcpy(concatPos, str2->c_str());

    // append termination char at the end
    resultArr[length] = '\0';

    // delete the previous array
    delete this->arr;
    // assign new array
    this->arr = resultArr;
    // update the length
    this->length = lengthOf(this->arr);
}

/*
overloaded signature for passing instances of MyString
instead of references
*/
void MyString::Concatenate(MyString str2) {
    this->Concatenate(&str2);
}

int MyString::GetLength() {
    return this->length;
}

const char* MyString::c_str() {
    return this->arr;
}