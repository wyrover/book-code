// Listing 70-1. Computing a Power of 10 at Compile Time with Templates
template<int N>
struct power10; // forward declaration

template<int N>
struct square
{
  // A metaprogramming function to square a value.
  enum t { value = N * N };
};

template<int N, bool Even>
struct power10_aux
{
  // Primary template is for odd N (Even is false)
  enum t { value = 10 * power10<N - 1>::value };
};

template<int N>
struct power10_aux<N, true>
{
  // Specialization when N is even: square the value
  enum t { value = square<power10<N / 2>::value>::value };
};

template<int N>
struct power10
{
  enum t { value = power10_aux<N, N % 2 == 0>::value };
};

template<>
struct power10<0>
{
  enum t { value = 1 };
};
