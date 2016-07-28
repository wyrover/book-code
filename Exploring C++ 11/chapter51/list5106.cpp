// Listing 51-6. Computing a Power of 10 at Compile Time
/// Called from power10 to compute 10<sup>@p n</sup>, storing the result so far in @p result.
int constexpr power10_helper(int n, int result)
{
  return n == 0 ? result : power10_helper(n - 1, 10 * result);
}

/// Compute a power of 10 at compile time. The type T must support subtraction and multiplication.
int constexpr power10(int n)
{
  return power10_helper(n, 1);
}
