// Listing 70-11. Implementing an Integer-Keyed Type Choice
#include <limits>
#include <type_traits>

template<class T>
typename std::enable_if<std::numeric_limits<T>::is_signed, T>::type
minus(T const& x)
{
   return -x;
}
