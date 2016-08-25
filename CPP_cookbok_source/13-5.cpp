#include <iostream>
#include <locale>
#include <string>

using namespace std;

int main()
{
    cin.imbue(locale("german"));
    const time_get<char>& dateReader =
        use_facet<time_get<char> >(cin.getloc());
    time_base::dateorder d = dateReader.date_order();
    string s;

    switch (d) {
    case time_base::no_order:
        s = "No order";
        break;

    case time_base::dmy:
        s = "day/month/year";
        break;

    case time_base::mdy:
        s = "month/day/year";
        break;

    case time_base::ymd:
        s = "year/month/day";
        break;

    case time_base::ydm:
        s = "year/day/month";
        break;
    }

    cout << "Date order for locale " << cin.getloc().name()
         << " is " << s << endl;
}