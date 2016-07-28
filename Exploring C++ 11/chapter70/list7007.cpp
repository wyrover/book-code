// Listing 70-7. Another Implementation of the promote Metafunction
// Primary template when IsMember=std::true_type.
template<class IsMember, class T>
struct get_member {
   typedef T type;
};

template<class T>
struct get_member<std::true_type, T>
{
   typedef int type;
};

template<class T>
struct promote {
    typedef typename get_member<typename is_member<T,
        bool, unsigned char, signed char, char, unsigned short, short>::type, T>::type type;
};
