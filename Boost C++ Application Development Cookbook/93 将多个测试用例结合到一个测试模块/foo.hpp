#ifndef _FOO_HPP_
#define _FOO_HPP_

#include <stdexcept>
struct foo {
    int val_;

    operator int() const;
    bool is_not_null() const;
    void throws() const; // throws(std::logic_error)
};

#endif // _FOO_HPP_