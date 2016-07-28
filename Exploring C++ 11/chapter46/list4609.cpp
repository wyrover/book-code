// Listing 46-9. Postfix Increment and Decrement Operators
rational rational::operator++(int)
{
  rational result{*this};
  numerator_ += denominator_;
  return result;
}

rational rational::operator--(int)
{
  rational result{*this};
  numerator_ -= denominator_;
  return result;
}
