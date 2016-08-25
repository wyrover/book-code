#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int main()
{
    ios_base::fmtflags flags =  // Save old flags
        cout.flags();
    double pi = 3.14285714;
    cout << "pi = " << setprecision(5)  // Normal (default) mode; only
         << pi << '\n';                 // show 5 digits, including both
    // sides of decimal point.
    cout << "pi = " << fixed            // Fixed-point mode;
         << showpos                     // show a "+" for positive nums,
         << setprecision(3)             // show 3 digits to the *right*
         << pi << '\n';                 // of the decimal.
    cout << "pi = " << scientific       // Scientific mode;
         << noshowpos                   // don't show plus sign anymore
         << pi * 1000 << '\n';
    cout.flags(flags);  // Set the flags to the way they were
}