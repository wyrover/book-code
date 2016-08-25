#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cctype>

using namespace std;

inline bool caseInsCharCompSingle(char a, char b)
{
    return (toupper(a) == b);
}

string::const_iterator caseInsFind(string& s, const string& p)
{
    string tmp;
    transform(p.begin(), p.end(),               // Make the pattern
              back_inserter(tmp),                 // upper-case
              toupper);
    return (search(s.begin(), s.end(),          // Return the iter-
                   tmp.begin(), tmp.end(),       // ator returned by
                   caseInsCharCompSingle));        // search
}

int main()
{
    string s = "row, row, row, your boat";
    string p = "YOUR";
    string::const_iterator it = caseInsFind(s, p);

    if (it != s.end()) {
        cout << "Found it!\n";
    }
}
