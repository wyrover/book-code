#include <iostream>

class Foo
{
public:
    Foo() {}

    Foo(const Foo&)
    {
        std::cout << "Foo(const Foo&)\n";
    }

    ~Foo()
    {
        std::cout << "~Foo()\n";
    }

    Foo& operator=(const Foo&)
    {
        std::cout << "operator=(const Foo&)\n";
        return *this;
    }
};

class Bar
{
public:
    Bar() {}

private:
    Foo f;
};

int main()
{
    std::cout << "Creating b1\n";
    Bar b1;
    std::cout << "Creating b2 as a copy of b1\n";
    Bar b2(b1);
    std::cout << "Assigning b1 to b2\n";
    b2 = b1;
}

