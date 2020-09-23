class MyString {
private:
    char* arr;
    int length;
    static int lengthOf(const char* arr);
public:
    MyString(const char* arr);
    /*
    custom destructor not neccessary, as
    default destructor calls delete on ALL
    member variables and functions when
    delete is called on the object.
    as this->arr only stores primitives
    (the characters), no further deletion
    is needed in a custom destructor
    */
    
    void Concatenate(MyString* str2);
    void Concatenate(MyString str2);
    int GetLength();
    const char* c_str();
};