#include <iostream>
#include <string>

using std::string;

string pathAppend(const string& p1, const string& p2)
{
    char sep = '/';
    string tmp = p1;
#ifdef _WIN32
    sep = '\\';
#endif

    if (p1[p1.length()] != sep) {  // Need to add a
        tmp += sep;                // path separator
        return (tmp + p2);
    } else
        return (p1 + p2);
}

int main(int argc, char** argv)
{
    string path = argv[1];
    std::cout << "Appending somedir\\anotherdir is \""
              << pathAppend(path, "somedir\\anotherdir") << "\"\n";
}