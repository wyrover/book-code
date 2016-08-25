#include <iostream>
#include <string>

using namespace std;

// ManipInfra is a small, intermediary class that serves as a utility
// for custom manipulators with arguments. Call its constructor with a
// function pointer and a value from your main manipulator function.
// The function pointer should be a helper function that does the
// actual work. See examples below.
template<typename T, typename C>
class ManipInfra
{

public:
    ManipInfra(basic_ostream<C>& (*pFun)
               (basic_ostream<C>&, T), T val)
        : manipFun_(pFun), val_(val) {}
    void operator()(basic_ostream<C>& os) const
    {
        manipFun_(os, val_);   // Invoke the function pointer with the
    }
private:                      // stream and value
    T val_;
    basic_ostream<C>& (*manipFun_)
    (basic_ostream<C>&, T);
};

template<typename T, typename C>
basic_ostream<C>& operator<<(basic_ostream<C>& os,
                             const ManipInfra<T, C>& manip)
{
    manip(os);
    return (os);
}

// Helper function that is ultimately called by the ManipInfra class
ostream& setTheWidth(ostream& os, int n)
{
    os.width(n);
    return (os);
}

// Manipulator function itself.  This is what is used by client
// code
ManipInfra<int, char> setWidth(int n)
{
    return (ManipInfra<int, char>(setTheWidth, n));
}

// Another helper that takes a char argument
ostream& setTheFillChar(ostream& os, char c)
{
    os.fill(c);
    return (os);
}

ManipInfra<char, char> setFill(char c)
{
    return (ManipInfra<char, char>(setTheFillChar, c));
}

int main()
{
    cout << setFill('-')
         << setWidth(10) << right << "Proust\n";
}