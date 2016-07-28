 1 #include <boost/mpl/or.hpp>
 2 #include <boost/type_traits.hpp>
 3
 4 if (boost::mpl::or_<
 5                     boost::is_pointer<int*>,
 6                     boost::is_array<int*>
 7                    >::value) {
 8   std::cout << "int* is a pointer or array type\n";
 9 }
10
11 if (boost::mpl::or_<
12                     boost::is_pointer<int[]>,
13                     boost::is_array<int[]>
14                    >::value) {
15   std::cout << "int* is a pointer or array type\n";
16 }

