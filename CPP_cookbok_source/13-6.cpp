#include <iostream>
#include <locale>
#include <string>
#include <sstream>

using namespace std;

long double readMoney(istream& in, bool intl = false)
{
    long double val;
    // Create a reader facet
    const money_get<char>& moneyReader =
        use_facet<money_get<char> >(in.getloc());
    // End marker
    istreambuf_iterator<char> end;
    // State variable for detecting errors
    ios_base::iostate state = 0;
    moneyReader.get(in, end, intl, in, state, val);

    // failbit will be set if something went wrong
    if (state != 0 && !(state & ios_base::eofbit))
        throw "Couldn't read money!\n";

    return (val);
}

void writeMoney(ostream& out, long double val, bool intl = false)
{
    // Create a writer facet
    const money_put<char>& moneyWriter =
        use_facet<money_put<char> >(out.getloc());

    // Write to the stream. Call failed( ) (the return value is an
    // ostreambuf_iterator) to see if anything went wrong.
    if (moneyWriter.put(out, intl, out, out.fill(), val).failed())
        throw "Couldn't write money!\n";
}

int main()
{
    long double val = 0;
    float exchangeRate = 0.775434f;  // Dollars to Euros
    locale locEn("english");
    locale locFr("french");
    cout << "Dollars: ";
    cin.imbue(locEn);
    val = readMoney(cin, false);
    cout.imbue(locFr);
    // Set the showbase flag so the currency char is printed
    cout.setf(ios_base::showbase);
    cout << "Euros: ";
    writeMoney(cout, val * exchangeRate, true);
}