#include <iostream>
#include <locale>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool localeLessThan(const string& s1, const string& s2)
{
    const collate<char>& col =
        use_facet<collate<char> >(locale());  // Use the global locale
    const char* pb1 = s1.data();
    const char* pb2 = s2.data();
    return (col.compare(pb1, pb1 + s1.size(),
                        pb2, pb2 + s2.size()) < 0);
}

int main()
{
    // Create two strings, one with a German character
    string s1 = "diät";
    string s2 = "dich";
    vector<string> v;
    v.push_back(s1);
    v.push_back(s2);
    // Sort without giving a locale, which will sort according to the
    // current global locale's rules.
    sort(v.begin(), v.end());

    for (vector<string>::const_iterator p = v.begin();
         p != v.end(); ++p)
        cout << *p << endl;

    // Set the global locale to German, and then sort
    locale::global(locale("german"));
    sort(v.begin(), v.end(), localeLessThan);

    for (vector<string>::const_iterator p = v.begin();
         p != v.end(); ++p)
        cout << *p << endl;
}