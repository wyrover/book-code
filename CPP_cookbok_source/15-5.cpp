#include <iostream>
#include <cstring>

class String    // Assume the String class declaration
{
    // has at least everything shown here
public:
    String();
    String(const char* p);
    String(const String& orig);
    ~String()
    {
        delete buf_;
    }

    String& append(const String& s);
    size_t length() const;
    const char* data() const;
    String& operator=(const String& orig);

    // ...
};

String operator+(const String& lhs, const String& rhs)
{
    String tmp(lhs); // Copy construct a temp object
    tmp.append(rhs); // Use a member function to do the real work
    return (tmp);    // Return the temporary
}

int main()
{
    String s1("banana ");
    String s2("rancher");
    String s3, s4, s5, s6;
    s3 = s1 + s2;           // Works fine, no surprises
    s4 = s1 + "rama";       // Constructs "rama" automatically using
    // the constructor String(const char*)
    s5 = "ham " + s2;       // Hey cool, it even does it backward
    s6 = s1 + "rama " + s2;
    std::cout << "s3 = " << s3.data() << '\n';
    std::cout << "s4 = " << s4.data() << '\n';
    std::cout << "s5 = " << s5.data() << '\n';
    std::cout << "s6 = " << s6.data() << '\n';
}