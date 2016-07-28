// Listing 70-2. Implementing the _binary User-Defined Literal Operator
/// Compute one bit of a binary integer.
/// Compute a new result by taking the right-most decimal digit from @p digits,
/// and if it is 1, shifting the 1 left by @p shift places and bitwise ORing
/// the value with @p result. Ignore digits other than 0 or 1. Recurse
/// to compute the remaining result.
/// @param digits The user-supplied decimal digits, should use only 0 and 1
/// @param result The numeric result so far
/// @param shift The number of places to shift the right-most digit in @p digits
/// @return if @p digits is zero, return @p result; otherwise return the result
/// of converting @p digits to binary
constexpr unsigned long long binary_helper(unsigned long long digits,
    unsigned long long result, int shift)
{
    return digits == 0 ?
        result :
        binary_helper(digits / 10,
            result | ((digits % 10 == 1) << shift),
            digits % 10 < 2 ? shift + 1 : shift);
}

constexpr unsigned long long operator"" _binary(unsigned long long digits)
{
   return binary_helper(digits, 0ULL, 0);
}
