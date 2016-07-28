// Box.h
// Defines the Box class that will be a base for the Carton class
#ifndef BOX_H
#define BOX_H
#include <iostream>                              // For standard streams
#include <istream>                               // For stream classes
#include <utility>                               // For comparison operator templates
using namespace std::rel_ops;                    // Comparison operator template namespace

class Box
{
protected:
    size_t length {};
    size_t width {};
    size_t height {};

public:
    explicit Box(size_t l = 1, size_t w = 1, size_t h = 1) : length {l}, width {w}, height {h} {}
    virtual ~Box() = default;

    virtual double volume() const;                 // Volume of a box

    // Comparison operators for Box object
    bool operator<(const Box& box) const;
    bool operator==(const Box& box) const;

    // Stream input and output
    virtual std::istream& read(std::istream& in);
    virtual std::ostream& write(std::ostream& out) const;
};


inline double Box::volume() const
{
    return length * width * height;
}

// Less-than operator
inline bool Box::operator<(const Box& box) const
{
    return volume() < box.volume();
}

//Equality comparion operator
inline bool Box::operator==(const Box& box) const
{
    return length == box.length && width == box.width && height == box.height;
}

// Stream extraction operator
inline std::istream& operator>>(std::istream& in, Box& box)
{
    return box.read(in);
}

// Stream insertion operator
inline std::ostream& operator<<(std::ostream& out, Box& box)
{
    return box.write(out);
}
#endif