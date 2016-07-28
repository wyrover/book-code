#ifndef TEST_HPP_
#define TEST_HPP_

#include <exception>
#include <iostream>
#include <ostream>

// For internal use by the TEST() macro.
// Turn the macro argument into a character string literal
#define TEST_STRINGIFY(x) #x

// For internal use by the TEST() macro.
// Report a test failure.
inline void test_failed(char const* expr, char const* file, int line)
{
   std::cerr << file << ", line " << line << ": test failed: " << expr << '\n';
}

// For internal use by the TEST() macro
// Run a test. Report a failure if the condition is false or
inline void test_run(bool condition, char const* expr, char const* file, int line)
{
  if (not condition)
    test_failed(expr, file, line);
}

// For internal use by the TEST() macro.
// Report an exception.
inline void test_exception(std::exception const& ex, char const* expr, char const* file, int line)
{
  std::string msg( expr );
  msg += " threw an exception: ";
  msg += ex.what();
  test_failed(msg.c_str(), file, line);
}

/// Test a condition, @p x.
/// If @p x evaluates to @c true the test passes.
/// If not, the test fails, and a message is printed to @c cerr.
/// The text of @p x, plus the file name and line number are printed.
///
/// See Boost.Test for a real test framework
///
/// @param x A condition to test; the condition must be able to be converted implicitly to @c bool.
#define TEST(x) \
try {\
  test_run(x, TEST_STRINGIFY(x), __FILE__, __LINE__);\
}\
catch(std::exception const& ex)\
{\
  test_exception(ex, TEST_STRINGIFY(x), __FILE__, __LINE__);\
}

#endif
