// Listing 52-7. Examples of using Declarations with Namespaces
 1 #include <iostream>
 2
 3 void print(int i)
 4 {
 5   std::cout << "int: " << i << '\n';
 6 }
 7
 8 namespace labeled
 9 {
10   void print(double d)
11   {
12     std::cout << "double: " << d << '\n';
13   }
14 }
15
16 namespace simple
17 {
18   void print(int i)
19   {
20     std::cout << i << '\n';
21  }
22   void print(double d)
23   {
24     std::cout << d << '\n';
25   }
26 }
27
28 void test_simple()
29 {
30   using simple::print;
31   print(42);
32   print(3.14159);
33 }
34
35 void test_labeled()
36 {
37   using labeled::print;
38   print(42);
39   print(3.14159);
40 }
41
42 int main()
43 {
44   test_simple();
45   test_labeled();
46 }
