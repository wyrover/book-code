// Hash_Name.h
// Function object type to hash Name objects for Ex5_06
#ifndef HASH_NAME_H
#define HASH_NAME_H
#include "Name.h"

class Hash_Name
{
public:
    size_t operator()(const Name& name)
    {
        return name.hash();
    }
};
#endif