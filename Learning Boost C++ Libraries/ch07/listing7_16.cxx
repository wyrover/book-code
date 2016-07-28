#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_pod.hpp>

// optimized for POD-type
template <typename T, size_t N>
typename boost::enable_if<boost::is_pod<T>>::type
copy(T (&lhs)[N], T (&rhs)[N])
{
  memcpy(lhs, rhs, N*sizeof(T));
}

