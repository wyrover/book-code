#include <string>
#include <iostream>

// The approach for narrow character strings
void rtrim(std::string& s, char c)
{
    if (s.empty())
        return;

    std::string::iterator p;

    for (p = s.end(); p != s.begin() && *--p == c;);

    if (*p != c)
        p++;

    s.erase(p, s.end());
}

int main()
{
    std::string s = "zoo";
    rtrim(s, 'o');
    std::cout << s << '\n';
}
