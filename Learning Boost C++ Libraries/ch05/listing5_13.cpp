#include <iostream>
#include <boost/ptr_container/ptr_vector.hpp>

struct ConcreteBase {
    virtual void doWork() {}
};

struct Derived1 : public ConcreteBase {
    Derived1(int n) : data(n) {}
    void doWork() override
    {
        std::cout << data << "\n";
    }
    int data;
};

struct Derived2 : public ConcreteBase {
    Derived2(int n) : data(n) {}
    void doWork() override
    {
        std::cout << data << "\n";
    }
    int data;
};

int main()
{
    boost::ptr_vector<ConcreteBase> vec;
    typedef boost::ptr_vector<ConcreteBase>::iterator iter_t;
    vec.push_back(new Derived1(1));
    vec.push_back(new Derived2(2));

    for (iter_t it = vec.begin(); it != vec.end(); ++it) {
        ConcreteBase obj = *it;
        obj.doWork();
    }
}

