#include <iostream>
#include <map>
#include <string>

using namespace std;

int main()
{
    map<string, string> strMap;
    strMap["Monday"]    = "Montag";
    strMap["Tuesday"]   = "Dienstag";
    strMap["Wednesday"] = "Mittwoch";
    strMap["Thursday"]  = "Donnerstag";
    strMap["Friday"]    = "Freitag";
    strMap["Saturday"]  = "Samstag";
    // strMap.insert(make_pair("Sunday", "Sonntag"));
    strMap.insert(pair<string, string>("Sunday", "Sonntag"));

    for (map<string, string>::iterator p = strMap.begin();
         p != strMap.end(); ++p) {
        cout << "English: " << p->first
             << ", German: " << p->second << endl;
    }

    cout << endl;
    strMap.erase(strMap.find("Tuesday"));

    for (map<string, string>::iterator p = strMap.begin();
         p != strMap.end(); ++p) {
        cout << "English: " << p->first
             << ", German: " << p->second << endl;
    }
}
