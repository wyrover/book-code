#include <string>
#include <iostream>

using namespace std;

int main()
{
    string s = "";
    string sr = "";
    sr.reserve(9000);
    cout << "s.length    = " << s.length()   << '\n';
    cout << "s.capacity  = " << s.capacity() << '\n';
    cout << "s.max_size  = " << s.max_size() << '\n';
    cout << "sr.length   = " << sr.length()   << '\n';
    cout << "sr.capacity = " << sr.capacity() << '\n';
    cout << "sr.max_size = " << sr.max_size() << '\n';

    for (int i = 0; i < 10000; ++i) {
        if (s.length() == s.capacity()) {
            cout << "s reached capacity of " << s.length()
                 << ", growing...\n";
        }

        if (sr.length() == sr.capacity()) {
            cout << "sr reached capacity of " << sr.length()
                 << ", growing...\n";
        }

        s += 'x';
        sr += 'x';
    }
}
