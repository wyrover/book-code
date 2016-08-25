#include <string>
#include <iostream>

using namespace std;

template<typename T>
void removeSubstrs(basic_string<T>& s,
                   const basic_string<T>& p)
{
    basic_string<T>::size_type n = p.length();

    for (basic_string<T>::size_type i = s.find(p);
         i != basic_string<T>::npos;
         i = s.find(p))
        s.erase(i, n);
}

int main()
{
    string s = "One fish, two fish, red fish, blue fish";
    string p = "fish";
    removeSubstrs(s, p);
    cout << s << '\n';
}
