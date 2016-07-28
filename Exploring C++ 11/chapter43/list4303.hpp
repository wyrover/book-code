// Listing 43-3. Functor intrange to Generate Integers in a Certain Range
#ifndef INTRANGE_HPP_
#define INTRANGE_HPP_

#include <algorithm>

/// Check whether an integer lies within an inclusive range.
class intrange
{
public:
  inline intrange(int low, int high);
  inline bool operator()(int test) const;
private:
  int const low_;
  int const high_;
};

/// Construct an integer range.
/// If the parameters are in the wrong order,
/// swap them to the right order.
/// @param low the lower bound of the inclusive range
/// @param high the upper bound of the inclusive range
inline intrange::intrange(int low, int high)
: low_{std::min(low, high)}, high_{std::max(low, high)}
{}

/// Check whether a value lies within the inclusive range.
/// @param test the value to test
inline bool intrange::operator()(int test)
const
{
  return test >= low_ and test <= high_;
}

#endif
