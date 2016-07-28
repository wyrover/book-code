// Listing 62-2. Deliberate Errors with new and delete
#include <memory>

int main()
{
   int* p{ new int[10] };
   delete p;
   p = new int;
   delete[] p;
   std::unique_ptr<int> up{ new int[10] };
   up.reset();
   std::unique_ptr<int[]> upa{ new int };
   upa.reset();
}
   
