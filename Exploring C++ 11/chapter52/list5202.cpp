// Listing 52-2. Defining the fixed Class Template in the numeric Namespace
#ifndef FIXED_HPP_
#define FIXED_HPP_

namespace numeric
{
  template<class T, int N>
  class fixed
  {
    ... copied from Exploration 51...
  };
  template<class T, int N>
  bool operator==(fixed<T,N> const& a, fixed<T,N> const& b);
  template<class T, int N>
  fixed<T,N> operator+(fixed<T,N> const& a, fixed<T,N> const& b);
  // and so on...
} // namespace numeric

#endif
