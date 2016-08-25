#include <string>
#include <iostream>

template<typename T>
int countUnique(const std::basic_string<T>& s)
{
    using std::basic_string;
    basic_string<T> chars;

    for (typename basic_string<T>::const_iterator p = s.begin();
         p != s.end(); ++p) {
        if (chars.find(*p) == basic_string<T>::npos)
            chars += *p;
    }

    return (chars.length());
}

int main()
{
    std::string s = "Abracadabra";
    std::cout << countUnique(s) << '\n';
}
