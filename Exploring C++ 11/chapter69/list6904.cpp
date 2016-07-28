// Listing 69-4. Ranking Functions for Overload Resolution
 1 #include <iostream>
 2 #include <string>
 3
 4 void print(std::string const& str) { std::cout << str; }
 5 void print(int x)                  { std::cout << "int: " << x; }
 6 void print(double x)               { std::cout << "double: " << x; }
 7
 8 class base {
 9 public:
10   void print(std::string const& str) const { ::print(str); ::print("\n"); }
11   void print(std::string const& s1, std::string const& s2)
12   {
13     print(s1); print(s2);
14   }
15 };
16
17 class convert : public base {
18 public:
19   convert()              { print("convert()"); }
20   convert(double)        { print("convert(double)"); }
21   operator int() const   { print("convert::operator int()"); return 42; }
22   operator float() const { print("convert::operator float()"); return 3.14159f; }
23 };
24
25 class demo : public base {
26 public:
27   demo(int)      { print("demo(int)"); }
28   demo(long)     { print("demo(long)"); }
29   demo(convert)  { print("demo(convert)"); }
30   demo(int, int) { print("demo(int, int)"); }
31 };
32
33 class other {
34 public:
35   other()        { std::cout << "other::other()\n"; }
36   other(int,int) { std::cout << "other::other(int, int)\n"; }
37   operator convert() const
38   {
39     std::cout << "other::operator convert()\n"; return convert();
40   }
41 };
42
43 int operator+(demo const&, demo const&)
44 {
45   print("operator+(demo,demo)\n"); return 42;
46 }
47
48 int operator+(int, demo const&) { print("operator+(int,demo)\n"); return 42; }
49
50 int main()
51 {
52   other x{};
53   demo d{x};
54   3L + d;
55   short s{2};
56   d + s;
57 }
