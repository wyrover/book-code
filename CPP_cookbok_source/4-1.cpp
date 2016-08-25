#include <string>
#include <iostream>

using namespace std;

// The generic approach
template<typename T>
void pad(basic_string<T>& s,
         typename basic_string<T>::size_type n, T c)
{
    if (n > s.length())
        s.append(n - s.length(), c);
}

int main()
{
    string s = "Appendix A";
    wstring ws = L"Acknowledgments"; // The "L" indicates that
    // this is a wide char
    pad(s, 20, '*');                 // literal
    pad(ws, 20, L'*');
    cout << s << std::endl;
    wcout << ws << std::endl;
}
