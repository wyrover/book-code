// Listing 70-4. Using a Function Parameter Pack to Implement the _binary Operator
/// Extract one bit from a bit string and then recurse.
template<char Head, char... Rest>
struct binary_helper
{
   constexpr unsigned long long operator()(unsigned long long result) const;
};

/// Teminate recursion when interpreting a bit string.

template<char Head>
struct binary_helper<Head>
{
   constexpr unsigned long long operator()(unsigned long long result) const;
};
   
template<char Head, char... Rest>
constexpr unsigned long long
binary_helper<Head, Rest...>::operator()(unsigned long long result)
const
{
   static_assert(Head == '0' or Head == '1', "_binary contains only 0 or 1");
   return binary_helper<Rest...>{}(result << 1 | (Head - '0'));
}

template<char Head>
constexpr unsigned long long
binary_helper<Head>::operator()(unsigned long long result)
const
{
   static_assert(Head == '0' or Head == '1', "_binary contains only 0 or 1");
   return result << 1 | (Head - '0');
}

template<char... Digits>
constexpr unsigned long long operator"" _binary()
{
   return binary_helper<Digits...>{}(0ULL);
}
