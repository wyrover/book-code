#include <iostream>
#include <locale>
#include <string>

using namespace std;

// There is a global locale in the background that is set up by the
// runtime environment. It is the "C" locale by default. You can
// replace it with locale::global(const locale&).
int main()
{
    locale loc(""); // Create a copy of the user's locale
    cout << "Locale name = " << loc.name() << endl;
    cout.imbue(loc); // Tell cout to use the formatting of
    // the user's locale
    cout << "pi in locale " << cout.getloc().name() << " is "
         << 3.14 << endl;
}