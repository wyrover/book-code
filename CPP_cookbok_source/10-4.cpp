#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// make floating-point output look normal
inline ios_base& floatnormal(ios_base& io)
{
    io.setf(0, ios_base::floatfield);
    return (io);
}
int main()
{
    ios_base::fmtflags flags =  // Save old flags
        cout.flags();
    double pi = 22.0 / 7.0;
    cout << "pi = " << scientific   // Scientific mode
         << pi * 1000 << '\n';
    cout << "pi = " << floatnormal
         << pi << '\n';
    cout.flags(flags);
}