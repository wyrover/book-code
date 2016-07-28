// Listing 37-5. Accidentally Inheriting Privately
class base
{
public:
  base(int v) : value_{v} {}
  int value() const { return value_; }
private:
  int value_;
};

class derived : base
{
public:
  derived() : base{42} {}
};

int main()
{
  base b{42};
  int x{b.value()};
  derived d{};
  int y{d.value()};
}
