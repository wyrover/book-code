// Listing 51-1. The std::pair Class Template
template<class T, class U>
struct pair
{
   typedef T first_type;
   typedef U second_type;
   T first;
   U second;
   pair();
   pair(T const& first, U const& second);
   template<class T2, class U2>
   pair(pair<T2, U2> const& other);
};
