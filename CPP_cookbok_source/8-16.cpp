#include <iostream>

using namespace std;

class Base
{
public:
    virtual void foo()
    {
        cout << "Base::foo( )" << endl;
    }
};

class Derived : public Base
{
public:
    virtual void foo()
    {
        cout << "Derived::foo( )" << endl;
    }
};

int main()
{
    Derived* p = new Derived();
    p->foo();        // Calls the derived version
    p->Base::foo();  // Calls the base version
}