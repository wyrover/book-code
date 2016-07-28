 1 #include <boost/mpl/or.hpp>
 2 #include <boost/type_traits.hpp>
 3
 4 template <typename T>
 5 struct is_pointer_or_array
 6       : boost::mpl::or_<boost::is_pointer<T>, 
 7                         boost::is_array<T>>
 8 {};

