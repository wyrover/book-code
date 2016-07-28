 1 #include <boost/type_traits.hpp>
 2 #include <type_traits>
 3 #include <boost/mpl/and.hpp>
 4 #include <boost/mpl/or.hpp>
 5 #include <boost/mpl/not.hpp>
 6 #include <boost/mpl/greater.hpp>
 7 #include <boost/mpl/greater_equal.hpp>
 8 #include <boost/mpl/equal.hpp>
 9 #include <boost/mpl/if.hpp>
10 #include <boost/mpl/integral_c.hpp>
11 #include <boost/utility/enable_if.hpp>
12 #include <iostream>
13
14 namespace mpl = boost::mpl;
15
16 template <typename T, typename S>
17 struct is_larger
18    : mpl::greater<mpl::integral_c<size_t, sizeof(T)>
19                 , mpl::integral_c<size_t, sizeof(S)>>
20 {};
21 template <typename T, typename S>
22 struct is_smaller_equal
23   : mpl::not_<is_larger<T, S>>
24 {};
25
26 template <typename T, typename S>
27 struct is_floating_assignable
28    : mpl::and_<
29        boost::is_floating_point<T>
30      , boost::is_arithmetic<S>
31      , is_smaller_equal<S, T>
32      >
33 {};
34
35 template <typename T, typename S>
36 struct is_integer_assignable
37    : mpl::and_<
38        boost::is_integral<T>
39      , boost::is_integral<S>
40      , is_smaller_equal<S, T>
41      , mpl::if_<boost::is_signed<S>
42               , boost::is_signed<T>
43               , mpl::or_<boost::is_unsigned<T>
44                        , mpl::and_<boost::is_signed<T>
45                                  , is_larger<T, S>>
46                         >
47               >
48      >
49 {};
50
51 template <typename T, typename S>
52 struct is_non_pod_assignable
53    : mpl::and_<
54                mpl::not_<mpl::and_<boost::is_pod<T>
55                                  , boost::is_pod<S>>
56                         >
57              , std::is_assignable<T, S>
58               >
59 {};
60
61 template <typename T, typename U>
62 struct is_array_assignable
63    : boost::false_type
64 {};
65
66 template <typename T, size_t M, typename S, size_t N>
67 struct is_array_assignable<T (&)[M], S (&)[N]>
68    : mpl::and_<
69           mpl::or_<
70               boost::is_same<T, S>
71             , is_floating_assignable<T, S>
72             , is_integer_assignable<T, S>
73             , is_non_pod_assignable<T, S>
74              >
75         , mpl::greater_equal<mpl::integral_c<size_t, M>
76                            , mpl::integral_c<size_t, N>>
77         >
78 {};
79
80
81 template <typename T, size_t M, typename S, size_t N>
82 typename boost::enable_if<is_array_assignable<T(&)[M], 
83                                               S(&)[N]>>::type
84 assignArray(T (&target)[M], S (&source)[N])
85 { /* actual copying implementation */ }

