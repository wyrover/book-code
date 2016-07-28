// Listing 52-1. Defining the rational Class Template in the numeric Namespace
#ifndef RATIONAL_HPP_
#define RATIONAL_HPP_

namespace numeric
{
  template<class T>
  class rational
  {
    ... you know what goes here...
  };
  template<class T>
  bool operator==(rational<T> const& a, rational<T> const& b);
  template<class T>
  rational<T> operator+(rational<T> const& a, rational<T> const& b);
  ... and so on...
} // namespace numeric

#endif
