#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

template<typename C>
void printContainer(const C& c, char delim = ',', ostream& out = cout)
{
    printRange(c.begin(), c.end(), delim, out);
}

template<typename Fwd>
void printRange(Fwd first, Fwd last, char delim = ',', ostream& out = cout)
{
    out << "{";

    while (first != last) {
        out << *first;

        if (++first != last)
            out << delim << ' ';
    }

    out << "}" << endl;
}

int main()
{
    cout << "Enter a series of strings: ";
    istream_iterator<string> start(cin);
    istream_iterator<string> end;
    vector<string> v(start, end);
    printContainer(v);
    printRange(v.begin(), v.end(), ';', cout);
}
