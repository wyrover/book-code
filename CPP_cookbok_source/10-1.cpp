#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int main()
{
    ios_base::fmtflags flags = cout.flags();
    string first, last, citystate;
    int width = 20;
    first = "Richard";
    last  = "Stevens";
    citystate = "Tucson, AZ";
    cout << left                      // Left-justify in each field
         << setw(width) << first      // Then, repeatedly set the width
         << setw(width) << last       // and write some data
         << setw(width) << citystate << endl;
    cout.flags(flags);
}