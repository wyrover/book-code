// Listing 52-3. Defining Entities in a Namespace
namespace numeric
{
  template<class T>
  rational<T> operator+(rational<T> const& a, rational<T> const& b)
  {
    rational<T> result{a};
    result += b;
    return result;
  }
}

template<class T, int N>
numeric::fixed<T, N> numeric::operator+(fixed<T, N> const& a, fixed<T, N> const& b)
{
  fixed<T, N> result{a};
  result += b;
  return result;
}
