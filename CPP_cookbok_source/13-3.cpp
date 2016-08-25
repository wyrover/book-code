#include <iostream>
#include <fstream>
#include <locale>
#include <string>

using namespace std;

int main()
{
    try {
        locale loc("");
        cout << "Locale name = " << loc.name() << endl;
        locale locFr("french");
        locale locEn("english-american");
        locale locBr("portuguese-brazilian");
        cout.imbue(locFr); // Tell cout to use French formatting
        cout << "3.14 (French) = " << 3.14 << endl;
        cout << "Name = " << locFr.name() << endl;
        cout.imbue(locEn); // Now change to English (American)
        cout << "3.14 (English) = " << 3.14 << endl;
        cout << "Name = " << locEn.name() << endl;
        cout.imbue(locFr); // Tell cout to use Brazilian formatting
        cout << "3.14 (Brazil) = " << 3.14 << endl;
        cout << "Name = " << locBr.name() << endl;
    } catch (runtime_error& e) {
        // If you use an invalid locale name, a runtime_error exception
        // is thrown.
        cerr << "Error: " << e.what() << endl;
    }
}