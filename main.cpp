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
    
    MyString* str3 = MyString::Concatenate(str, str2);

    std::cout << "str3 " << str3->c_str() << std::endl;
    std::cout << "length of str3: " << str3->GetLength() << std::endl;

    delete str;
    delete str2;
    delete str3;

    return 0;
}