// Listing 52-8. Comparing a using Directive with a using Declaration
#include <iostream>

void demonstrate_using_directive()
{
   using namespace std;
   typedef int ostream;
   ostream x{0};
   std::cout << x << '\n';
}

void demonstrate_using_declaration()
{
   using std::ostream;
   typedef int ostream;
   ostream x{0};
   std::cout << x << '\n';
}
