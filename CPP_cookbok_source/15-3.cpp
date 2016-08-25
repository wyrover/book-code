#include <iostream>
#include <string>

void concat(const std::string& s1, // These are declared const, so they
            const std::string& s2, // cannot be changed
            std::string& out)
{
    out = s1 + s2;
}

int main()
{
    std::string s1 = "Cabo ";
    std::string s2 = "Wabo";
    std::string s3;
    concat(s1, s2, s3);
    std::cout << "s1 = " << s1 << '\n';
    std::cout << "s2 = " << s2 << '\n';
    std::cout << "s3 = " << s3 << '\n';
}