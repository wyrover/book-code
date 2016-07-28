#ifndef IOFLAGS_HPP_
#define IOFLAGS_HPP_

/** @file
 * @brief Save and restore I/O stream flags.
 */

/** Save and restore I/O stream flags.
 * When a function needs to temporarily alter an I/O stream flags,
 * simply define an object of type @c ioflags. Set whatever flags
 * you want. When the block exits or function returns, the
 * original flags are restored.
 */

class ioflags
{
public:
  /// Save the formatting flags from @p stream.
  /// @param stream The stream that will have its flags modified and restored.
  ioflags(std::basic_ios<char>& stream) : stream_(stream), flags_{stream.flags()} {}
  /// Restore the formatting flags.
  ~ioflags() { stream_.flags(flags_); }

  ioflags(ioflags const&) = delete;
  ioflags& operator=(ioflags const&) = delete;

private:
  std::basic_ios<char>& stream_;
  std::ios_base::fmtflags flags_;
};

#endif
