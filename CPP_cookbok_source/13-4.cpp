#include <iostream>
#include <ctime>
#include <locale>
#include <sstream>
#include <iterator>

using namespace std;

void translateDate(istream& in, ostream& out)
{
    // Create a date reader
    const time_get<char>& dateReader =
        use_facet<time_get<char> >(in.getloc());
    // Create a state object, which the facets will use to tell
    // us if there was a problem.
    ios_base::iostate state = 0;
    // End marker
    istreambuf_iterator<char> end;
    tm t; // Time struct (from <ctime>)
    // Now that all that's out of the way, read in the date from
    // the input stream and put it in a time struct.
    dateReader.get_date(in, end, in, state, &t);

    // Now the date is in a tm struct. Print it to the out stream
    // using its locale. Make sure you only print out what you
    // know is valid in t.
    if (state == 0 || state == ios_base::eofbit) {
        // The read succeeded.
        const time_put<char>& dateWriter =
            use_facet<time_put<char> >(out.getloc());
        char fmt[] = "%x";

        if (dateWriter.put(out, out, out.fill(),
                           &t, &fmt[0], &fmt[2]).failed())
            cerr << "Unable to write to output stream.\n";
    } else {
        cerr << "Unable to read cin!\n";
    }
}

int main()
{
    cin.imbue(locale("english"));
    cout.imbue(locale("german"));
    translateDate(cin, cout);
}