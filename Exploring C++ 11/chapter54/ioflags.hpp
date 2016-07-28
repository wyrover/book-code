#ifndef IOFLAGS_HPP_
#define IOFLAGS_HPP_

/// Save current I/O stream flags to restore when exiting the block.
/// from Listing 39-4
class ioflags
{
public:
  /// Save the formatting flags from @p stream.
  ioflags(std::basic_ios<char>& stream) : stream_(stream), flags_{stream.flags()} {}
  ioflags(ioflags const&) = delete;
  /// Restore the formatting flags.
  ~ioflags() { stream_.flags(flags_); }
private:
  std::basic_ios<char>& stream_;
  std::ios_base::fmtflags flags_;
};

#endif

