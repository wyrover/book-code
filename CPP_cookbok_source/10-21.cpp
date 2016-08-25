#include <iostream>
#include <string>

using std::string;

string getFileName(const string& s)
{
    char sep = '/';
#ifdef _WIN32
    sep = '\\';
#endif
    size_t i = s.rfind(sep, s.length());

    if (i != string::npos) {
        return (s.substr(i + 1, s.length() - i));
    }

    return ("");
}

int main(int argc, char** argv)
{
    string path = argv[1];
    std::cout << "The file name is \"" << getFileName(path) << "\"\n";
}