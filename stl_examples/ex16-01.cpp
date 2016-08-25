// Demonstrating a counting iterator class.
#include <iostream.h>
#include <algo.h>
#include <bstring.h>
#include "counting.h"  
int main()
{
    int x[] = {12, 4, 3, 7, 17, 9, 11, 6};
    counting_iterator<int*, int> i(x, "Curly"), 
                                 j(x, "Moe"), 
                                 end(x+8, "Larry");
    cout << "Traversing array x\n"
         << "  from i (Curly) to end (Larry)\n";
    while (i != end) {
       cout << *i << endl;
       ++i;
    }
    cout << "After the traversal:\n";
    i.report();
    end.report();
    cout << "Assigning j (Moe) to i (Curly)."
         << endl;
    i = j;
    cout << "Searching the array\n"
         << "  from i (Moe) to end (Larry)\n"
         << "  using find\n";
    counting_iterator<int*, int> k =
        find(i, end, 9);
    cout << "After the find:\n";
    k.report();
    i.report();
    end.report();
}
