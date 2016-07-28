// Listing 50-6. Specializing numeric_limits for the rational Class
namespace std {
template<>
class numeric_limits<rational>
{
public:
  static constexpr bool is_specialized{true};
  static constexpr rational min() noexcept { return rational(numeric_limits<int>::min()); }
  static constexpr rational max() noexcept { return rational(numeric_limits<int>::max()); }
  static rational lowest() noexcept { return -max(); }
  static constexpr int digits{ 2 * numeric_limits<int>::digits };
  static constexpr int digits10{ numeric_limits<int>::digits10 };
  static constexpr int max_digits10{ numeric_limits<int>::max_digits10 };
  static constexpr bool is_signed{ true };
  static constexpr bool is_integer{ false };
  static constexpr bool is_exact{ true };
  static constexpr int radix{ 2 };
  static constexpr bool is_bounded{ true };
  static constexpr bool is_modulo{ false };
  static constexpr bool traps{ std::numeric_limits<int>::traps };

  static rational epsilon() noexcept
     { return rational(1, numeric_limits<int>::max()-1); }
  static rational round_error() noexcept
     { return rational(1, numeric_limits<int>::max()); }

  // The following are meaningful only for floating-point types.
  static constexpr int min_exponent{ 0 };
  static constexpr int min_exponent10{ 0 };
  static constexpr int max_exponent{ 0 };
  static constexpr int max_exponent10{ 0 };
  static constexpr bool has_infinity{ false };
  static constexpr bool has_quiet_NaN{ false };
  static constexpr bool has_signaling_NaN{ false };
  static constexpr float_denorm_style has_denorm {denorm_absent};
  static constexpr bool has_denorm_loss {false};
  // The following are meant only for floating-point types, but you have
  // to define them, anyway, even for nonfloating-point types. The values
  // they return do not have to be meaningful.
  static constexpr rational infinity() noexcept { return max(); }
  static constexpr rational quiet_NaN() noexcept { return rational(); }
  static constexpr rational signaling_NaN() noexcept { return rational(); }
  static constexpr rational denorm_min() noexcept { return rational(); }
  static constexpr bool is_iec559{ false };
  static constexpr bool tinyness_before{ false };
  static constexpr float_round_style round_style{ round_toward_zero };
};

} // namespace std
