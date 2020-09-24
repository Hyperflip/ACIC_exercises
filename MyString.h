class MyString {
private:
    char* arr;
    int length;
    // method for calculating the length of a char array
    static int lengthOf(const char* arr);
public:
    MyString(const char* arr);
    /*
    destructor neccessary to clean up the
    char array that was allocated on the heap
    in the constructor
    */
    ~MyString();

    /*
    I decided for Concatenate() to be a static method that
    takes two MyStrings and returns another new MyString.
    It seemed sensible to me to think of Concatenate() as a
    utility function, as it does not alter any one MyString.
    */
    static MyString* Concatenate(MyString* str, MyString* str2);
    // overloaded signature for Concatenate? Best practice?

    int GetLength();
    const char* c_str();
};