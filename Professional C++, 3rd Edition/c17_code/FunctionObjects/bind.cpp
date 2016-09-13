// bind.cpp
#include <iostream>
#include <string>
#include <functional>

using namespace std;

void func(int num, const string& str)
{
    cout << "func(" << num << ", " << str << ")" << endl;
}

void overloaded(int /* num */) {}
void overloaded(float /* f */) {}

int main()
{
    // Bind second argument to a fixed value.
    string myString = "abc";
    auto f1 = bind(func, placeholders::_1, myString);
    f1(16);
    // rearrange arguments
    auto f2 = bind(func, placeholders::_2, placeholders::_1);
    f2("Test", 32);
    // Bind overloaded function
    //auto f3 = bind(overloaded, placeholders::_1);                 // ERROR
    auto f4 = bind((void(*)(float))overloaded, placeholders::_1);   // OK
    return 0;
}

