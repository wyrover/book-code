#ifndef RECORD_IO_H
#define RECORD_IO_H
#include <string>                                // For string class
#include <istream>                               // For istream class
#include <ostream>                               // For ostream class
#include <utility>                               // For pair type
#include <tuple>                                 // For tuple type

using Name = std::pair < std::string, std::string >;
using Phone = std::tuple < std::string, std::string, std::string >;

// Phone number output
inline std::ostream& operator<<(std::ostream& out, const Phone& phone)
{
    std::string area_code {}, exchange {}, number {};
    std::tie(area_code, exchange, number) = phone;
    out << area_code << " " << exchange << " " << number;
    return out;
}

// Name output
inline std::ostream& operator<<(std::ostream& out, const Name& name)
{
    out << name.first << " " << name.second;
    return out;
}

// Phone number input
inline std::istream& operator>>(std::istream& in, Phone& phone)
{
    std::string area_code {}, exchange {}, number {};
    in >> std::ws >> area_code >> exchange >> number;
    phone = std::make_tuple(area_code, exchange, number);
    return in;
}

// Name input
inline std::istream& operator>>(std::istream& in, Name& name)
{
    in >> std::ws >> name.first >> name.second;
    return in;
}
#endif