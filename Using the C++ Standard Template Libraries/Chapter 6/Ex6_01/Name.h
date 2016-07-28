#ifndef NAME_H
#define NAME_H
#include <string>                                // For string class

class Name
{
private:
    std::string first {};
    std::string second {};

public:
    Name(const std::string& name1, const std::string& name2) : first(name1), second(name2) {}
    Name() = default;
    std::string get_first() const
    {
        return first;
    }
    std::string get_second() const
    {
        return second;
    }

    friend std::istream& operator>>(std::istream& in, Name& name);
    friend std::ostream& operator<<(std::ostream& out, const Name& name);
};

// Stream input for Name objects
inline std::istream& operator>>(std::istream& in, Name& name)
{
    return in >> name.first >> name.second;
}

// Stream output for Name objects
inline std::ostream& operator<<(std::ostream& out, const Name& name)
{
    return out << name.first << " " << name.second;
}
#endif