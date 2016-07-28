// Hash_Function_Objects.h
// Hash function object types for Ex4_07
#ifndef HASH_FUNCTION_OBJECTS_H
#define HASH_FUNCTION_OBJECTS_H

#include <string>                                // For string class
#include <utility>                               // For pair type
#include <tuple>                                 // For tuple type

using Name = std::pair < std::string, std::string >;
using Phone = std::tuple < std::string, std::string, std::string >;

// Hash a phone number
class PhoneHash
{
public:
    size_t operator()(const Phone& phone) const
    {
        return std::hash<std::string>()(std::get<0>(phone) + std::get<1>(phone) + std::get<2>(phone));
    }
};

// Hash a name
class NameHash
{
public:
    size_t operator()(const Name& name) const
    {
        return std::hash<std::string>()(name.first + name.second);
    }
};
#endif