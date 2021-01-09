/*
Author: Philipp Andert
*/

#include <iostream>
#include <memory>
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
    // deleter reference: https://en.cppreference.com/w/cpp/memory/unique_ptr

    struct Deleter {
        void operator()(MyString* str) {
            std::cout << "using Deleter of MyString" << std::endl;
            // this calls ~MyString()
            delete str;
        }
    };

    class Iterator{
    public:
        /*
        current reference to a character in the parent MyString,
        this is a pointer, so it can easily be incremented in operator++
        */
        char* pos;

        // Iterators can be initiliazed at any position
        Iterator(char* pos) {
            this->pos = pos;
        }
        
        // increment to the next char in memory
        Iterator& operator++() {
            this->pos++;
            return *this;
        }

        // compare two Iterators; done by comparing both Iterator's char references
        bool operator==(const Iterator& other) const {
            return this->pos == other.pos;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        // dereferencing Iterators yields the current char that the Iterator references
        char operator*() {
            return *this->pos;
        }

    };

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
    returns by value, because nothing gets assigned and a new
    value (a new char array) is calculated
    */
    MyString operator+(const MyString& other) {
        return Concatenate(*this, other);
    }

    MyString operator+(const char* arr) {
        return MyString::operator+(MyString(arr));
    }

    /*
    returns by reference, since the value is assigned to the left
    opperand
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

    // returns the first char of the parent MyString
    Iterator begin() {
        Iterator it = Iterator(&this->arr[0]);
        return it;
    }

    // return the past-the-end character of the parent MyString (terminator char)
    Iterator end() {
        Iterator it = Iterator(&this->arr[this->GetLength()]);
        return it;
    }
};

template <class T, class D = std::default_delete<T>> class MyUniquePtr {
private:
    T* data;
    D deleter;

public:
// rule of five reference: https://cpppatterns.com/patterns/rule-of-five.html
// unique_ptr reference: https://en.cppreference.com/w/cpp/memory/unique_ptr

    // constructor
    MyUniquePtr(T* value) : data(value) {}
    // overloaded constructor with deleter
    MyUniquePtr(T* value, const D& del) : data(value), deleter(del) {}
    // copy constructor
    MyUniquePtr(const MyUniquePtr& other) = delete;
    // copy assignment constructor
    MyUniquePtr& operator=(const MyUniquePtr& other) = delete;
    // move constructor
    MyUniquePtr(MyUniquePtr&& other) {
        data = other.data;
        other.data = nullptr;
    }
    // move assignment constructor
    MyUniquePtr& operator=(MyUniquePtr&& other) {
        if(&other != this) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }

        return *this;
    }
    // destructor
    ~MyUniquePtr() {
        delete data;
    }

    T* operator-> () const {
        return data;
    }

    T operator* () const {
        return *data;
    }

    operator bool() const {
        return data != NULL;
    }

    T* Release() {
        T* temp = data;
        // DON'T delete data, as it will be used elsewhere
        data = nullptr;
        return temp;
    }

    // reset as per https://en.cppreference.com/w/cpp/memory/unique_ptr/reset
    // different than moodle assignment
    void Reset(T* data = nullptr) {
        deleter(this->data);
        this->data = data;
    }

    // swap as per https://en.cppreference.com/w/cpp/memory/unique_ptr/swap2
    // different than moodle assignment
    void Swap(MyUniquePtr& other) {
        T* temp = this->data;
        this->data = other.data;
        other.data = temp;
    }

};

struct Entity {
    int id;

    Entity(int val) : id(val) {};
    Entity() {};
};

int main() {

    // constructor
    MyUniquePtr<Entity> entityPointer1(new Entity(10));
    MyUniquePtr<Entity> entityPointer2(new Entity(20));
    MyUniquePtr<Entity> entityPointer3(new Entity(30));
    // copy constructor is a deleted method, which is the intended behaviour:
    //MyUniquePtr<Entity> entityPointer2(entityPointer);
    // same with copy assignment constructor
    //MyUniquePtr<Entity> entityPointer2 = entityPointer;
    // move constructor
    MyUniquePtr<Entity> moveConstructed(std::move(entityPointer2));
    std::cout << "(from entityPointer2) moveConstructed->id: " << moveConstructed->id << std::endl;
    std::cout << "(bool)entitiyPointer2: " << (bool)entityPointer2 << std::endl;
    // move assignment constructor
    MyUniquePtr<Entity> moveAssignConstructed = std::move(entityPointer3);
    std::cout << "(from entityPointer3) moveAssignConstructed->id: " << moveAssignConstructed->id << std::endl;
    std::cout << "(bool)entityPointer3: " << (bool)entityPointer3 << std::endl;

    std::cout << std::endl;

    std::cout << "entityPointer1->id: " << entityPointer1->id << std::endl;
    std::cout << "(*entityPointer1).id: " << (*entityPointer1).id << std::endl;
    std::cout << "(bool)entityPointer1: " << (bool)entityPointer1 << std::endl;

    std::cout << std::endl;

    Entity* entity = entityPointer1.Release();
    std::cout << "Entity* entity = entityPointer1.Release();\n"
        "entity->id: " << entity->id << "\n"
        "(bool)entityPointer1: " << (bool)entityPointer1 << std::endl;
    delete entity;

    entityPointer2.Reset(new Entity(25));
    std::cout << "entityPointer2.Reset(new Entity(25));\n"
        "entityPointer2->id: " << entityPointer2->id << std::endl;
    entityPointer3.Reset(new Entity(35));
    std::cout << "entityPointer3.Reset(new Entity(35));" << std::endl;

    entityPointer2.Swap(entityPointer3);
    std::cout << "entityPointer2.Swap(entityPointer3);\n"
        "entityPointer2->id: " << entityPointer2->id << "\n" <<
        "entityPointer3->id: " << entityPointer3->id << std::endl;

    // deleter demo
    MyUniquePtr<MyString, MyString::Deleter> myStringPtr(new MyString("hello"));
    std::cout << "myStringPtr->c_str(): " << myStringPtr->c_str() << std::endl;
    std::cout << "myStringPtr.Reset(): ";
    myStringPtr.Reset();
    return 0;
}
