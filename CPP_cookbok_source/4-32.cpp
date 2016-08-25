#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void split(const string& s, char c,
           vector<string>& v)
{
    int i = 0;
    int j = s.find(c);

    while (j >= 0) {
        v.push_back(s.substr(i, j - i));
        i = ++j;
        j = s.find(c, j);

        if (j < 0) {
            v.push_back(s.substr(i, s.length()));
        }
    }
}

void loadCSV(istream& in, vector<vector<string>*>& data)
{
    vector<string>* p = NULL;
    string tmp;

    while (!in.eof()) {
        getline(in, tmp, '\n');                     // Grab the next line
        p = new vector<string>();
        split(tmp, ',', *p);                        // Use split from
        // <link linkend="cplusplusckbk-CHP-4-SECT-7">Recipe 4.7</link>
        data.push_back(p);
        cout << tmp << '\n';
        tmp.clear();
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
        return (EXIT_FAILURE);

    ifstream in(argv[1]);

    if (!in)
        return (EXIT_FAILURE);

    vector<vector<string>*> data;
    loadCSV(in, data);

    // Go do something useful with the data...

    for (vector<vector<string>*>::iterator p = data.begin();
         p != data.end(); ++p) {
        delete *p;                                  // Be sure to de-
    }                                              // reference p!
}
