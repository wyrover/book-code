#include <iostream>
#include <typeinfo>

using namespace std;

class Base
{
public:
    virtual ~Base() {}  // Make this a polymorphic class
};
class Derived : public Base
{
public:
    virtual ~Derived() {}
};

int main()
{
    Derived d;

    // Query the type relationship
    if (dynamic_cast<Base*>(&d)) {
        cout << "Derived is a subclass of Base" << endl;
    } else {
        cout << "Derived is NOT a subclass of Base" << endl;
    }
}