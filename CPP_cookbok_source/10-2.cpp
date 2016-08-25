#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

// TableFormatter formats data for output to a stream of characters
// of type T.
template<typename T>
class TableFormatter
{

public:
    TableFormatter(basic_ostream<T>& os) : out_(os) {}
    ~TableFormatter()
    {
        out_ << flush;
    }

    template<typename valT>
    void writeTableRow(const vector<valT>& v, int width);
    //...

private:
    basic_ostream<T>& out_;
};

template<typename T>    // refers to class template param list
template<typename valT> // refers to mem fn template param list
void TableFormatter<T>::writeTableRow(const std::vector<valT>& v,
                                      int width)
{
    ios_base::fmtflags flags = out_.flags();
    out_.flush();
    out_ << setprecision(2) << fixed;  // Set the precision, in case

    // this is floating-point data
    for (vector<valT>::const_iterator p = v.begin();
         p != v.end(); ++p)
        out_ << setw(width) << left << *p;  // Set the width, justify,

    // and write the element
    out_ << endl;     // Flush
    out_.setf(flags); // Set the flags back to normal
}

int main()
{
    TableFormatter<char> fmt(cout);
    vector<string> vs;
    vs.push_back("Sunday");
    vs.push_back("Monday");
    vs.push_back("Tuesday");
    fmt.writeTableRow(vs, 12);
    fmt.writeTableRow(vs, 12);
    fmt.writeTableRow(vs, 12);
    vector<double> vd;
    vd.push_back(4.0);
    vd.push_back(3.0);
    vd.push_back(2.0);
    vd.push_back(1.0);
    fmt.writeTableRow(vd, 5);
}