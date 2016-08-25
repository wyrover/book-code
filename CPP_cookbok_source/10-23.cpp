#include <iostream>
#include <string>

using std::string;

string getPathName(const string& s)
{
    char sep = '/';
#ifdef _WIN32
    sep = '\\';
#endif
    size_t i = s.rfind(sep, s.length());

    if (i != string::npos) {
        return (s.substr(0, i));
    }

    return ("");
}

int main(int argc, char** argv)
{
    string path = argv[1];
    std::cout << "The path name is \"" << getPathName(path) << "\"\n";
}