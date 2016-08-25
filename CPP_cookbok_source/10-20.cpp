#include <iostream>
#include <string>

using std::string;

string getFileExt(const string& s)
{
    size_t i = s.rfind('.', s.length());

    if (i != string::npos) {
        return (s.substr(i + 1, s.length() - i));
    }

    return ("");
}

int main(int argc, char** argv)
{
    string path = argv[1];
    std::cout << "The extension is \"" << getFileExt(path) << "\"\n";
}