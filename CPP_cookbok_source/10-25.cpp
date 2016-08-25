#include <iostream>
#include <string>

using std::string;

void replaceExt(string& s, const string& newExt)
{
    string::size_type i = s.rfind('.', s.length());

    if (i != string::npos) {
        s.replace(i + 1, newExt.length(), newExt);
    }
}

int main(int argc, char** argv)
{
    string path = argv[1];
    replaceExt(path, "foobar");
    std::cout << "The new name is \"" << path << "\"\n";
}