/*
Author: Philipp Andert
*/

#include <iostream>
#include "MyString.h"

int main() {

    MyString* str = new MyString("Foo");
    MyString* str2 = new MyString("Bar");

    std::cout << "str: " << str->c_str() << std::endl;
    std::cout << "str2: " << str2->c_str() << std::endl;
    
    str->Concatenate(str2);

    std::cout << "str after concatenation: " << str->c_str() << std::endl;
    std::cout << "length of str: " << str->GetLength() << std::endl;

    delete str;
    delete str2;

    return 0;
}