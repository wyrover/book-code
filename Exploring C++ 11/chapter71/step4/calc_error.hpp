#ifndef CALC_ERROR_HPP_
#define CALC_ERROR_HPP_

#include <cstdlib>
#include <stdexcept>
#include <string>

/** @file
 * Define all the error and exception classes for the calculator.
 * They all derive from calc_error.
 */

/** Base class for all calculator errors. */
class calc_error : public std::runtime_error
{
public:
  calc_error(std::string const& msg) : runtime_error{msg} {}
};

/** Exception class for parser errors. */
class parse_error : public calc_error {
public:
  parse_error(std::string const& msg) : calc_error{msg} {}
};

/** Exception class for parser syntax errors. */
class syntax_error : public parse_error {
public:
  syntax_error(std::string const& msg) : parse_error{"syntax error: " + msg} {}
};

/** Exception class for function call problems.
 */
class function_error : public calc_error
{
public:
  function_error(std::string const &name, std::size_t expected, std::size_t actual)
  : calc_error{msg(name, expected, actual)}
  {}
private:
  std::string msg(std::string const& name, std::size_t expected, std::size_t actual);
};

/** Exception class for calling a function that was not defined. */
class no_such_function : public calc_error {
public:
  no_such_function(std::string const& what) : calc_error{"unknown function: " + what} {}
};

#endif
