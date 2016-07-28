// Numeric_Range.h for Ex2_07
// Defines class templates for a range and iterators for the range
#ifndef NUMERIC_RANGE_H
#define NUMERIC_RANGE_H

#include <exception>                             // For standard exception types
#include <iterator>                              // For iterator type
#include <type_traits>                           // For compile-time type checking

template <typename T> class Numeric_Iterator;    // Template type declaration

// Defines a numeric range
template<typename T>
class Numeric_Range
{
    static_assert(std::is_integral<T>::value || std::is_floating_point<T>::value,
                  "Numeric_Range type argument must be numeric.");

    friend class Numeric_Iterator < T >;

protected:
    T start;                                       // First value in the range
    T step;                                        // Increment between successive values
    size_t count;                                  // Number of values in the range
public:
    Numeric_Range(T first = 0, T incr = 1, size_t n = 2) : start {first}, step {incr}, count {n} {}

    // Return the begin iterator for the range
    Numeric_Iterator<T> begin()
    {
        return Numeric_Iterator<T>(*this);
    }

    // Return the end iterator for the range
    Numeric_Iterator<T> end()
    {
        Numeric_Iterator<T> end_iter(*this);
        end_iter.value = start + count * step;        // End iterator value is one step over the last
        return end_iter;
    }
};

// Iterator class template- it's a forward iterator
template<typename T>
class Numeric_Iterator : public std::iterator < std::forward_iterator_tag, T >
{
    friend class Numeric_Range < T >;

protected:
    Numeric_Range<T>& range;                       // Reference to the range for this iterator
    T value;                                       // Value pointed to

public:
    explicit Numeric_Iterator(Numeric_Range<T>& a_range) : range {a_range}, value {a_range.start} {};

    // Assignment operator
    Numeric_Iterator& operator=(const Numeric_Iterator& src)
    {
        range = src.range;
        value = src.value;
    }

    // Dereference an iterator
    T& operator*()
    {
        // When the value is one step more than the last, it's an end iterator
        if (value == static_cast<T>(range.start + range.count * range.step)) {
            throw std::logic_error("Cannot dereference an end iterator.");
        }

        return value;
    }

    // Prefix increment operator
    Numeric_Iterator& operator++()
    {
        // When the value is one step more than the last, it's an end iterator
        if (value == static_cast<T>(range.start + range.count * range.step)) {
            throw std::logic_error("Cannot increment an end iterator.");
        }

        value += range.step;                         // Increment the value by the range step
        return *this;
    }

    // Postfix increment operator
    Numeric_Iterator operator++(int)
    {
        // When the value is one step more than the last, it's an end iterator
        if (value == static_cast<T>(range.start + range.count * range.step)) {
            throw std::logic_error("Cannot increment an end iterator.");
        }

        auto temp = *this;
        value += range.step;                         // Increment the value by the range step
        return temp;                                 // The iterator before it's incremented
    }

    // Comparisons
    bool operator<(const Numeric_Iterator& iter) const
    {
        return value < iter.value;
    }
    bool operator==(const Numeric_Iterator& iter) const
    {
        return value == iter.value;
    }
    bool operator!=(const Numeric_Iterator& iter) const
    {
        return value != iter.value;
    }
    bool operator>(const Numeric_Iterator& iter) const
    {
        return value > iter.value;
    }
    bool operator<=(const Numeric_Iterator& iter) const
    {
        *this < iter || *this == iter;
    }
    bool operator>=(const Numeric_Iterator& iter) const
    {
        *this > iter || *this == iter;
    }
};
#endif
