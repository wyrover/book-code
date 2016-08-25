#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

class MyClass
{
protected:
    int value_;
public:
    static list<MyClass*> instances_;
    MyClass(int val);
    ~MyClass();
    static void showList();
};

list<MyClass*> MyClass::instances_;

MyClass::MyClass(int val)
{
    instances_.push_back(this);
    value_ = val;
}

MyClass::~MyClass()
{
    list<MyClass*>::iterator p =
        find(instances_.begin(), instances_.end(), this);

    if (p != instances_.end())
        instances_.erase(p);
}

void MyClass::showList()
{
    for (list<MyClass*>::iterator p = instances_.begin();
         p != instances_.end(); ++p)
        cout << (*p)->value_ << endl;
}

int main()
{
    MyClass a(1);
    MyClass b(10);
    MyClass c(100);
    MyClass::showList();
}