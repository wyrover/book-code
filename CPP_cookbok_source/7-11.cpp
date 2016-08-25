#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

int main()
{
    // An input iterator is the opposite of an output iterator: it
    // reads elements from a stream as if it were a container.
    cout << "Enter a series of strings: ";
    istream_iterator<string> start(cin);
    istream_iterator<string> end;
    vector<string> v(start, end);
    // Treat the output stream as a container by using an
    // output_iterator.  It constructs an output iterator where writing
    // to each element is equivalent to writing it to the stream.
    copy(v.begin(), v.end(), ostream_iterator<string>(cout, ", "));
}
