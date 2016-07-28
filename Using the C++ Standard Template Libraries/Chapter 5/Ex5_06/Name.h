// Name.h for Ex5_06
// Defines a person's name
#ifndef NAME_H
#define NAME_H
#include <string>                                // For string class
#include <ostream>                               // For output streams
#include <istream>                               // For input streams
using std::string;

class Name
{
private:
    string first {};
    string second {};

public:
    Name(const string& name1, const string& name2) : first(name1), second(name2) {}
    Name() = default;

    const string& get_first() const
    {
        return first;
    }
    const string& get_second() const
    {
        return second;
    }

    size_t get_length() const
    {
        return first.length() + second.length() + 1;
    }

    // Less-than operator
    bool operator<(const Name& name) const
    {
        return second < name.second || (second == name.second && first < name.first);
    }

    // Equality operator
    bool operator==(const Name& name) const
    {
        return (second == name.second) && (first == name.first);
    }

    size_t hash() const
    {
        return std::hash<std::string>()(first + second);
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