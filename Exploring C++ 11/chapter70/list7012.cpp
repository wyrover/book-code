// Listing 70-12. Specializing the rational Class Using enable_if
#include <limits>
#include <type_traits>

template<class T, class Enable = typename std::enable_if<std::numeric_limits<T>::is_integer, T>::type>
class rational {
public:
   ... normal class definition here ...
};

rational<long> okay{1, 2};
rational<float> problem{1, 2};
