// Listing 70-6. One Implementation of the promote Metafunction
template<class T> struct promote          { typedef T type; };
template<> struct promote<bool>           { typedef int type; };
template<> struct promote<char>           { typedef int type; };
template<> struct promote<signed char>    { typedef int type; };
template<> struct promote<unsigned char>  { typedef int type; };
template<> struct promote<short>          { typedef int type; };
template<> struct promote<unsigned short> { typedef int type; };
