// Listing 43-5. Generating Random Integers
#ifndef RANDOMINT_HPP_
#define RANDOMINT_HPP_

#include <algorithm>
#include <random>

/// Generate uniformly distributed random integers in a range.
class randomint
{
public:
  typedef std::default_random_engine::result_type result_type;

  /// Construct a random-number generator to produce numbers in the range [<tt>low</tt>, <tt>high</tt>].
  /// If @p low > @p high the values are reversed.
  randomint(result_type low, result_type high)
     // std::random_device uses a system-dependent generation of randomness
      // to seed the pseudo-random-number generator.
  : prng_{std::random_device{}()},
    distribution_{std::min(low, high), std::max(low, high)}
  {}

  /// Generate the next random number generator.
  result_type operator()()
  {
     return distribution_(prng_);
  }

private:
  // implementation-defined pseudo-random-number generator
  std::default_random_engine prng_;
  // Map random numbers to a uniform distribution.
  std::uniform_int_distribution<result_type> distribution_;
};
#endif
