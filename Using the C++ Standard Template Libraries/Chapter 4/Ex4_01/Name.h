// Name.h for Ex4_01
// Defines a person's name
#ifndef NAME_H
#define NAME_H
#include <string>                                // For string class
#include <ostream>                               // For output streams
#include <istream>                               // For input streams

class Name
{
private:
    std::string first {};
    std::string second {};

public:
    Name(const std::string& name1, const std::string& name2) : first(name1), second(name2) {}
    Name() = default;


    // Less-than operator
    bool operator<(const Name& name) const
    {
        return second < name.second || (second == name.second && first < name.first);
    }

    friend std::istream& operator>>(std::istream& in, Name& name);
    friend std::ostream& operator<<(std::ostream& out, const Name& name);
};

// Extraction operator overload
inline std::istream& operator>>(std::istream& in, Name& name)
{
    in >> name.first >> name.second;
    return in;
}

// Insertion operator overload
inline std::ostream& operator<<(std::ostream& out, const Name& name)
{
    out << name.first + " " + name.second;
    return out;
}

#endif