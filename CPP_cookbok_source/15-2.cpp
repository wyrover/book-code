#include <iostream>
#include <string>

class MyClass
{

public:
    MyClass() : ival_(0), sval_("foo") {}
    ~MyClass() {}

    void incr()
    {
        ++ival_;
    }
    void decr()
    {
        ival_--;
    }

private:
    std::string sval_;
    int ival_;
};

int main()
{
    MyClass obj;
    int         MyClass::* mpi = &MyClass::ival_;  // Data member
    std::string MyClass::* mps = &MyClass::sval_;  // pointers
    void (MyClass::*mpf)();  // A pointer to a member function that
    // takes no params and returns void
    void (*pf)();            // A normal function pointer
    int* pi = &obj.ival_;   // int pointer referring to int member--no
    // problem.
    mpf = &MyClass::incr;   // A pointer to a member function. You can't
    // write this value to a stream. Look at it
    // in your debugger to see what its
    // representation looks like.
    pf = &MyClass::incr;    // Error: &MyClass::incr is not an instance
    // of a function
    std::cout << "mpi = " << mpi << '\n';
    std::cout << "mps = " << mps << '\n';
    std::cout << "pi =  " << pi << '\n';
    std::cout << "*pi = " << *pi << '\n';
    obj.*mpi = 5;
    obj.*mps = "bar";
    (obj.*mpf)();  // now obj.ival_ is 6
    std::cout << "obj.ival_ = " << obj.ival_ << '\n';
    std::cout << "obj.sval_ = " << obj.sval_ << '\n';
}