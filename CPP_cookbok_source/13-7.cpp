#include <iostream>
#include <locale>
#include <string>

using namespace std;

string printPattern(moneypunct<char>::pattern& pat)
{
    string s(pat.field);  // pat.field is a char[4]
    string r;

    for (int i = 0; i < 4; ++i) {
        switch (s[i]) {
        case moneypunct<char>::sign:
            r += "sign ";
            break;

        case moneypunct<char>::none:
            r += "none ";
            break;

        case moneypunct<char>::space:
            r += "space ";
            break;

        case moneypunct<char>::value:
            r += "value ";
            break;

        case moneypunct<char>::symbol:
            r += "symbol ";
            break;
        }
    }

    return (r);
}

int main()
{
    locale loc("danish");
    const moneypunct<char>& punct =
        use_facet<moneypunct<char> >(loc);
    cout << "Decimal point:       " << punct.decimal_point() << '\n'
         << "Thousands separator: " << punct.thousands_sep() << '\n'
         << "Currency symbol:     " << punct.curr_symbol() << '\n'
         << "Positive sign:       " << punct.positive_sign() << '\n'
         << "Negative sign:       " << punct.negative_sign() << '\n'
         << "Fractional digits:   " << punct.frac_digits() << '\n'
         << "Positive format:     "
         << printPattern(punct.pos_format()) << '\n'
         << "Negative format:     "
         << printPattern(punct.neg_format()) << '\n';
    // Grouping is represented by a string of chars, but the meaning
    // of each char is its integer value, not the char it represents.
    string s = punct.grouping();

    for (string::iterator p = s.begin(); p != s.end(); ++p)
        cout << "Groups of: " << (int)*p << '\n';
}