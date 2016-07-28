// Box.h
// Defines the Box class for Ex2_06
#ifndef BOX_H
#define BOX_H
#include <iostream>                              // For standard streams
#include <utility>                               // For comparison operator templates
using namespace std::rel_ops;                    // Comparison operator template namespace

class Box
{
private:
    size_t length {};
    size_t width {};
    size_t height {};

public:
    explicit Box(size_t l = 1, size_t w = 1, size_t h = 1) : length {l}, width {w}, height {h} {}
    double volume() const
    {
        return length * width * height;
    }
    bool operator<(const Box& box)
    {
        return volume() < box.volume();
    }
    bool operator==(const Box& box)
    {
        return length == box.length && width == box.width && height == box.height;
    }

    friend std::istream& operator>>(std::istream& in, Box& box);
    friend std::ostream& operator<<(std::ostream& out, const Box& box);
};

inline std::istream& operator>>(std::istream& in, Box& box)
{
    std::cout << "Enter box length, width, & height separated by spaces - Ctrl+Z to end: ";
    size_t value;
    in >> value;

    if (in.eof()) return in;

    box.length = value;
    in >> value;
    box.width = value;
    in >> value;
    box.height = value;
    return in;
}

inline std::ostream& operator<<(std::ostream& out, const Box& box)
{
    out << "Box(" << box.length << "," << box.width << "," << box.height << ")  ";
    return out;
}
#endif