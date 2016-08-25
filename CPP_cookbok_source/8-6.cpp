#include <iostream>
#include <typeinfo>

using namespace std;

class Base {};
class Derived : public Base {};

int main()
{
    Base b, bb;
    Derived d;

    // Use typeid to test type equality
    if (typeid(b) == typeid(d)) { // No
        cout << "b and d are of the same type.\n";
    }

    if (typeid(b) == typeid(bb)) { // Yes
        cout << "b and bb are of the same type.\n";
    }

    if (typeid(d) == typeid(Derived)) { // Yes
        cout << "d is of type Derived.\n";
    }
}