// Listing 53-1. Specializing the hash Template for the rational Type
#include <array>
#include "rational.hpp"
namespace std {

template<class T>
class hash<rational<T>>
{
public:
  std::size_t operator()(rational<T> const& r)
  const
  {
    return hasher_(r.numerator()) + hasher_(r.denominator());
  }
private:
  std::hash<T> hasher_;
};
} // end of std
