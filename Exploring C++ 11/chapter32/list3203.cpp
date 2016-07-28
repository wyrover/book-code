// Listing 32-3. Assignment of an Integer to a rational
rational& operator=(int num)
{
  this->numerator = num;
  this->denominator = 1; // no need to call reduce()
  return *this;
}
