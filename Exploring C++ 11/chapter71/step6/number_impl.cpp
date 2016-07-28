#include <istream>
#include <ostream>
#include <sstream>
#include <utility>

#include "calc_error.hpp"
#include "number_impl.hpp"

number_impl::~number_impl()
{}

void number_impl::save(std::ostream& stream)
const
{
  do_save(stream);
}

std::shared_ptr<number_impl> number_impl::read_library(std::istream& stream)
{
  std::string type{};
  if (not (stream >> type))
    throw calc_error{"malformed library, missing number type"};

  if (type == "void")
    return std::make_shared<number_void>();

  if (type == "long") {
    long long x{};
    if (not (stream >> x))
      throw calc_error{"malformed library, missing long value"};
    return std::make_shared<number_long>(x);
  }

  if (type == "double") {
    double x{};
    if (not (stream >> x))
      throw calc_error{"malformed library, missing double value"};
    return std::make_shared<number_double>(x);
  }

  throw calc_error{"malformed library, unknown number type: " + type};
}

void number_impl::print(std::ostream& stream)
const
{
  stream << to_string();
}

std::string number_impl::to_string()
const
{
  return do_to_string();
}

std::shared_ptr<number_impl> number_impl::promote(number_impl& rhs)
const
{
  return do_promote(rhs);
}

bool number_impl::equals(number_impl const& rhs)
const
{
  return do_equals(rhs);
}

bool number_impl::less(number_impl const& rhs)
const
{
  return do_less(rhs);
}

std::shared_ptr<number_impl> number_impl::add(number_impl const& rhs)
{
  return do_add(rhs);
}

std::shared_ptr<number_impl> number_impl::subtract(number_impl const& rhs)
{
  return do_subtract(rhs);
}

std::shared_ptr<number_impl> number_impl::multiply(number_impl const& rhs)
{
  return do_multiply(rhs);
}

std::shared_ptr<number_impl> number_impl::divide(number_impl const& rhs)
{
  return do_divide(rhs);
}

// Default is not to promote.
// The return value from the promote_to_*() functions is either
// a brand-new number_impl object, or this.
std::shared_ptr<number_impl> number_impl::promote_to_void()
{
  return shared_from_this();
}

std::shared_ptr<number_impl> number_impl::promote_to_long()
{
  return shared_from_this();
}

std::shared_ptr<number_impl> number_impl::promote_to_rational()
{
  return shared_from_this();
}

std::shared_ptr<number_impl> number_impl::promote_to_double()
{
  return shared_from_this();
}



std::string number_void::do_to_string()
const
{
  return std::string{};
}

void number_void::do_save(std::ostream& stream)
const
{
  stream << "void ";
}

// void never equals anything, even another void
bool number_void::do_equals(number_impl const&)
const
{
  return false;
}

// void is never less than anything
bool number_void::do_less(number_impl const&)
const
{
  return false;
}

// void plus void equal void
std::shared_ptr<number_impl> number_void::do_add(number_impl const&)
{
  return shared_from_this();
}

std::shared_ptr<number_impl> number_void::do_subtract(number_impl const&)
{
  return shared_from_this();
}

std::shared_ptr<number_impl> number_void::do_multiply(number_impl const&)
{
  return shared_from_this();
}

std::shared_ptr<number_impl> number_void::do_divide(number_impl const&)
{
  return shared_from_this();
}

std::shared_ptr<number_impl> number_void::do_promote(number_impl& rhs)
const
{
  return rhs.promote_to_void();
}



std::string number_long::do_to_string()
const
{
  std::ostringstream stream{};
  stream << value();
  return stream.str();
}

void number_long::do_save(std::ostream& stream)
const
{
  stream << "long " << value() << ' ';
}

bool number_long::do_equals(number_impl const& rhs)
const
{
  return value() == dynamic_cast<number_long const&>(rhs).value();
}

bool number_long::do_less(number_impl const& rhs)
const
{
  return value() < dynamic_cast<number_long const&>(rhs).value();
}

std::shared_ptr<number_impl> number_long::do_add(number_impl const& rhs)
{
  return std::make_shared<number_long>(value_ + dynamic_cast<number_long const&>(rhs).value());
}

std::shared_ptr<number_impl> number_long::do_subtract(number_impl const& rhs)
{
  return std::make_shared<number_long>(value_ - dynamic_cast<number_long const&>(rhs).value());
}

std::shared_ptr<number_impl> number_long::do_multiply(number_impl const& rhs)
{
  return std::make_shared<number_long>(value_ * dynamic_cast<number_long const&>(rhs).value());
}

std::shared_ptr<number_impl> number_long::do_divide(number_impl const& rhs)
{
  long long right = dynamic_cast<number_long const&>(rhs).value();
  if (right == 0)
    throw calc_error{"division by zero"};
  return std::make_shared<number_rational>(value_, right);
}

std::shared_ptr<number_impl> number_long::do_promote(number_impl& rhs)
const
{
  return rhs.promote_to_long();
}

std::shared_ptr<number_impl> number_long::promote_to_void()
{
  return std::make_shared<number_void>();
}

std::shared_ptr<number_impl> number_long::promote_to_rational()
{
  return std::make_shared<number_rational>(value());
}

std::shared_ptr<number_impl> number_long::promote_to_double()
{
  return std::make_shared<number_double>(value());
}


std::string number_rational::do_to_string()
const
{
  std::ostringstream stream{};
  stream << value();
  return stream.str();
}

void number_rational::do_save(std::ostream& stream)
const
{
  stream << "rational " << value() << ' ';
}

bool number_rational::do_equals(number_impl const& rhs)
const
{
  return value() == dynamic_cast<number_rational const&>(rhs).value();
}

bool number_rational::do_less(number_impl const& rhs)
const
{
  return value() < dynamic_cast<number_rational const&>(rhs).value();
}

std::shared_ptr<number_impl> number_rational::do_add(number_impl const& rhs)
{
  rational<long long> result{ value_ + dynamic_cast<number_rational const&>(rhs).value() };
  if (result.denominator() == 1)
    return std::make_shared<number_long>(result.numerator());
  else
    return std::make_shared<number_rational>(result);
}

std::shared_ptr<number_impl> number_rational::do_subtract(number_impl const& rhs)
{
  rational<long long> result{ value_ - dynamic_cast<number_rational const&>(rhs).value() };
  if (result.denominator() == 1)
    return std::make_shared<number_long>(result.numerator());
  else
    return std::make_shared<number_rational>(result);
}

std::shared_ptr<number_impl> number_rational::do_multiply(number_impl const& rhs)
{
  rational<long long> result{ value_ * dynamic_cast<number_rational const&>(rhs).value() };
  if (result.denominator() == 1)
    return std::make_shared<number_long>(result.numerator());
  else
    return std::make_shared<number_rational>(result);
}

std::shared_ptr<number_impl> number_rational::do_divide(number_impl const& rhs)
{
  rational<long long> right{ dynamic_cast<number_rational const&>(rhs).value() };
  if (right.numerator() == 0)
    throw calc_error{"division by zero"};
  rational<long long> result = value_ / right;
  if (result.denominator() == 1)
    return std::make_shared<number_long>(result.numerator());
  else
    return std::make_shared<number_rational>(result);
}

std::shared_ptr<number_impl> number_rational::do_promote(number_impl& rhs)
const
{
  return rhs.promote_to_rational();
}

std::shared_ptr<number_impl> number_rational::promote_to_void()
{
  return std::make_shared<number_void>();
}

std::shared_ptr<number_impl> number_rational::promote_to_double()
{
  return std::make_shared<number_double>(static_cast<double>(value()));
}



std::string number_double::do_to_string()
const
{
  std::ostringstream stream{};
  stream << value();
  return stream.str();
}

void number_double::do_save(std::ostream& stream)
const
{
  stream << "double " << value() << ' ';
}

bool number_double::do_equals(number_impl const& rhs)
const
{
  return value() == dynamic_cast<number_double const&>(rhs).value();
}

bool number_double::do_less(number_impl const& rhs)
const
{
  return value() < dynamic_cast<number_double const&>(rhs).value();
}

std::shared_ptr<number_impl> number_double::do_add(number_impl const& rhs)
{
  return std::make_shared<number_double>(value_ + dynamic_cast<number_double const&>(rhs).value());
}

std::shared_ptr<number_impl> number_double::do_subtract(number_impl const& rhs)
{
  return std::make_shared<number_double>(value_ - dynamic_cast<number_double const&>(rhs).value());
}

std::shared_ptr<number_impl> number_double::do_multiply(number_impl const& rhs)
{
  return std::make_shared<number_double>(value_ * dynamic_cast<number_double const&>(rhs).value());
}

std::shared_ptr<number_impl> number_double::do_divide(number_impl const& rhs)
{
  double right{ dynamic_cast<number_double const&>(rhs).value() };
  if (right == 0.0)
    throw calc_error{"division by zero"};
  return std::make_shared<number_double>(value_ / right);
}

std::shared_ptr<number_impl> number_double::do_promote(number_impl& rhs)
const
{
  return rhs.promote_to_double();
}

std::shared_ptr<number_impl> number_double::promote_to_void()
{
  return std::make_shared<number_void>();
}
