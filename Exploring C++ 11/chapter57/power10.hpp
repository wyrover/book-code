#ifndef POWER10_HPP_
#define POWER10_HPP_

// Listing 51-6. Computing a Power of 10 at Compile Time
/// Called from power10 to compute 10<sup>@p n</sup>, storing the result so far in @p result.
template<class T>
T constexpr power10_helper(T n, T result)
{
  return n == T{} ? result : power10_helper(n - T{1}, T{10} * result);
}

/// Compute a power of 10 at compile time. The type T must support subtraction and multiplication.
template<class T>
T constexpr power10(T n)
{
  return power10_helper(n, T{1});
}

#endif
