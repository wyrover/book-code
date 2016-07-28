// Listing 40-1. Declarations and Definitions of Member Functions
class rational
{
public:
  rational();
  rational(int num);
  rational(int num, int den);
  void assign(int num, int den);
  int numerator() const;
  int denominator() const;
  rational& operator=(int num);
private:
  void reduce();
  int numerator_;
  int denominator_;
};

rational::rational()
: rational{0}
{}

rational::rational(int num)
: numerator_{num}, denominator_{1}
{}

rational::rational(int num, int den)
: numerator_{num}, denominator_{den}
{
  reduce();
}

void rational::assign(int num, int den)
{
  numerator_ = num;
  denominator_ = den;
  reduce();
}

void rational::reduce()
{
  assert(denominator_ != 0);
  if (denominator_ < 0)
  {
    denominator_ = -denominator_;
    numerator_ = -numerator_;
  }
  int div{gcd(numerator_, denominator_)};
  numerator_ = numerator_ / div;
  denominator_ = denominator_ / div;
}

int rational::numerator()
const
{
  return numerator_;
}

int rational::denominator()
const
{
  return denominator_;
}

rational& rational::operator=(int num)
{
  numerator_ = num;
  denominator_ = 1;
  return *this;
}
