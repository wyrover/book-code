//
//  Cxx1xExamples.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "Cxx1xExamples.h"

#include <iostream>

#include <vector>
#include <map>

using std::cout;
using std::string;
using std::endl;
using std::vector;
using std::map;

void autoExample()
{
    auto i = 1;          // this is an integer
    auto d = 2.0;        // this is a float
    auto d2 = d + 1;     // this is also a float
    auto str = "hello";  // this is a char *

    cout << "integer : " << i << " float: "  << d2 << " string " << str << endl;
}

void autoTemporaryExamp()
{
    std::vector<std::pair<int,std::string>> myVector;

    // without auto
    for (std::vector<std::pair<int,std::string>>::iterator it = myVector.begin();
         it != myVector.end(); ++it)
    {
        // do something here
    }

    // with auto
    for (auto it = myVector.begin(); it != myVector.end(); ++it)
    {
        // same thing here
    }
}

void autoFunctExample1(vector<int> &x)
{
    auto iterator = x.begin();
    // do something with iterator
}

auto autoFuncExample(vector<int> &x) -> decltype(x.begin())
{
    auto iterator = x.begin();
    // do something with iterator
    return iterator;
}

///


void testDecl()
{
    vector<map<string, int>> myVector;
}

////



class TestDefaults {
public:
    TestDefaults() = default;
    TestDefaults(int arg);
    virtual ~TestDefaults() = default;
    TestDefaults(const TestDefaults &) = delete;

    // other member functions here
};

#pragma mark Lambda


struct [[exported]] AttribSample
{
    int memberA;
    [[gnu::aligned (16)]]
    double memberB;
};

/// Lamda

void lambdaExample()
{
    auto avg = [](int a, int b) { return (a + b) / 2; };

    cout << "the average of 3 and 5 is "  << avg(3, 5) << endl;
}

void lambdaExample2()
{
    double factor = 2.5;
    auto scaledAvg = [&factor](int a, int b) { return factor * (a + b) / 2; };

    auto modifiedAvg = [&](int a, int b) { return scaledAvg(a,b); };

    cout << "the scaled average of 3 and 5 is "  << scaledAvg(3, 5) << endl;
    cout << "this should be the same "  << modifiedAvg(3, 5) << endl;
}

long double operator "" _eu(long double val)
{
    return val / 1.24;
}

void useUserDefinedLiterals()
{
    double price = 300; // price in dollars
    long double priceInEU =  300.0_eu;

    cout << " price in dollars: "  << price
    << " price in Euros: " << priceInEU << endl;
 }


// ----

void loopExample1()
{
    std::vector<std::pair<double,std::string>> v;

    // without auto
    for (std::vector<std::pair<double,std::string>>::iterator it = v.begin();
         it != v.end(); ++it)
    {
        // do something here
    }
}

void forLoopExample()
{
    std::vector<std::pair<double,std::string>> vectorOfPairs;

    for (auto &i : vectorOfPairs)
    {
        cout << " values are "
             << i.first << " and "
             << i.second << endl;
    }
}

//vector<int> && usingRValue(std::vector<int> &&r)
//{
//    vector<int> vi;
//    vi.push_back(2);
//    return vector<int>();
//}


class RValTest {
public:
    RValTest(int n);
    RValTest(const RValTest &x);
    ~RValTest();

    RValTest &operator=(RValTest &&p);  // this is for RVAL
    RValTest &operator=(RValTest &p);   // this is for LVAL
private:
    vector<int> data;
};


RValTest::RValTest(int n)
: data(n, 0)
{

}

RValTest::RValTest(const RValTest &p)
: data(p.data)
{
}

RValTest::~RValTest()
{
}

RValTest &RValTest::operator=(RValTest &&p)
{
    data.swap(p.data);
    cout << " calling rval assignment " << endl;
    return *this;
}

RValTest &RValTest::operator=(RValTest &p)
{
    if (this != &p)
    {
        data = p.data;
    }
    cout << " calling normal assignment " << endl;
    return *this;
}

void useRValTest()
{
    RValTest test(3);
    RValTest test2(4);

    test2 = test;  // use standard assignment
    test = RValTest(5);  // use rval assignment
}

void rvalExamp(string &&s)
{
    cout << " string is " << s << endl;
}

void rvalExamp(string &s)
{
    cout << " string lvalue: " << s << endl;
}

/// ---

class Dimensions {
public:
    Dimensions();
    Dimensions(double x);
    Dimensions(double x, double y);
    Dimensions(double x, double y, double z);

private:
    double m_x;
    double m_y;
    double m_z;
};

Dimensions::Dimensions()
: Dimensions(0, 0, 0)
{
}

Dimensions::Dimensions(double x)
: Dimensions(x, 0, 0)
{
}

Dimensions::Dimensions(double x, double y)
: Dimensions(x, y, 0)
{
}

Dimensions::Dimensions(double x, double y, double z)
: m_x(x),
  m_y(y),
  m_z(z)
{
}

/// ---- inheriting constructors

class Dimensions4 : public Dimensions {
public:
    using Dimensions::Dimensions;

};


void main_test()
{
    Dimensions4(1, 2, 4);
}

//// ------

struct TestStruct {
    int a;
    char b;
    double c;
};

template <class T>
constexpr int testDataSize(T)
{
    return sizeof(T);
}

constexpr int minTestSize()
{
    return 2 * testDataSize(TestStruct()) + 1;
}

///---


void *testNull()
{
    int *pi = new int;
    if (pi == nullptr)
    {
        return nullptr;
    }
    // *pi = 1 + nullptr; // this doesn't work, nullptr is not an integer
    return pi;
}

/// ----

void initializationTest()
{
    int x {}; // equivalent to int x = 0;
    int y { 0 };  // same as above
    const char *s { "var"  };
    double d { 2.4 };
    struct StrTest {
        int a;
        double d;
        char c;
    };

    StrTest structVal { 2, 4.2, 'f' };

    cout << " values are "  << x << "  " << y << " "  << s
        << " "  << d << " " << structVal.a << endl;


    class AClass {
    public:
        AClass(int v) : m_val(v) {}
        int m_val;
    };

    AClass obj = { 3 } ;
}

void containerInitialization()
{
    vector<int> vi = { 1, 3, 5, 7, 9, 11 };

    for (auto &v : vi)
    {
        cout << v << " ";
    }

    map<int,double> m = { { 2, 3.0}, {4, 5.0} };
    for (auto &v : m)
    {
        cout << v.first << " " << v.second << " ";
    }
}

class MyClass {
public:
    MyClass(std::initializer_list<int> args);
    vector<int> m_vector;
};

MyClass::MyClass(std::initializer_list<int> args)
{
    m_vector.insert(m_vector.begin(), args.begin(), args.end());
}

void useClassInitializer()
{
    MyClass myClass = { 2, 5, 6, 22, 34, 25 };

    for (auto &v : myClass.m_vector)
    {
        cout << v << " ";
    }
}

//

void testRVal()
{
    rvalExamp("a test string");
    string a = "string a ";
    string b = "string b ";
    rvalExamp(a + b);
    string c = "another example";
    rvalExamp(c);
    useRValTest();
}