// Listing 45-7. Checking for a Zero Denominator in reduce()
void rational::reduce()
{
  if (denominator_ == 0)
    throw zero_denominator{"denominator is zero"};
  if (denominator_ < 0)
  {
    denominator_ = -denominator_;
    numerator_ = -numerator_;
  }
  int div{gcd(numerator_, denominator_)};
  numerator_ = numerator_ / div;
  denominator_ = denominator_ / div;
}
