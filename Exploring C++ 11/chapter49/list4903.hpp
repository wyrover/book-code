// Listing 49-3. The rational Class Template
#ifndef RATIONAL_HPP_
#define RATIONAL_HPP_
template<class T>
class rational
{
public:
  typedef T value_type;
  rational() : rational{0} {}
  rational(T num) : numerator_{num}, denominator_{1} {}
  rational(T num, T den);

  void assign(T num, T den);

  template<class U>
  U convert()
  const
  {
    return static_cast<U>(numerator()) / static_cast<U>(denominator());
  }

  T numerator() const { return numerator_; }
  T denominator() const { return denominator_; }
private:
  void reduce();
  T numerator_;
  T denominator_;
};

template<class T>
rational<T>::rational(T num, T den)
: numerator_{num}, denominator_{den}
{
  reduce();
}

template<class T>
void rational<T>::assign(T num, T den)
{
  numerator_ = num;
  denominator_ = den;
  reduce();
}

template<class T>
bool operator==(rational<T> const& a, rational<T> const& b)
{
  return a.numerator() == b.numerator() and
         a.denominator() == b.denominator();
}

template<class T>
inline bool operator!=(rational<T> const& a, rational<T> const& b)
{
  return not (a == b);
}

#endif
