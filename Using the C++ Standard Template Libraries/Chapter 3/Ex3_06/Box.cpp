// Box.cpp
// Function members of the Box class
#include <iostream>
#include "Box.h"

// Read a Box object from a stream
std::istream& Box::read(std::istream& in)
{
    size_t value {};

    if ((in >> value).eof())
        return in;

    length = value;
    in >> width >> height;
    return in;
}

// Write a Box object to a stream
std::ostream& Box::write(std::ostream& out) const
{
    out << typeid(*this).name() << "(" << length << "," << width << "," << height << ")";
    return out;
}