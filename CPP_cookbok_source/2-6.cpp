// Value.h
#ifndef VALUE_H__
#define VALUE_H__
#include <string>

class Value
{
public:
    Value(const std::string& val) : val_(val) {}
    std::string getVal() const;
private:
    std::string val_;
};

#include "Value.inl"
#endif VALUE_H__

// Value.inl
inline std::string Value::getVal() const
{
    return (val_);
}
