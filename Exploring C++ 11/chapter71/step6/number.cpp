#include "number.hpp"
#include "number_impl.hpp"

#include <iostream>
#include <ostream>

number::number()
: pimpl_{std::make_shared<number_void>()}
{}

number::number(int x)
: pimpl_{std::make_shared<number_long>(x)}
{}

number::number(long x)
: pimpl_{std::make_shared<number_long>(x)}
{}

number::number(long long x)
: pimpl_{std::make_shared<number_long>(x)}
{}

number::number(rational<long long> const& x)
: pimpl_{std::make_shared<number_rational>(x)}
{}

number::number(double x)
: pimpl_{std::make_shared<number_double>(x)}
{}

number::number(std::istream& stream)
: pimpl_{number_impl::read_library(stream)}
{}

number::number(std::shared_ptr<number_impl> pimpl)
: pimpl_{std::move(pimpl)}
{}

void number::save(std::ostream& stream)
const
{
  pimpl_->save(stream);
}

std::string number::to_string()
const
{
  return pimpl_->to_string();
}

void number::print(std::ostream& stream)
{
  pimpl_->print(stream);
}

void number::coerce(number& that)
{
  // Promote that number to the type of this.
  that = number(this->pimpl_->promote(*that.pimpl_));
  // Promote this to the type of that.
  *this = number(that.pimpl_->promote(*this->pimpl_));
}

bool number::equals(number b)
{
  coerce(b);
  return pimpl_->equals(*b.pimpl_);
}

bool number::less(number b)
{
  coerce(b);
  return pimpl_->less(*b.pimpl_);
}

number number::operator+(number rhs)
{
  coerce(rhs);
  return number(pimpl_->add(*rhs.pimpl_));
}

number number::operator-(number rhs)
{
  coerce(rhs);
  return number(pimpl_->subtract(*rhs.pimpl_));
}

number number::operator*(number rhs)
{
  coerce(rhs);
  return number(pimpl_->multiply(*rhs.pimpl_));
}

number number::operator/(number rhs)
{
  coerce(rhs);
  return number(pimpl_->divide(*rhs.pimpl_));
}
